/* an example of solving a slightly more complex problem on the GPU:
   find the number of triangles containing origo and having all three
   vertices strictly inside a circle of radius N centered at origo.
   algorithm: brute force, try all ways of placing the three vertices.
   complexity is O(N^6), for N=105 this is 209^6 triangles generated.

   divide the brute force: loop 2*N-1 times over the kernel calls, launch
   (2*N-1)*(2*N-1) kernels (with id equals to two of the x-coordinates,
   and receiving the third as a parameter), and loop over all y-coordinates

   N=105 took 4360 seconds
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <cl/cl.h>

#define PLATFORMID 0
#define DEVICEID 0

#if defined(__MINGW32__) || defined(__MINGW64__)
	#define LL "%I64d"
#else
	#define LL "%lld"
#endif

/* given error code, return error string */
const char *errorstr(cl_int err) {
	switch (err) {
	case CL_SUCCESS:                          return "Success!";
	case CL_DEVICE_NOT_FOUND:                 return "Device not found.";
	case CL_DEVICE_NOT_AVAILABLE:             return "Device not available";
	case CL_COMPILER_NOT_AVAILABLE:           return "Compiler not available";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:    return "Memory object allocation failure";
	case CL_OUT_OF_RESOURCES:                 return "Out of resources";
	case CL_OUT_OF_HOST_MEMORY:               return "Out of host memory";
	case CL_PROFILING_INFO_NOT_AVAILABLE:     return "Profiling information not available";
	case CL_MEM_COPY_OVERLAP:                 return "Memory copy overlap";
	case CL_IMAGE_FORMAT_MISMATCH:            return "Image format mismatch";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:       return "Image format not supported";
	case CL_BUILD_PROGRAM_FAILURE:            return "Program build failure";
	case CL_MAP_FAILURE:                      return "Map failure";
	case CL_INVALID_VALUE:                    return "Invalid value";
	case CL_INVALID_DEVICE_TYPE:              return "Invalid device type";
	case CL_INVALID_PLATFORM:                 return "Invalid platform";
	case CL_INVALID_DEVICE:                   return "Invalid device";
	case CL_INVALID_CONTEXT:                  return "Invalid context";
	case CL_INVALID_QUEUE_PROPERTIES:         return "Invalid queue properties";
	case CL_INVALID_COMMAND_QUEUE:            return "Invalid command queue";
	case CL_INVALID_HOST_PTR:                 return "Invalid host pointer";
	case CL_INVALID_MEM_OBJECT:               return "Invalid memory object";
	case CL_INVALID_IMAGE_SIZE:               return "Invalid image size";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:  return "Invalid image format descriptor";
	case CL_INVALID_SAMPLER:                  return "Invalid sampler";
	case CL_INVALID_BINARY:                   return "Invalid binary";
	case CL_INVALID_BUILD_OPTIONS:            return "Invalid build options";
	case CL_INVALID_PROGRAM:                  return "Invalid program";
	case CL_INVALID_PROGRAM_EXECUTABLE:       return "Invalid program executable";
	case CL_INVALID_KERNEL_NAME:              return "Invalid kernel name";
	case CL_INVALID_KERNEL_DEFINITION:        return "Invalid kernel definition";
	case CL_INVALID_KERNEL:                   return "Invalid kernel";
	case CL_INVALID_ARG_INDEX:                return "Invalid argument index";
	case CL_INVALID_ARG_VALUE:                return "Invalid argument value";
	case CL_INVALID_ARG_SIZE:                 return "Invalid argument size";
	case CL_INVALID_KERNEL_ARGS:              return "Invalid kernel arguments";
	case CL_INVALID_WORK_DIMENSION:           return "Invalid work dimension";
	case CL_INVALID_WORK_GROUP_SIZE:          return "Invalid work group size";
	case CL_INVALID_WORK_ITEM_SIZE:           return "Invalid work item size";
	case CL_INVALID_GLOBAL_OFFSET:            return "Invalid global offset";
	case CL_INVALID_EVENT_WAIT_LIST:          return "Invalid event wait list";
	case CL_INVALID_EVENT:                    return "Invalid event";
	case CL_INVALID_OPERATION:                return "Invalid operation";
	case CL_INVALID_GL_OBJECT:                return "Invalid OpenGL object";
	case CL_INVALID_BUFFER_SIZE:              return "Invalid buffer size";
	case CL_INVALID_MIP_LEVEL:                return "Invalid mip-map level";
	default:                                  return "Unknown";
	}
}

void clerror(char *s,cl_int err) {
	printf("%s: %s\n",s,errorstr(err));
	exit(1);
}

void error(char *s) {
	puts(s);
	exit(1);
}

/* get system time in seconds */
double walltime () {
	static struct timeval t;
	gettimeofday ( &t, NULL );
	return t.tv_sec + 1e-6 * t.tv_usec;
}

/* get source code */
char *getsource(char *s) {
	FILE *f;
	char *t;
	size_t len;
	if(NULL==(f=fopen(s,"r"))) error("couldn't open file");
	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);
	t=malloc(len+1);
	fread(t,len,1,f);
	t[len]=0;
	fclose(f);
	return t;
}

/* opencl variables */
cl_context context;
cl_command_queue queue;
cl_platform_id *platforms;
cl_device_id *devices;
cl_uint numplatforms;
cl_uint numdevices;

/* create context and create command queue, use platform and device as
   indicated by PLATFORMID and DEVICEID */
void initopencl() {
	cl_int err;
	/* get platform ids */
	if(CL_SUCCESS!=(err=clGetPlatformIDs(0,NULL,&numplatforms)))
		clerror("couldn't get number of platforms",err);
	platforms=malloc(numplatforms*sizeof(cl_platform_id));
	if(CL_SUCCESS!=(err=clGetPlatformIDs(numplatforms,platforms,NULL)))
		clerror("couldn't get platforms",err);
	/* get device */
	if(CL_SUCCESS!=(err=clGetDeviceIDs(platforms[PLATFORMID],CL_DEVICE_TYPE_ALL,0,NULL,&numdevices)))
		clerror("couldn't get number of devices",err);
	devices=malloc(numdevices*sizeof(cl_device_id));
	if(CL_SUCCESS!=(err=clGetDeviceIDs(platforms[PLATFORMID],CL_DEVICE_TYPE_ALL,numdevices,devices,NULL)))
		clerror("couldn't get devices",err);
	/* create context */
	context=clCreateContext(NULL,1,devices+DEVICEID,NULL,NULL,&err);
	if(CL_SUCCESS!=err) clerror("couldn't get context",err);
	/* create command queue */
	queue=clCreateCommandQueue(context,devices[DEVICEID],0,&err);
	if(CL_SUCCESS!=err) clerror("couldn't create command queue",err);
}

void shutdownopencl() {
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	free(devices);
	free(platforms);
}

/* here ends opencl-boilerplace */

#define N 105
#define N2 (2*N-1)
#define SIZE N2*N2*N2

cl_uint a[SIZE];

int main() {
	cl_int err;
	cl_kernel kernel;
	cl_program program;
	cl_mem buffer;
	cl_ulong ans,cur;
	int i,x3,y3;
	char *source;
	size_t localws[3];
	size_t globalws[3];

	initopencl();
	source=getsource("pe184.cl");
	/* create a program object and load source code into it */
	program=clCreateProgramWithSource(context,1,(const char **)&source,NULL,&err);
	if(CL_SUCCESS!=err) clerror("error creating program",err);
	/* builds a program associated with a program object */
	if(CL_SUCCESS!=(err=clBuildProgram(program,1,devices+DEVICEID,NULL,NULL,NULL)))
		clerror("error building program",err);
	/* create kernel object from built program */
	kernel=clCreateKernel(program,"pe184",&err);
	if(CL_SUCCESS!=err) clerror("error creating kernel",err);
	free(source);

	/* create memory buffer: one cl_ulong for each kernel, a total of MAX=N/KERNEL */
	buffer=clCreateBuffer(context,CL_MEM_READ_WRITE,N2*N2*sizeof(cl_uint),NULL,&err);
	if(CL_SUCCESS!=err) clerror("error creating buffer",err);

	globalws[0]=N2;
	globalws[1]=N2;
	globalws[2]=1;

	localws[0]=N2;
	localws[1]=1;
	localws[2]=1;

	ans=0;
	for(x3=0;x3<N;x3++) {
		for(y3=0;y3<N;y3++) if(x3*x3+y3*y3<N*N) {
			/* set up kernel arguments */
			if(CL_SUCCESS!=(err=clSetKernelArg(kernel,0,sizeof(cl_mem),&buffer))) clerror("error setting kernel argument 0",err);
			if(CL_SUCCESS!=(err=clSetKernelArg(kernel,1,sizeof(cl_int),&x3))) clerror("error setting kernel argument 1",err);
			if(CL_SUCCESS!=(err=clSetKernelArg(kernel,2,sizeof(cl_int),&y3))) clerror("error setting kernel argument 2",err);

			/* run kernel */
			if(CL_SUCCESS!=(err=clEnqueueNDRangeKernel(queue,kernel,3,NULL,globalws,localws,0,NULL,NULL)))
				clerror("error running kernel",err);
			/* wait until kernel has finished */
			if(CL_SUCCESS!=(err=clFinish(queue))) clerror("error waiting for queue",err);

			/* copy to host memory */
			if(CL_SUCCESS!=(err=clEnqueueReadBuffer(queue,buffer,CL_TRUE,0,N2*N2*sizeof(cl_uint),a,0,NULL,NULL)))
				clerror("error copying result to host",err);
			if(CL_SUCCESS!=(err=clFinish(queue))) clerror("error waiting for queue",err);

			/* assemble final answer */
			for(cur=i=0;i<N2*N2;i++) cur+=a[i];
			if(x3) cur*=2;
			if(y3) cur*=2;
			ans+=cur;
		}
		printf("done x3=%d/%d\n",x3+1,N);
	}
	printf("answer: "LL"\n",ans/6);

	/* TODO deallocate buffer */

	clReleaseKernel(kernel);
	clReleaseProgram(program);
	shutdownopencl();
	return 0;
}