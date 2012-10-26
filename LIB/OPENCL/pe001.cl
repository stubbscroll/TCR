#define KERNEL 1000

__kernel void pe001(__global long *a) {
	int ix=get_global_id(0);
	long i=(long)ix*KERNEL;
	int j;
	long ans=0;
	for(j=0;j<KERNEL;j++) if((i+j)%3==0 || (i+j)%5==0) ans+=i+j;
	a[ix]=ans;
}
