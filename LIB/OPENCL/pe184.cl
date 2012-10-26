/* warning, this N must be the same as in the main program */
#define N 52
#define N2 (N*2-1)

__kernel void pe184(__global int *a,int z) {
	int ix=get_global_id(0),iy=get_global_id(1),iz=z;
	int x1=ix-N+1,x2=iy-N+1,x3=iz-N+1,y1,y2,y3,r=0;
	int idx=ix+iy*N2;
	int a0,a1,a2,a3;
	for(y1=-N;y1<N;y1++) if(x1*x1+y1*y1<N*N && (x1 || y1)) {
		for(y2=-N;y2<N;y2++) if(x2*x2+y2*y2<N*N && (x2 || y2)) {
			for(y3=-N;y3<N;y3++) if(x3*x3+y3*y3<N*N) if(x3 || y3) {
				/* calculate triangle area of our triangle. it it is zero, then
				   skip it because it's degenerate */
				a0=(x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
				if(a0==0) continue;
				if(a0<0) a0=-a0;
				/* replace each coordinate by origo in turn and calculate the
				   suim of all resulting triangle areas (absolute value).
				   if some of these areas are 0, then origo is not contained */
				a1=x2*y3-x3*y2; if(a1==0) continue; if(a1<0) a1=-a1;
				a2=x3*y1-x1*y3; if(a2==0) continue; if(a2<0) a2=-a2;
				a3=x1*y2-x2*y1; if(a3==0) continue; if(a3<0) a3=-a3;
				if(a0==a1+a2+a3) r++;
			}
		}
	}
	a[idx]=r;
}
