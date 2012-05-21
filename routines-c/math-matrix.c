#define M 64
int id[M][M],n;

void matrixaddmod(int a[M][M],int b[M][M],int mod,int r[M][M]) {
   int i,j;
   for(i=0;i<n;i++) for(j=0;j<n;j++) r[i][j]=(a[i][j]+b[i][j])%mod;
}

/* OK 25.01.2010 project euler 237 */
void matrixmulmod(int a[M][M],int b[M][M],int mod,int r[M][M]) {
   int x[M][M],i,j,k;
   for(i=0;i<n;i++) for(j=0;j<n;j++) {
      x[i][j]=0;
      for(k=0;k<n;k++) x[i][j]=(x[i][j]+a[i][k]*b[k][j])%mod;
   }
   for(i=0;i<n;i++) for(j=0;j<n;j++) r[i][j]=x[i][j];
}

/* OK 25.01.2010 project euler 237 */
void matrixpowmod(int a[M][M],int N,int mod,int r[M][M]) {
   int i,j;
   int b[M][M],x[M][M];
   if(!N) {
      for(i=0;i<n;i++) for(j=0;j<n;j++) r[i][j]=(i==j);
      return;
   }
   for(i=0;i<n;i++) for(j=0;j<n;j++) x[i][j]=(i==j),b[i][j]=a[i][j];
   while(1) {
      if(N&1) matrixmulmod(x,b,mod,x);
      N>>=1;
      if(!N) break;
      matrixmulmod(b,b,mod,b);
   }
   for(i=0;i<n;i++) for(j=0;j<n;j++) r[i][j]=x[i][j];
}

void matrixaccummod(int a[M][M],int N,int mod,int r[M][M])
{
   int t1[M][M],t2[M][M],t3[M][M],i,j;
   if(N==0) for(i=0;i<n;i++) for(j=0;j<n;j++) r[i][j]=0;
   else if(N&1) {
      matrixpowmod(a,N,mod,t1);
      matrixaccummod(a,N-1,mod,t2);
      matrixaddmod(t1,t2,mod,r);
   } else {
      matrixaccummod(a,N/2,mod,t1);
      matrixpowmod(a,N/2,mod,t2);
      matrixaddmod(t2,id,mod,t3);
      matrixmulmod(t1,t3,mod,r);
   }
}
