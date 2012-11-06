/* a rather minimal example demonstrating printf from kernel! */
/* nb, requires OpenCL 1.2 */

#include <stdio.h>
#include <CL/cl.h>

const char *src=
"__kernel void print() {              \n"
"  int g=get_global_id(0);            \n"
"  int l=get_local_id(0);             \n"
"  printf(\"hello from %i %i\\n\",g,l);\n"
"}\n";

int main() {
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue q;
	cl_program program;
	cl_kernel kernel;
	size_t globalws=16;
	size_t localws=4;
	/* just get any GPU device on the first platform */
	clGetPlatformIDs(1,&platform,NULL);
	clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1,&device,NULL);
	/* create context and command queue on device */
	context=clCreateContext(NULL,1,&device,NULL,NULL,NULL);
	q=clCreateCommandQueue(context,device,0,NULL);
	/* build program and compile kernel */
	program=clCreateProgramWithSource(context,1,&src,NULL,NULL);
	/* obtain compile error */
	if(clBuildProgram(program,1,&device,NULL,NULL,NULL)) {
		char s[2048];
		size_t len;
		clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,sizeof(s),s,&len);
		printf("build log:\n%s\n",s);
		return 0;
	}
	kernel=clCreateKernel(program,"print",NULL);
	/* launch kernel */
	clEnqueueNDRangeKernel(q,kernel,1,NULL,&globalws,&localws,0,NULL,NULL);
	clFinish(q);
	return 0;
}
