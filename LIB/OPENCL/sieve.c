/* rather verbose example with full error-checking */
/* nb, this program is much slower than a straightforward serial cpu
   implementation. also, the division into work groups is messed up, i made
   this program before i understood how it works.
   also, no bitmasks make me sad */

#include <cl/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char *s) {
	puts(s);
	exit(1);
}

/* MAXP=4063 or higher crashes */
#define MAXP 10000000

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

/* init memory block */
void initmem() {
	if(NULL==(in=malloc(MAXP*sizeof(cl_uint)))) error("out of memory");
	memset(in,1,MAXP);
	in[0]=in[1]=0;
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
	cl_context_properties cps[3]={CL_CONTEXT_PLATFORM,(cl_context_properties)id,0};
	context=clCreateContextFromType(cps,CL_DEVICE_TYPE_CPU,NULL,NULL,&status);
	if(CL_SUCCESS!=status) error("couldn't create context");
	if(CL_SUCCESS!=clGetContextInfo(context,CL_CONTEXT_DEVICES,0,NULL,(size_t *)&n))
		error("error getting context info 1");
	if(!n) error("no devices!");
	if(NULL==(devices=malloc(n))) error("out of memory");
	if(CL_SUCCESS!=clGetContextInfo(context,CL_CONTEXT_DEVICES,n,devices,NULL))
		error("error getting context info 2");
	/*  create opencl command queue */
	cmdq=clCreateCommandQueue(context,devices[0],0,&status);
	if(CL_SUCCESS!=status) error("error creating command queue");
	/*  create memory buffers */
	initmem();
	/*  copy contents of in to inbuffer */
	inbuffer=clCreateBuffer(context,CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR,
		MAXP,in,&status);
	if(CL_SUCCESS!=status) {
		printf("max alloc %d\n",(int)CL_DEVICE_MAX_MEM_ALLOC_SIZE);
		printf("status %d\n",status);
		error("error creating input buffer");
	}
	loadkernel("sieve.cl");
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
	size_t globalthreads[3];
	int prim,max,i,antall;
	cl_event events[2];
	/* get max work item dimensions and max work item sizes */
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
	printf("max work group size: %d [",(int)maxworkgroupsize);
	for(i=0;i<maxdims;i++) printf("%d ",(int)maxworkitemsizes[i]);
	printf("]\n");

	prim=2;
	max=MAXP;
	while(1) {
		while(prim*prim<MAXP && !in[prim]) prim++;
		if(prim*prim>=MAXP) break;
		antall=(MAXP-prim)/prim;
		if(antall<1048576) {
			globalthreads[0]=1024;
			globalthreads[1]=antall/1024+1;
			globalthreads[2]=1;
		} else {
			globalthreads[0]=1024;
			globalthreads[1]=1024;
			globalthreads[2]=antall/1048576+1;
		}
		if(CL_SUCCESS!=clSetKernelArg(kernel,0,sizeof(cl_mem),(void *)&inbuffer))
			error("error setting kernel arg (output)");
		if(CL_SUCCESS!=clSetKernelArg(kernel,1,sizeof(int),(void *)&prim))
			error("error setting kernel arg (input)");
		if(CL_SUCCESS!=clSetKernelArg(kernel,2,sizeof(int),(void *)&max))
			error("error setting kernel arg (input)");
		/* enqueue kernel run call */
		if(CL_SUCCESS!=clEnqueueNDRangeKernel(cmdq,kernel,3,NULL,globalthreads,
			0,0,NULL,&events[0])) error("error enqueueing kernel");
		/*  wait for the kernel call to finish */
		if(CL_SUCCESS!=clWaitForEvents(1,&events[0])) error("error waiting");
		if(CL_SUCCESS!=clReleaseEvent(events[0])) error("error releasing");
		/* enqueue readbuffer */
		if(CL_SUCCESS!=clEnqueueReadBuffer(cmdq,inbuffer,CL_TRUE,0,MAXP,in,0,
			NULL,&events[1])) error("error enqueueing readbuffer");
		if(CL_SUCCESS!=clWaitForEvents(1,&events[1])) error("error waiting 1");
		if(CL_SUCCESS!=clReleaseEvent(events[1])) error("error releasing 1");
		prim++;
	}
	antall=0;
	for(i=2;i<MAXP;i++) if(in[i]) antall++;
	printf("number of primes %d\n",antall);
}

int main() {
	init();
	calculate();
	shutdown();
	return 0;
}
