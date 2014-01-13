/* create nice images of the mandelbrot set using opencl! */

/* usage:
   mandel [n]
   no parameter: no image saved
   n=0: save image from host calculations
   n=1: save image from device calculations
*/

/* i got compile errors when i used double in the kernel, so this version uses float */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <CL/cl.h>

typedef unsigned char uchar;

/* problem size (image resolution): must be multiples of LOCALX, LOCALY */
/* it seems to be ok if these values exceed max dimension sizes */

#define XSIZE 8192
#define YSIZE 4096

/* local work sizes */
/* warning, must not exceed capabilities of device
   - on radeon hd 6850, x*y<=1024 (so 16*16 is ok, somehow 32*32 and 16*32 aren't ok)
   - on radeon mobility hd 4570, x*y<=128 (so 8*8 is ok, 8*16 is ok, 16*16 is not ok)
*/
/* warning, these values should not be too far away from the device's capability as possible,
   or cores will be wasted */
#define LOCALX 16
#define LOCALY 8

#define MAXITER 255

/* mandelbrot variables */
double xleft=-2.01;
double xright=1;
double yupper;
double ycenter=1e-6;
double step;

int host_pixel[XSIZE*YSIZE];
int device_pixel[XSIZE*YSIZE];

#define PIXEL(i,j) ((i)+(j)*XSIZE)

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

/* host mandelbrot routines */
void host_calculate() {
	int i,j,iter;
	double cx,cy,zx,zy,temp;
	for(j=0;j<YSIZE;j++) {
		for(i=0;i<XSIZE;i++) {
			/* Calculate the number of iterations until divergence for each pixel.
			   If divergence never happens, return MAXITER */
			iter=0;
			zx=cx = (xleft + step*i);
			zy=cy = (yupper - step*j);
			while(zx*zx + zy*zy < 4) {
				temp = zx*zx - zy*zy + cx;
				zy = 2*zx*zy + cy;
				zx = temp;
				if(++iter==MAXITER) break;
			}
			host_pixel[PIXEL(i,j)]=iter;
		}
	}
}

/* save 24-bits bmp file, buffer must be in bmp format: upside-down */
void savebmp(char *name,uchar *buffer,int x,int y) {
	FILE *f=fopen(name,"wb");
	unsigned int size=x*y*3+54;
	uchar header[54]={'B','M',size&255,(size>>8)&255,(size>>16)&255,size>>24,0,
		0,0,0,54,0,0,0,40,0,0,0,x&255,x>>8,0,0,y&255,y>>8,0,0,1,0,24,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	if(!f) {
		printf("Error writing image to disk.\n");
		return;
	}
	fwrite(header,1,54,f);
	fwrite(buffer,1,x*y*3,f);
	fclose(f);
}

/* given iteration number, set a colour */
void fancycolour(uchar *p,int iter) {
	if(iter==MAXITER);
	else if(iter<8) { p[0]=128+iter*16; p[1]=p[2]=0; }
	else if(iter<24) { p[0]=255; p[1]=p[2]=(iter-8)*16; }
	else if(iter<160) { p[0]=p[1]=255-(iter-24)*2; p[2]=255; }
	else { p[0]=p[1]=(iter-160)*2; p[2]=255-(iter-160)*2; }
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
cl_kernel kernel;
cl_program program;

/* hardcode id */
#define PLATFORMID 0
#define DEVICEID 0

#define MAXCHAR 1024

int main(int argc,char **argv) {
	double hosttime=0,devtime=0,memtime=0,overheadtime=0,start;
	cl_int err;
	cl_uint numplatforms;
	cl_uint numdevices;
	cl_platform_id *platforms;
	cl_device_id *devices;
	cl_mem devmem;
	char *source;
	int i,j,k,p,xsize=XSIZE,ysize=YSIZE,d,errors;
	unsigned char *buffer;
	size_t localws[2];
	size_t globalws[2];
	float fxleft,fyupper,fstep;

	step=(xright-xleft)/XSIZE;
	yupper=ycenter+(step*YSIZE)/2;

	start=walltime();
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

	source=getsource("mandel.cl");

	/* create a program object and load source code into it */
	program=clCreateProgramWithSource(context,1,(const char **)&source,NULL,&err);
	if(CL_SUCCESS!=err) clerror("error creating program",err);

	/* builds a program associated with a program object */
	if(CL_SUCCESS!=(err=clBuildProgram(program,1,devices+DEVICEID,NULL,NULL,NULL)))
		clerror("error building program",err);

	/* create kernel object from built program */
	kernel=clCreateKernel(program,"mandel",&err);
	if(CL_SUCCESS!=err) clerror("error creating kernel",err);
	free(source);
	overheadtime+=walltime()-start;

	start=walltime();
	/* create device memory buffer */
	devmem=clCreateBuffer(context,CL_MEM_READ_WRITE, XSIZE*YSIZE*sizeof(int),NULL,&err);
	if(CL_SUCCESS!=err) clerror("error creating buffer",err);
	memtime+=walltime()-start;

	start=walltime();
	/* create kernel arguments */
	fxleft=xleft; fyupper=yupper; fstep=step;
	if(CL_SUCCESS!=(err=clSetKernelArg(kernel,0,sizeof(cl_mem),&devmem))) clerror("error setting kernel argument 0",err);
	if(CL_SUCCESS!=(err=clSetKernelArg(kernel,1,sizeof(cl_float),&fxleft))) clerror("error setting kernel argument 1",err);
	if(CL_SUCCESS!=(err=clSetKernelArg(kernel,2,sizeof(cl_float),&fyupper))) clerror("error setting kernel argument 2",err);
	if(CL_SUCCESS!=(err=clSetKernelArg(kernel,3,sizeof(cl_float),&fstep))) clerror("error setting kernel argument 3",err);
	if(CL_SUCCESS!=(err=clSetKernelArg(kernel,4,sizeof(cl_int),&xsize))) clerror("error setting kernel argument 4",err);
	if(CL_SUCCESS!=(err=clSetKernelArg(kernel,5,sizeof(cl_int),&ysize))) clerror("error setting kernel argument 5",err);

	globalws[0]=XSIZE;
	globalws[1]=YSIZE;
	localws[0]=LOCALX;
	localws[1]=LOCALY;

	/* run kernel */
	if(CL_SUCCESS!=(err=clEnqueueNDRangeKernel(queue,kernel,2,NULL,globalws,localws,0,NULL,NULL)))
		clerror("error running kernel",err);
	/* wait until kernel has finished */
	if(CL_SUCCESS!=(err=clFinish(queue))) clerror("error waiting for queue",err);
	devtime+=walltime()-start;

	start=walltime();
	/* copy to host memory */
	if(CL_SUCCESS!=(err=clEnqueueReadBuffer(queue,devmem,CL_TRUE,0,XSIZE*YSIZE*sizeof(int),device_pixel,0,NULL,NULL)))
		clerror("error copying result to host",err);
	if(CL_SUCCESS!=(err=clFinish(queue))) clerror("error waiting for queue",err);
	memtime+=walltime()-start;
	
	/* shutdown */
	start=walltime();
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	overheadtime+=walltime()-start;

	/* host calculation */
	start=walltime();
	host_calculate();
	hosttime+=walltime()-start;

	printf("host calculation time   : %7.3f\n",hosttime);
	printf("device calculation time : %7.3f\n",devtime);
	printf("memory transfer time    : %7.3f\n",memtime);
	printf("other opencl overhead   : %7.3f\n",overheadtime);
	printf("total device time       : %7.3f\n",devtime+memtime+overheadtime);
	printf("speedup                 : %7.3f\n",hosttime/(devtime+memtime+overheadtime));

	/* compare results */
	for(errors=i=0;i<XSIZE;i++) for(j=0;j<YSIZE;j++) {
		d=host_pixel[PIXEL(i,j)]-device_pixel[PIXEL(i,j)];
		if(d<0) d=-d;
		if(d>1) errors++;
	}
	printf("errors: %d\n",errors);
	
	if(argc>1) {
		k=strtol(argv[1],NULL,10);
		/* create nice image from iteration counts. take care to create it upside
			 down (bmp format) */
		buffer=(unsigned char *)calloc(XSIZE*YSIZE*3,1);
		for(i=0;i<XSIZE;i++) {
			for(j=0;j<YSIZE;j++) {
				p=((YSIZE-j-1)*XSIZE+i)*3;
				fancycolour(buffer+p,(k?device_pixel:host_pixel)[PIXEL(i,j)]);
			}
		}
		/* write image to disk */
		savebmp("mandel.bmp",buffer,XSIZE,YSIZE);
	}
	return 0;
}
