/* stupid kernel, direct translation of serial code */

__kernel void pe255(__global long *a,long i) {
	int id=get_global_id(0),iter=0,x,y;
	long n=i+id;
	/* hardcoded for 14-digit numbers */
	x=7000000;
	do {
		y=x;
		iter++;
		x=(y+(n+y-1)/y)>>1;
	} while(x!=y);
	a[id]+=iter;
}
