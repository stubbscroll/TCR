/* minimal example code for listing available opencl platforms */

#include <cl/cl.h>
#include <stdio.h>
#include <stdlib.h>

void error(char *s) {
  puts(s);
  exit(1);
}

#define MAXCHAR 80
void platforms() {
  cl_uint n;
  cl_platform_id *id;
  int i;
  char s[MAXCHAR];
  if(CL_SUCCESS!=clGetPlatformIDs(0,NULL,&n)) error("error getting platforms 1");
  if(NULL==(id=malloc(sizeof(cl_platform_id)*n))) error("out of memory");
  if(CL_SUCCESS!=clGetPlatformIDs(n,id,NULL)) error("error getting platforms 2");
  puts("list of opencl platforms on this machine:");
  for(i=0;i<n;i++) {
    if(CL_SUCCESS!=clGetPlatformInfo(id[i],CL_PLATFORM_VENDOR,MAXCHAR,s,NULL))
      error("error getting platform 3");
    printf("%d: %s\n",i,s);
  }
  free(id);
}

int main() {
  platforms();
  return 0;
}
