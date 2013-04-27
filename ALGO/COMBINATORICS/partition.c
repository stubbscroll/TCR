#include <stdio.h>

/*  TODO rewrite this for long long */

/*mpz_t *table;
long long maxindex;
*/
/*  a(n) = a(n-1)+a(n-2)-a(n-5)-a(n-7)+a(n-12)+a(n-15)-...
    where i in n-i is from the sequence n(3n-1)/2 with n=+-1, +-2, +-3 ...
    source: third formula at http://oeis.org/A000041
    time complexity: O(n^1.5) */
/*
int A000041_init(long long max){
  if(max>=MAX || max<1) return 0;
  if(0==(table=malloc(sizeof(mpz_t)*((int)max+1)))) return 0;
  maxindex=max+1;
  mpz_init_set_si(table[0],1);  
  for(int i=1;i<=max;i++) {
    mpz_init_set_si(table[i],0);
    for(long long j=1,k;;j++) {
      k=i-j*(3*j-1)/2;
      if(k<0) break;
      if(j&1) mpz_add(table[i],table[i],table[k]);
      else mpz_sub(table[i],table[i],table[k]);
      k=i-j*(3*j+1)/2;
      if(k<0) break;
      if(j&1) mpz_add(table[i],table[i],table[k]);
      else mpz_sub(table[i],table[i],table[k]);
    }
  }
  return 1;
}

*/

/* unordered set partition! division of n elements into sets.
   represented by int* where different int means different sets.
   example: all 15 ways to split 4 elements into sets
   0000 0001 0010 0011 0012 0100 0101 0102 0110 0111 0112 0120 0121 0122 0123
*/

#define MAXU 128
int unorder[MAXU][MAXU];

/* precalculate helper array */
/* TODO untested */
void generateunorder() {
	int i,j;
	for(i=0;i<MAXU;i++) unorder[0][i]=1;
	for(i=1;i<MAXU;i++) for(j=0;j<MAXU;j++)
		unorder[i][j]=j*unorder[i-1][j]+unorder[i-1][j+1];
}

/* return rank of f[0], ..., f[n-1] */
/* requires that generateunorder() has been called */
/* TODO untested */
int rankunorder(int n,int *f) {
	int r=0,j=0,i;
	for(i=1;i<n;i++) {
		r+=f[i]*unorder[n-i-1][j+1];
		if(j<f[i]) j=f[i];
	}
	return r;
}

/* return unordered set in f, given rank r and size n */
/* requires that generateunorder() has been called */
/* TODO untested */
void unrankunorder(int n,int r,int *f) {
	int i,j=0;
	f[0]=0;
	for(i=1;i<n;i++) if((j+1)*unorder[n-i-1][j+1]<=r) {
		f[i]=j+1;
		r-=(j+1)*unorder[n-i-1][j+1];
		j++;
	} else {
		f[i]=r/unorder[n-i-1][j+1];
		r%=unorder[n-i-1][j+1];
	}
}

/* TODO nextunorderedsetpartition */

int main() {
	int i,j,k;
	int a[100];
	int o[]={0,1,2,5,15,52,203,877,4140,21147};
	generateunorder();
	for(i=1;i<10;i++) {
		for(j=0;j<o[i];j++) {
			unrankunorder(i,j,a);
			k=rankunorder(i,a);
			if(k!=j) printf("error %d %d!=%d\n",i,j,k);
			printf("%d %d:",i,j);
			for(k=0;k<i;k++) printf(" %d",a[k]);
			printf("\n");
		}
	}
	return 0;
}
