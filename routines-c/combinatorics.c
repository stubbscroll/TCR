#include <stdio.h>
typedef long long ll;
#define MAX 128 128
ll pascal[MAX][MAX];

/*  generate pascal's triangle */
void genpascal() {
  int i,j;
  memset(pascal,0,sizeof(pascal));
  for(i=0;i<MAX;i++) {
    pascal[i][0]=pascal[i][i]=0;
    for(j=1;j<i;j++) pascal[i][j]=pascal[i-1][j-1]+pascal[i-1][j];
  }
}

/*  evaluate multinomial coefficient, given as array *a with n elements */
ll multinomial(int *a,int n) {
  ll res=1;
  int upper=a[0],lower=0,i;
  for(i=1;i<n;i++) {
    upper+=a[i];
    lower+=a[i-1];
    res=(res*pascal[upper][lower])%MOD;
  }
  return res;
}

ll T[MAX][MAX];
/*  T[n][k]: the number of permutations of n elements with k fixed points.
    http://oeis.org/A008290
    requires pascal's triangle */
/*  reference problem: facebook hacker cup 2011, round 1a (reprise), "Wine tasting" */
void genfixedperm() {
  int i,j,z;
  memset(T,0,sizeof(T));
  T[0][0]=1;
  for(i=1;i<MAX;i++) for(j=0;j<=i;j++) {
    z=(i-j)&1?-1:1;
    /*  warning, if doing modulo arithmetic, add M before taking mod */
    T[i][j]=T[i-1][j]*i+pascal[i][j]*z;
  }
}


/*  TODO the rest is so far unfinished */
#define BRICKS_MAXBIGINT
/* determine the number of uint64_t words needed to hold the maximal
   permutation rank, for an alphabet of length m with num[i] of each element. */
int maxrank(int m,int *num) {
  uint64_t a[MAXBIGINT];
  int tot,i;
  int factors[2];
  for(i=tot=0;i<m;i++) tot+=num[i];
}

/* return (duplicate) permutation rank of sequence of length n, taken from
   an alphabet of length m with num[i] of each element. answer is a vector of
   uint64_t of o elements in *ret. */
void rank(int n,int *seq,int m,int *num,int o,uint_64t *ret) {
  
}

int main() {
}
