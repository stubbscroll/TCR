#include <stdio.h>
#include <math.h>
typedef long long ll;
typedef unsigned long long ull;

/*  continued fractions! */

/*  convert sqrt to continued fraction */
/*  OK Project Euler 064 04.08.2011 */
void sqrtperiod(ll num,int *a,int maxa,int *n) {
  ll p=0,q=1;
  a[0]=(int)(sqrt(num)+1e-7);
  if(num==a[0]*a[0]) { *n=1; return; }
  for(*n=1;*n<maxa;(*n)++) {
    p=a[*n-1]*q-p;
    q=(num-p*p)/q;
    a[*n]=(a[0]+p)/q;
    if(a[*n]==a[0]+a[0]) { (*n)++; return; }
  }
}

/*  convert continued fraction to fraction!
    input: a, array of size max with cfrac terms
    n: how many terms to take (can be larger than max, will cycle)
    result is p/q */
/*  NB, not tested */
void cfractofrac(int *a,int max,int n,ull *p,ull *q) {
  ull t;
  if(n--==1) { *p=a[0]; *q=1; return; }
  *p=a[!n?0:(n-1)%(max-1)+1]; *q=1;
  for(n--;n>-1;n--) {
    t=*p;
    *p=*p*a[!n?0:(n-1)%(max-1)+1]+*q;
    *q=t;
  }
}

int main() {
  int i,a[1000],n,ans;
  ull teller,nevner;
  for(ans=0,i=2;i<10000;i++) {
    sqrtperiod(i,a,1000,&n);
    if(n>1 && !(n&1)) ans++;
  }
  printf("Project Euler 064: %d\n",ans);
  return 0;
}
