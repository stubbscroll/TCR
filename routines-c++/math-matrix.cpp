//TODO: calculate inverse
//TODO: calculate determinant
//TODO: calculate permanent
//TODO: gausselim

//=============== matrix routines ====================

// 15.03.2007, tested on srm 306 div 1 hard, ok

/* identity matrix (int) for size n */

vvi matrixid(int n){vvi a(n,vi(n,0));fir(n)a[i][i]=1;return a;}

// 15.03.2007, tested on srm 306 div 1 hard, ok

/* add two matrices of integers with modulo
   handles overflow
   for mod<2^30, remove (ll) */

vvi matrixaddmod(vvi &a,vvi &b,int mod)
{
   int n=a.si,m=a[0].si;
   vvi x(n,vi(m,0));
   fir(n) fjr(m) x[i][j]=((ll)a[i][j]+b[i][j])%mod;
   return x;
}

// 15.03.2007, tested on srm 306 div 1 hard, ok

/* multiply two matrices of integers, multiply with modulo
   handles overflow
   for mod<46341, remove (ll) */

vvi matrixmulmod(vvi &a,vvi &b,int mod)
{
   int n=a.si,m=a[0].si,o=b[0].si;
   if(m!=b.si) return vvi(0);
   vvi x(n,vi(o,0));
   fir(n) fjr(o) fkr(m) x[i][j]=(x[i][j]+(ll)a[i][k]*b[k][j])%mod;
   return x;
}

// 15.03.2007, tested on srm 306 div 1 hard, ok

/* calculate a^n with modulo */

vvi matrixpowmod(vvi &a,int n,int mod)
{
   if(!n) return matrixid(a.si);
   vvi b=a,r=matrixid(a.si);
   while(1) {
      if(n&1) r=matrixmulmod(r,b,mod);
      n>>=1;
      if(!n) break;
      b=matrixmulmod(b,b,mod);
   }
   return r;
}

// 15.03.2007, tested on srm 306 div 1 hard, ok (worst case 989 ms)

/* return matrix sum a^1 + a^2 + a^3 + ... + a^n with modulo */

vvi matrixaccummod(vvi &a,int n,int mod)
{
   if(n==0) return vvi(a.si,vi(a.si,0));
   else if(n&1) {
      vvi t1=matrixpowmod(a,n,mod);
      vvi t2=matrixaccummod(a,n-1,mod);
      return matrixaddmod(t1,t2,mod);
   } else {
      vvi t1=matrixaccummod(a,n/2,mod);
      vvi t2=matrixid(a.si);
      vvi t3=matrixpowmod(a,n/2,mod);
      vvi t4=matrixaddmod(t3,t2,mod);
      return matrixmulmod(t1,t4,mod);
   }
}

void printmatrix(vvi x)
{
   fir(x.si) fjr(x[0].si) printf("%d%c",x[i][j],j==x[0].si-1?'\n':' ');printf("\n");
}

