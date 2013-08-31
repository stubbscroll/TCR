/* kernel v2, simulatenously process vectors of 4 elements */

__kernel void pe255(__global long *a,long i) {
	int id=get_global_id(0),iter=4,z;
	long4 n={i+id*4,i+1+id*4,i+2+id*4,i+3+id*4};
	long4 x=(long4)(7000000);
	/* always perform one iteration */
	long4 y=(7000000+(n+6999999)/7000000)>>1;
again:
	/* swap betweem using x,y, duplicate the code */
	z=(x.x!=y.x)+(x.y!=y.y)+(x.z!=y.z)+(x.w!=y.w);
	if(!z) { a[id]+=iter; return; }
	x=(y+(n+y-1)/y)>>1;
	iter+=z;
	z=(x.x!=y.x)+(x.y!=y.y)+(x.z!=y.z)+(x.w!=y.w);
	if(!z) { a[id]+=iter; return; }
	y=(x+(n+x-1)/x)>>1;
	iter+=z;
	goto again;
}
