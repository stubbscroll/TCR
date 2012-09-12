#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef long long ll;

/*  rank/unrank of permutations with no duplicates!
    O(n) algorithm due to Myrvold, Ruskey: "Ranking and unranking permutations
    in linear time" (2000)
    NB, ranking is not lexicographic! */
/*  max n=20, since 21! overflows long long */

/*  OK UVa 10181 17.08.2011 */

ll fact[21];

/*  must be called before using rank/unrank */
void initfact() {
  int i;
  fact[0]=1;
  for(i=1;i<21;i++) fact[i]=fact[i-1]*i;
}

/*  given rank, return permutation
    n: number of elements
    r: number (rank) to convert to permutation
    *p: permutation to be generated */
void permutationunrank2(int n,ll r,int *p) {
  int i,s,t;
  for(i=0;i<n;i++) p[i]=i;
  while(n) {
    s=r%n;
    t=p[n-1],p[n-1]=p[s],p[s]=t;
    r/=n;
    n--;
  }
}

ll permbtr(int n,int *p,int *q) {
  int s,t;
  if(n<2) return 0;
  s=p[n-1]; p[n-1]=p[q[n-1]]; p[q[n-1]]=s;
  t=q[s];   q[s]=q[n-1];      q[n-1]=t;
  return s+n*permbtr(n-1,p,q);
}

/*  return rank of permutation
    n: number of elements
    *p: permutation to rank */
/*  warning, *p is destroyed! */
ll permutationrank2(int n,int *p) {
  int i,q[16];
  for(i=0;i<n;i++) q[p[i]]=i;
  return permbtr(n,p,q);
}

/*  generate the next permutation, or return 0 if there is no
    lexicographically next permutation.
    destroys the incoming string.
    works with duplicates as well.  */
/*  datatype can be char *, int * etc */
/* OK UVA 12079 (NWERC 2005 problem D), n<=9, 08.09.2012 */
int nextpermutation(char *a,int n) {
  int j,k,i;
  char t;  /*  datatype of permutation */
  for(j=n-2;j>-1 && a[j]>=a[j+1];j--);
  if(j<0) return 0;
  for(i=n-1;a[j]>=a[i];i--);
  t=a[j]; a[j]=a[i]; a[i]=t;
  for(k=j+1,i=n-1;k<i;k++,i--) t=a[k], a[k]=a[i], a[i]=t;
  return 1;
}

/*  faster generating of all permutations (not lexicographic).
    permutations need not be sorted before call.
    string can have no duplicates. */
/*  WARNING, not tested */
void generatepermutations(char *a,int n) {
  int o[100],c[100],j,s,q;
  char t;
  for(j=0;j<n;j++) c[j]=0,o[j]=1;
p2:
  /*  visit permutation here */
  puts(a);
  /*  end of visit permutation */
  j=n; s=0;
p4:
  q=c[j-1]+o[j-1];
  if(q<0) goto p7;
  if(q==j) goto p6;
  t=a[j-c[j-1]+s-1]; a[j-c[j-1]+s-1]=a[j-q+s-1]; a[j-q+s-1]=t;
  c[j-1]=q;
  goto p2;
p6:
  if(j==1) return;
  s++;
p7:
  o[j-1]=-o[j-1]; j--;
  goto p4;
}

/*  TODO add lexicographic permutation rank/unrank */

/*  permutation rank with duplicates!
    both rank and unrank runs in O(n^3) time (unfortunately).
    permutation lengths up to MAXP are supported, as long as the
    rank is less than 2^64 (unsigned long long).
*/

typedef unsigned long long ull;

#define MAXP 80

/* precalculated pascal's triangle */
ull pascal[MAXP][MAXP];

/*  must call this before evamultinomial and rank/unrank */
void precalculatepascal() {
  int i,j;
  for(i=0;i<MAXP;i++) {
    pascal[i][0]=pascal[i][i]=1;
    for(j=1;j<i;j++) pascal[i][j]=pascal[i-1][j-1]+pascal[i-1][j];
  }
}

/*  evaluate multinomial coefficient (without division)
    based on knuth 1.2.6 equation 4.3 */
/*  HNU 11763 (BAPC 2009 problem D) 0 ms, 18.08.2001 */
ull evalmultinomial(int *c,int n) {
  ull r=1;
  int a=c[0],b=0,i;
  for(i=1;i<n;i++) a+=c[i],b+=c[i-1],r*=pascal[a][b];
  return r;
}

/*  OK UVa 12335 29.10.2011 */
void permutationunrank(ull rank,int *distr,int m,int *seq) {
  ull run,next;
  int tot=0,i,j,left[MAXP];
  for(i=0;i<m;i++) tot+=distr[i],left[i]=distr[i];
  for(i=0;i<tot;i++) {
    /* for each position in the resulting permutation:
       find the element here by calculating the number of permutations
       of the remaining values */
    run=0;
    for(j=0;j<m;j++) if(left[j]) {
      left[j]--;
      next=run+evalmultinomial(left,m);
      if(next>rank) {
        seq[i]=j;
        rank-=run;
        break;
      }
      left[j]++;
      run=next;
    }
  }
}

/*  given permutation and distribution of letters, return rank
    seq:  sequence (permutation) to rank, has n elements
    distr; distr[i] is the number of element i, m different elements */
/*  (distr can be calculated from seq, but with multiple calls to
     permutationrank() this is inefficient.)*/
/*  HNU 11763 (BAPC 2009 problem D) 0 ms, 18.08.2001 */
ull permutationrank(int *seq,int n,int *distr,int m) {
  ull r=0;
  int left[MAXP],i,j;
  for(i=0;i<m;i++) left[i]=distr[i];
  for(i=0;i<n;i++) {
    for(j=0;j<seq[i];j++) if(left[j]) {
      left[j]--;
      r+=evalmultinomial(left,m);
      left[j]++;
    }
    left[seq[i]]--;
  }
  return r;
}

int main() {
  char s[]="abcde";
  int n=strlen(s);
  do {
    puts(s);
  } while(nextpermutation(s,n));
  strcpy(s,"1234");
  n=strlen(s);
  generatepermutations(s,n);
  return 0;
}
