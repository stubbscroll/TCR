#define ISSET(a,row,col) (a[(row)][(col)>>6]&(1ULL<<((col)&63)))
#define SET(a,row,col) a[(row)][(col)>>6]|=1ULL<<((col)&63)

/* gaussian elimination mod 2 on bitmasks, A is n*m, b is n*o */
/* a is a malloced array of pointers, each a[i] is of size
   sizeof(ull)*(m+o+63)/64 */
/* return 0: no solutions, 1: one solution, 2: free variables */
/* WARNING, not tested on competition problems */
int bitgauss(ull **a,int n,int m,int o) {
	int i,j,k,z=m+o,c=0,fri=0,bz=(z+63)>>6;
	ull t;
	/* process each column */
	for(i=0;i<m;i++) {
		/* TODO check words instead of bits */
		for(j=c;j<n;j++) if(ISSET(a,j,i)) break;
		if(j==n) { fri=1; continue; }
		/* swap? */
		if(j>c)  for(k=0;k<bz;k++) {
			t=a[j][k],a[j][k]=a[c][k],a[c][k]=t;
		}
		/* subtract multiples of this row */
		for(j=0;j<n;j++) if(j!=c && ISSET(a,j,i)) {
			for(k=0;k<bz;k++) a[j][k]^=a[c][k];
		}
		c++;
	}
	/* detect no solution: rows with 0=b */
	for(i=0;i<n;i++) {
		/* TODO make bit-efficient solution later */
		for(j=0;j<m;j++) if(ISSET(a,i,j)) goto ok;
		for(;j<z;j++) if(ISSET(a,i,j)) return 0;
	ok:;
	}
	return 1+fri;
}
