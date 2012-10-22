/* range minimum query:
   construction time O(n log n), query time	O(1),
   memory complexity O(n log n).
	 unfortunately, the query time is really O(log n) since we need to find
	 the largest power of two not exceeding end-start+1. TODO consider
	 wasting O(n) memory precalculating this.
	 return index of lowest value in a range.
	 if only value is needed, change construct to store value instead.
	 this is an easier implementation that doesn't allow dynamic updates
	 in an efficient way.
   for range max query, reverse inequalities on lines marked with (<) */

/* LOGN is ceil(log2(N)) */
#define N 50000
#define LOGN 16

/* rmq[i][j] stores index to minimum for indexes i to i+2^j-1, inclusive */
int rmq[N][LOGN];

/* a is the array we want to construct from */
/* if a needs a sentinel value at the end, make sure n includes this! */
/* OK POJ 2637 (NCPC 2005 F "worst weather ever"), n<=50000 q<=10000, 313 ms, 02.10.2012 */
void construct(int *a,int n) {
	int i,j,k;
	for(i=0;i<n;i++) rmq[i][0]=i;
	for(j=k=1;k<=n;j++,k<<=1) for(i=0;i+k+k<n;i++) {
		/* construct rmq[i][j] from min of rmq[i][j-1] and rmq[i+k][j-1] */
		if(a[rmq[i][j-1]]<a[rmq[i+k][j-1]]) rmq[i][j]=rmq[i][j-1]; /* (<) */
		else rmq[i][j]=rmq[i+k][j-1];
	}
}

/* return index of lowest value between start and end, inclusive */
/* it is the caller's responsibility to avoid querying empty ranges
   (start>end) */
/* OK POJ 2637 (NCPC 2005 F "worst weather ever"), n<=50000 q<=10000, 313 ms, 02.10.2012 */
int query(int *a,int start,int end) {
	int w=end-start+1,t;
	/* find largest power of 2 which doesn't exceed w */
	for(t=0;(1<<(t+1))<=w;t++);
	if(a[rmq[start][t]]<a[rmq[end-(1<<t)+1][t]]) return rmq[start][t]; /* (<) */
	else return rmq[end-(1<<t)+1][t];
}
