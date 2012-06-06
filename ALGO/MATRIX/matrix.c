#define M 2

typedef long long ll;

/*	OK UVa 10229 N<2^31 26.05.2012 */
/*	OK UVa 12470 N<=10^16 06.06.2012 */
void matrixmulmod(int a[M][M],int b[M][M],int mod,int r[M][M],int n) {
	int x[M][M],i,j,k;
	for(i=0;i<n;i++) for(j=0;j<n;j++) {
		x[i][j]=0;
		for(k=0;k<n;k++) x[i][j]=(x[i][j]+(ll)a[i][k]*b[k][j])%mod;
	}
	for(i=0;i<n;i++) for(j=0;j<n;j++) r[i][j]=x[i][j];
}

/*	OK UVa 10229 N<2^31 26.05.2012 */
void matrixpowmod(int a[M][M],int N,int mod,int r[M][M],int n) {
	int i,j;
	int b[M][M],x[M][M];
	if(!N) {
		for(i=0;i<n;i++) for(j=0;j<n;j++) r[i][j]=(i==j);
		return;
	}
	for(i=0;i<n;i++) for(j=0;j<n;j++) x[i][j]=(i==j),b[i][j]=a[i][j];
	while(1) {
		if(N&1) matrixmulmod(x,b,mod,x,n);
		N>>=1;
		if(!N) break;
		matrixmulmod(b,b,mod,b,n);
	}
	for(i=0;i<n;i++) for(j=0;j<n;j++) r[i][j]=x[i][j];
}
