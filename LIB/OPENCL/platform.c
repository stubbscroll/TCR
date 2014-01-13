/* minimal example code for listing available opencl platforms */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

#if defined(__MINGW32__) || defined(__MINGW64__)
	#define LL "%I64d"
#else
	#define LL "%lld"
#endif

/* Prints out the error string for an error code */
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

void error(char *s) {
	puts(s);
	exit(1);
}

void clerror(char *s,cl_int err) {
	printf("%s: %s\n",s,errorstr(err));
	exit(1);
}

#define MAXCHAR 1024
void platforms() {
	cl_uint n,m,z,dim;
	cl_platform_id *id;
	cl_device_id *dev;
	cl_device_type dt;
	cl_int err;
	cl_ulong ul;
	cl_device_mem_cache_type cache;
	int i,j,k;
	size_t max;
	size_t maks[100];
	static char s[MAXCHAR];
	if(CL_SUCCESS!=clGetPlatformIDs(0,NULL,&n)) error("error getting platforms 1");
	if(NULL==(id=malloc(sizeof(cl_platform_id)*n))) error("out of memory");
	if(CL_SUCCESS!=clGetPlatformIDs(n,id,NULL)) error("error getting platforms 2");
	puts("list of opencl platforms on this machine:");
	for(i=0;i<n;i++) {
		printf("platform %d:\n",i);
		if(CL_SUCCESS!=clGetPlatformInfo(id[i],CL_PLATFORM_PROFILE,MAXCHAR,s,NULL))
			error("error getting platform 3");
		printf("  profile %s\n",s);
		if(CL_SUCCESS!=clGetPlatformInfo(id[i],CL_PLATFORM_VERSION,MAXCHAR,s,NULL))
			error("error getting platform 3");
		printf("  version %s\n",s);
		if(CL_SUCCESS!=clGetPlatformInfo(id[i],CL_PLATFORM_NAME,MAXCHAR,s,NULL))
			error("error getting platform 3");
		printf("  name    %s\n",s);
		if(CL_SUCCESS!=clGetPlatformInfo(id[i],CL_PLATFORM_VENDOR,MAXCHAR,s,NULL))
			error("error getting platform 3");
		printf("  vendor  %s\n",s);
		if(CL_SUCCESS!=clGetPlatformInfo(id[i],CL_PLATFORM_EXTENSIONS,MAXCHAR,s,NULL))
			error("error getting platform 3");
		printf("  ext     %s\n",s);
		if(CL_SUCCESS!=clGetDeviceIDs(id[i],CL_DEVICE_TYPE_ALL,0,NULL,&m))
			error("error getting device id");
		dev=malloc(m*sizeof(cl_device_id));
		if(CL_SUCCESS!=(err=clGetDeviceIDs(id[i],CL_DEVICE_TYPE_ALL,m,dev,NULL)))
			error("error getting device id");
		for(j=0;j<m;j++) {
			printf("  device %d-%d:\n",i,j);
			if(CL_SUCCESS!=clGetDeviceInfo(dev[j],CL_DEVICE_TYPE,sizeof(cl_device_type),&dt,NULL)) error("error getting info");
			printf("    type             ");
			if(dt&CL_DEVICE_TYPE_CPU) printf(" CPU");
			if(dt&CL_DEVICE_TYPE_GPU) printf(" GPU");
			if(dt&CL_DEVICE_TYPE_ACCELERATOR) printf(" Accel");
			if(dt&CL_DEVICE_TYPE_DEFAULT) printf(" Default");
			printf("\n");
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_VENDOR_ID,sizeof(cl_uint),&z,NULL))) clerror("error getting info",err);
			printf("    vendor id         %u\n",z);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(cl_uint),&z,NULL))) clerror("error getting info",err);
			printf("    max compute units %u\n",z);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,sizeof(cl_uint),&dim,NULL))) clerror("error getting info",err);
			printf("    max dimensions    %u\n",dim);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_MAX_WORK_ITEM_SIZES,sizeof(size_t)*dim,maks,NULL))) clerror("error getting info",err);
			printf("    max sizes        ");
			for(k=0;k<dim;k++) printf(" %d",(int)maks[k]);printf("\n");
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(size_t),&max,NULL))) clerror("error getting info",err);
			printf("    max wg size       %d\n",(int)max);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_MAX_MEM_ALLOC_SIZE,sizeof(cl_ulong),&ul,NULL))) clerror("error getting info",err);
			printf("    max mem alloc     "LL"\n",(long long)ul);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_LOCAL_MEM_SIZE,sizeof(cl_ulong),&ul,NULL))) clerror("error getting info",err);
			printf("    local mem         "LL"\n",(long long)ul);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_GLOBAL_MEM_SIZE,sizeof(cl_ulong),&ul,NULL))) clerror("error getting info",err);
			printf("    global mem        "LL"\n",(long long)ul);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,sizeof(cl_device_mem_cache_type),&cache,NULL))) clerror("error getting info",err);
			printf("    cache             ");
			if(cache==CL_NONE) puts("none");
			else if(cache==CL_READ_ONLY_CACHE) puts("read-only");
			else if(cache==CL_READ_WRITE_CACHE) puts("read-write");
			if(cache!=CL_NONE) {
				if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,sizeof(cl_ulong),&ul,NULL))) clerror("error getting info",err);
				printf("    global cache      "LL"\n",(long long)ul);
				if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,sizeof(cl_ulong),&ul,NULL))) clerror("error getting info",err);
				printf("    global cacheline  "LL"\n",(long long)ul);
			}
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_NAME,MAXCHAR,s,NULL))) clerror("error getting info",err);
			printf("    device name       %s\n",s);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_VENDOR,MAXCHAR,s,NULL))) clerror("error getting info",err);
			printf("    device vendor     %s\n",s);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DRIVER_VERSION,MAXCHAR,s,NULL))) clerror("error getting info",err);
			printf("    driver version    %s\n",s);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_PROFILE,MAXCHAR,s,NULL))) clerror("error getting info",err);
			printf("    device profile    %s\n",s);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_VERSION,MAXCHAR,s,NULL))) clerror("error getting info",err);
			printf("    device version    %s\n",s);
			if(CL_SUCCESS!=(err=clGetDeviceInfo(dev[j],CL_DEVICE_EXTENSIONS,MAXCHAR,s,NULL))) clerror("error getting info",err);
			printf("    device extensions %s\n",s);
		}
		free(dev);
	}
	free(id);
}

int main() {
	platforms();
	return 0;
}
