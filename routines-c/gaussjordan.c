#define M 26
#define N 27
#define EPS 1e-8

int deq(double a,double b)
{
   return fabs(a-b)<EPS;
}

void gauss(double a[M][N],int n,int m)
{
   double r;
   int i,j,k,l;

   for(i=0;i<n;i++) {
      for(j=0;j<n;j++) {
         for(k=0;k<i;k++) if(!deq(a[j][k],0.0)) goto fail;
         if(deq(a[j][i],0.0)) goto fail;
         for(l=i+1;l<m;l++) a[j][l]/=a[j][i];
         a[j][i]=1;
         for(k=0;k<n;k++) if(k!=j) {
            r=a[k][i];
            for(l=i;l<m;l++) a[k][l]-=a[j][l]*r;
            a[k][i]=0;
         }
      fail:;
      }
   }
}

