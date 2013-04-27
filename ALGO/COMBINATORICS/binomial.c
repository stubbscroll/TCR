typedef long long ll;
typedef unsigned long long ull;

#define MAXP 200
ll pascal[MAXP][MAXP];

void genpascal() {
	int i,j;
	for(i=0;i<MAXP;i++) {
		pascal[i][0]=pascal[i][i]=1;
		for(j=1;j<i;j++) pascal[i][j]=pascal[i-1][j]+pascal[i-1][j-1];
	}
}

/* evaluate the multinomial coefficient
   ( a[0] a[1] ... a[n-1] )
   ( a[0]+a[1]+...+a[n-1] ) */
ull multinomial(int *a,int n) {
	ull res=1;
	int upper=a[0],lower=0,i;
	for(i=1;i<n;i++) upper+=a[i],lower+=a[i-1],res*=pascal[upper][lower];
	return res;
}

/* evaluate n choose k modulo 2. warning, needs 0<=k<=n */
int nck2(int n,int k) {
	return (k&(~n))==0;
}
