#include <stdio.h>
#include <string.h>

typedef long long ll;
typedef unsigned long long ull;

ll intpow(ll a,int n) {
  ll ans=1;
  while(n--) ans*=a;
  return ans;
}

/*  factorial */
#define MAXFAC 21
ll fac[MAXFAC];
void genfac() {
  int i;
  fac[0]=1;
  for(i=1;i<MAXFAC;i++) fac[i]=fac[i-1]*i;
}

/*  pascal's triangle
    (n k) = number of ways to pick k things among n things */
#define MAXPASCAL 100
ll pascal[MAXPASCAL][MAXPASCAL];
void genpascal() {
  int i,j;
  for(i=0;i<MAXPASCAL;i++) {
    pascal[i][0]=pascal[i][i]=1;
    for(j=1;j<i;j++) pascal[i][j]=pascal[i-1][j-1]+pascal[i-1][j];
  }
}

/*  stirling numbers of the second kind:
    {n k} = number of ways to partition n things into k nonempty subsets */
#define MAXSTIRLINGSET 100
ll stirlingset[MAXSTIRLINGSET][MAXSTIRLINGSET];
void genstirlingset() {
  int i,j;
  memset(stirlingset,0,sizeof(stirlingset));
  for(i=1;i<MAXSTIRLINGSET;i++) {
    stirlingset[i][0]=0;
    stirlingset[i][1]=1;
    for(j=2;j<=i;j++) stirlingset[i][j]=j*stirlingset[i-1][j]+stirlingset[i-1][j-1];
  }
  stirlingset[0][0]=1;
}

void testpascal() {
  int i,j;
  genpascal();
  puts("pascal's triangle:");
  for(i=0;i<16;i++) {
    for(j=0;j<=i;j++) printf("%4I64d ",pascal[i][j]);
    putchar('\n');
  }
}

void teststirlingset() {
  int i,j;
  genstirlingset();
  puts("stirling's numbers of the second kind (set partitions):");
  for(i=0;i<11;i++) {
    for(j=0;j<=i;j++) printf("%5I64d ",stirlingset[i][j]);
    putchar('\n');
  }
  putchar('\n');
}

int main() {
  testpascal();
  teststirlingset();
  
  return 0;
}
