/* super duper minimal opencl example (no error checking):
   write global id to array.
   ripped from AMD and reformatted slightly */

#include <stdio.h>
#include <CL/cl.h>

#define ITEMS 512

const char *src=
"__kernel void hello(__global uint *dst) {    \n"
"  dst[get_global_id(0)]=get_global_id(0);    \n"
"}\n";

int main() {
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue q;
	cl_program program;
	cl_kernel kernel;
	cl_mem buffer;
	cl_uint *p;
	size_t globalws=ITEMS;
	int i;
	/* just get any GPU device on the first platform */
	clGetPlatformIDs(1,&platform,NULL);
	clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1,&device,NULL);
	/* create context and command queue on device */
	context=clCreateContext(NULL,1,&device,NULL,NULL,NULL);
	q=clCreateCommandQueue(context,device,0,NULL);
	/* build program and compile kernel */
	program=clCreateProgramWithSource(context,1,&src,NULL,NULL);
	clBuildProgram(program,1,&device,NULL,NULL,NULL);
	kernel=clCreateKernel(program,"hello",NULL);
	buffer=clCreateBuffer(context,CL_MEM_WRITE_ONLY,ITEMS*sizeof(cl_uint),NULL,NULL);
	/* launch kernel, let opencl pick local work size by NULL-ing it */
	clSetKernelArg(kernel,0,sizeof(buffer),&buffer);
	clEnqueueNDRangeKernel(q,kernel,1,NULL,&globalws,NULL,0,NULL,NULL);
	clFinish(q);
	p=clEnqueueMapBuffer(q,buffer,CL_TRUE,CL_MAP_READ,0,ITEMS*sizeof(cl_uint),0,NULL,NULL,NULL);
	for(i=0;i<ITEMS;i++) printf("%d: %d\n",i,p[i]);
	/* very minimal example, so no cleanup */
	return 0;
}
