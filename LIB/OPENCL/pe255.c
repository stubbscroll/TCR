/* brute force algorithm for PE255. written for educational purposes,
   not to cheat at project euler

   estimated runtime: 232200 seconds => 64.5 hours
   for 1e9 numbers:
   a naive serial CPU version takes 82.040 s
   OpenCL version on GPU takes 3.393 (2.580) s
   OpenCL version on CPU takes 15.222 (66.637) s
   speedup against CPU is 31.1x which is decent

   total number of iterations for 1e13 <= n < 1e13+1e9 is 6000000316
*/

#include <stdio.h>
#include <CL/cl.h>
typedef long long ll;
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

#define START 10000000000000LL
#define END (START+10000000000LL)
//#define END 100000000000000LL

/* choose kernel: 1 or 4 */
#define KERNEL 4

#define LOCAL 128
#define GLOBAL 10000000

int main() {
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue q;
	cl_program program;
	cl_kernel kernel;
	cl_mem buffer;
	cl_int err;
	static ll count[GLOBAL];
	char *src;
	ll ans=0,i;
	int j;
	size_t globalws=GLOBAL;
	size_t localws=LOCAL;

	clGetPlatformIDs(1,&platform,NULL);
	clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1,&device,NULL);
	context=clCreateContext(NULL,1,&device,NULL,NULL,NULL);
	q=clCreateCommandQueue(context,device,0,NULL);
#if KERNEL==1
	src=getsource("pe255.cl");
#else
	src=getsource("pe255-2.cl");
#endif
	program=clCreateProgramWithSource(context,1,(const char **)&src,NULL,&err);
	if(CL_SUCCESS!=err) clerror("couldn't create program",err);
	if(CL_SUCCESS!=(err=clBuildProgram(program,1,&device,NULL,NULL,NULL))) {
		static char s[1048576];
		size_t len;
		printf("error building program: %s\n",errorstr(err));
		clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,sizeof(s),s,&len);
		printf("build log:\n%s\n",s);
		return 0;
	}
	kernel=clCreateKernel(program,"pe255",&err);
	if(CL_SUCCESS!=err) clerror("couldn't create kernel",err);
	memset(count,0,GLOBAL*sizeof(ll));
	buffer=clCreateBuffer(context,CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR,GLOBAL*sizeof(cl_long),count,&err);
	if(CL_SUCCESS!=err) clerror("couldn't create buffer",err);
	for(i=START;i<END;i+=GLOBAL*KERNEL) {
//		printf("[%I64d/%I64d] ",i/GLOBAL,END/GLOBAL);
		if(CL_SUCCESS!=(err=clSetKernelArg(kernel,0,sizeof(cl_mem),&buffer))) clerror("error setting kernel argument 0",err);
		if(CL_SUCCESS!=(err=clSetKernelArg(kernel,1,sizeof(cl_long),&i))) clerror("error setting kernel argument 1",err);
		if(CL_SUCCESS!=(err=clEnqueueNDRangeKernel(q,kernel,1,NULL,&globalws,&localws,0,NULL,NULL)))
			clerror("error running kernel",err);
		clFinish(q);
	}
	if(CL_SUCCESS!=(err=clEnqueueReadBuffer(q,buffer,CL_TRUE,0,GLOBAL*sizeof(cl_long),count,0,NULL,NULL)))
		clerror("error copying result to host",err);
	clFinish(q);
	for(j=0;j<GLOBAL;j++) ans+=count[j];
	printf("\nanswer=%I64d %I64d %f\n",ans,END-START,(double)ans/(END-START));
	clReleaseMemObject(buffer);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(q);
	clReleaseContext(context);

	return 0;
}
