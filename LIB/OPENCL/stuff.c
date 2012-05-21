/*  rather verbose example with full error-checking */

#include <cl/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char *s) {
  puts(s);
  exit(1);
}

#define MAXP 1024

cl_context context;
cl_device_id *devices;
cl_command_queue cmdq;
cl_program program;
cl_kernel kernel;

cl_uchar *in;
cl_mem inbuffer;

void loadkernel(char *s) {
  FILE *f;
  char *t;
  size_t len;
  int status;
  if(NULL==(f=fopen(s,"r"))) error("couldn't open file");
  fseek(f,0,SEEK_END);
  len=ftell(f);
  fseek(f,0,SEEK_SET);
  t=malloc(len+1);
  fread(t,len,1,f);
  fclose(f);
  len=strlen(t);
  program=clCreateProgramWithSource(context,1,(const char **)&t,&len,&status);
  if(CL_SUCCESS!=status) error("error creating program");
  if(CL_SUCCESS!=clBuildProgram(program,1,devices,NULL,NULL,NULL))
    error("error building program");
  kernel=clCreateKernel(program,"sieve",&status);
  if(CL_SUCCESS!=status) error("error creating kernel");
}

/*  do sieve for primes up to 7. we don't want several work-items to
    write to the same byte */
void initmem() {
  int i;
  if(NULL==(in=malloc(MAXP*sizeof(cl_uint)))) error("out of memory");
  /*  from byte 1, our sieve is cyclic with a period of 105 when we sieve
      up to 7 */
  memset(in,255,106);
  in[0]=252;
  for(i=4;i<848;i+=2) in[i>>3]&=~(1<<(i&7));
  for(i=9;i<848;i+=3) in[i>>3]&=~(1<<(i&7));
  for(i=25;i<848;i+=5) in[i>>3]&=~(1<<(i&7));
  for(i=49;i<848;i+=7) in[i>>3]&=~(1<<(i&7));
  /*  duplicate up to MAXP. probably slow, bleh, do this on device? */
  for(i=106;i<MAXP;i++) in[i]=in[i-105];
}

#define MAXCHAR 80
void init() {
  cl_uint n;
  cl_platform_id *ids,id=NULL;
  int i,status;
  char s[MAXCHAR];
  /*  choose AMD platform if possible, otherwise just pick one */
  if(CL_SUCCESS!=clGetPlatformIDs(0,NULL,&n))
    error("error getting platforms 1");
  if(NULL==(ids=malloc(sizeof(cl_platform_id)*n))) error("out of memory");
  if(CL_SUCCESS!=clGetPlatformIDs(n,ids,NULL))
    error("error getting platforms 2");
  for(i=0;i<n;i++) {
    if(CL_SUCCESS!=clGetPlatformInfo(ids[i],CL_PLATFORM_VENDOR,MAXCHAR,s,NULL))
      error("error getting platform 3");
    if(!strcmp(s,"Advanced Micro Devices, Inc.")) {
      id=ids[i];
      break;
    }
  }
  if(i==n) id=ids[0];
  free(ids);
  /*  get context stuff */
  cl_context_properties cps[3]={CL_CONTEXT_PLATFORM,
    (cl_context_properties)id,0};
  context=clCreateContextFromType(cps,CL_DEVICE_TYPE_CPU,NULL,NULL,&status);
  if(CL_SUCCESS!=status) error("couldn't create context");
  if(CL_SUCCESS!=clGetContextInfo(context,CL_CONTEXT_DEVICES,0,NULL,&n))
    error("error getting context info 1");
  if(!n) error("no devices!");
  if(NULL==(devices=malloc(n))) error("out of memory");
  if(CL_SUCCESS!=clGetContextInfo(context,CL_CONTEXT_DEVICES,n,devices,NULL))
    error("error getting context info 2");

  /*  TODO display stats about devices? */

  /*  create opengl command queue */
  cmdq=clCreateCommandQueue(context,devices[0],0,&status);
  if(CL_SUCCESS!=status) error("error creating command queue");
  /*  create memory buffers */
  initmem();
  /*  copy contents of in to inbuffer */
  inbuffer=clCreateBuffer(context,CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR,
    MAXP,in,&status);
  if(CL_SUCCESS!=status) {
    printf("max alloc %zd\n",CL_DEVICE_MAX_MEM_ALLOC_SIZE);
    printf("status %d\n",status);
    error("error creating input buffer");
  }
  loadkernel("stuff.cl");
}

void shutdown() {
  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseMemObject(inbuffer);
  clReleaseCommandQueue(cmdq);
  clReleaseContext(context);
  free(devices);
  free(in);
}

void calculate() {
  cl_uint maxdims;
  size_t maxworkgroupsize;
  size_t maxworkitemsizes[3];
  cl_uint computeunits;
  size_t globalthreads[1];
  int prim,max,i;
  cl_event events[2];
  /*  get max work item dimensions and max work item sizes */
  if(CL_SUCCESS!=clGetDeviceInfo(devices[0],CL_DEVICE_MAX_WORK_GROUP_SIZE,
    sizeof(size_t),(void *)&maxworkgroupsize,NULL))
      error("error getting work grup size");
  if(CL_SUCCESS!=clGetDeviceInfo(devices[0],CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
    sizeof(cl_uint),(void *)&maxdims,NULL))
      error("error getting work item dimensions");
  if(CL_SUCCESS!=clGetDeviceInfo(devices[0],CL_DEVICE_MAX_WORK_ITEM_SIZES,
    sizeof(size_t)*maxdims,(void *)maxworkitemsizes,NULL))
      error("error getting work item sizes");
  if(CL_SUCCESS!=clGetDeviceInfo(devices[0],CL_DEVICE_MAX_COMPUTE_UNITS,
    sizeof(cl_uint),(void *)&computeunits,NULL))
      error("error getting max compute units");
  printf("number of compute units: %u\n",computeunits);
  printf("max dimensions: %u\n",maxdims);
  printf("max work group size: %zd [",maxworkgroupsize);
  for(int i=0;i<maxdims;i++) printf("%zd ",maxworkitemsizes[i]);
  printf("]\n");

  prim=8;
  max=MAXP;
  while(1) {
    while(prim*prim<MAXP*8 && !(in[prim>>3]&(1<<(prim&7)))) prim++;
    if(prim*prim>=MAXP*8) break;
    printf("%d ",prim);
    globalthreads[0]=(MAXP*8+prim-1)/prim;
    if(CL_SUCCESS!=clSetKernelArg(kernel,0,sizeof(cl_mem),(void *)&inbuffer))
      error("error setting kernel arg (output)");
    if(CL_SUCCESS!=clSetKernelArg(kernel,1,sizeof(int),(void *)&prim))
      error("error setting kernel arg (input)");
    if(CL_SUCCESS!=clSetKernelArg(kernel,2,sizeof(int),(void *)&max))
      error("error setting kernel arg (input)");
    /*  enqueue kernel run call */
    if(CL_SUCCESS!=clEnqueueNDRangeKernel(cmdq,kernel,1,NULL,globalthreads,
      0,0,NULL,&events[0])) error("error enqueueing kernel");
    /*  wait for the kernel call to finish */
    if(CL_SUCCESS!=clWaitForEvents(1,&events[0])) error("error waiting");
    if(CL_SUCCESS!=clReleaseEvent(events[0])) error("error releasing");
    /*  enqueue readbuffer */
    if(CL_SUCCESS!=clEnqueueReadBuffer(cmdq,inbuffer,CL_TRUE,0,MAXP,in,0,
      NULL,&events[1])) error("error enqueueing readbuffer");
    if(CL_SUCCESS!=clWaitForEvents(1,&events[1])) error("error waiting 1");
    if(CL_SUCCESS!=clReleaseEvent(events[1])) error("error releasing 1");
    break;
  }
  printf("\nprimes:");
  for(i=2;i<MAXP*8;i++) if(in[i>>3]&(1<<(i&7))) printf(" %d",i);printf("\n");
}

int main() {
  init();
  calculate();
  shutdown();
  return 0;
}
