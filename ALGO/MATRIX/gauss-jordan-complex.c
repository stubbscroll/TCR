#include <stdio.h>
#include <math.h>

#define MAXR 111
#define MAXC 112
#define EPS 1e-9
double a[MAXR][MAXC],b[MAXR][MAXC];
int n;

/* check if a+bi is equal to c+di */
double ceq(double a,double b,double c,double d) {
	return fabs(a-c)<EPS && fabs(b-d)<EPS;
}

void complexmul(double a,double b,double c,double d,double *e,double *f) {
	double x=a*c-b*d,y=a*d+b*c;
	*e=x; *f=y;
}

void printmatrix() {
	int i,j;
	for(i=0;i<n;i++) {
		for(j=0;j<=n;j++) printf("(%.1f,%.1f) ",a[i][j],b[i][j]);
		printf("\n");
	}
}

/* n,m: number of rows/columns of A, o: number of columns in b */
/* return 0: no solutions 1: one solution, 2: free variables */
/* OK UVa 472 n<=99 0.060 seconds 16.06.2012 */
int complexgauss(double a[MAXR][MAXC],double b[MAXR][MAXC],int n,int m,int o) {
	int i,j,k,z=m+o,c=0,fri=0;
	double t,x,y,s;
	/* process each column */
	for(i=0;i<m;i++) {
		for(j=c;j<n;j++) if(!ceq(a[j][i],b[j][i],0,0)) break;
		if(j==n) { fri=1; continue; }
		/* swap? */
		if(j>c) for(k=i;k<z;k++) {
			t=a[j][k],a[j][k]=a[c][k],a[c][k]=t,
			t=b[j][k],b[j][k]=b[c][k],b[c][k]=t;
		}
		/* divide such that A_c,i becomes 1: x,y is multiplicative inverse */
		x=a[c][i]/(a[c][i]*a[c][i]+b[c][i]*b[c][i]);
		y=-b[c][i]/(a[c][i]*a[c][i]+b[c][i]*b[c][i]);
		for(k=i;k<z;k++) complexmul(a[c][k],b[c][k],x,y,&a[c][k],&b[c][k]);
		/* subtract multiples of this row */
		for(j=0;j<n;j++) if(j!=c && !ceq(a[j][i],b[j][i],0,0)) {
			x=a[j][i];
			y=b[j][i];
			for(k=0;k<z;k++) {
				complexmul(a[c][k],b[c][k],x,y,&s,&t);
				a[j][k]-=s; b[j][k]-=t;
			}
		}
		c++;
	}
	/* detect no solution: rows with 0=b */
	for(i=0;i<n;i++) {
		for(j=0;j<m;j++) if(!ceq(a[i][j],b[i][j],0,0)) goto ok;
		for(;j<z;j++) if(!ceq(a[i][j],b[i][j],0,0)) return 0;
	ok:;
	}
	return 1+fri;
}
