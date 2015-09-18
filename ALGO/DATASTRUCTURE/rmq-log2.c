/* range minimum query with binary trees
   construction time O(n), query time O(log n), update O(log n),
   memory complexity O(n).
   return index of lowest value in a range. if there is a tie, one of the
   lowest indices are returned (not necessarily the earliest).
   if only value is needed, change construct() to store value instead.
   for range max query, reverse inequalities on lines marked with (<)
   for other operations, replace all lines marked with (<) as such:
   - in construct/update: if(..>..) xx else xx
     => rmq[j][i] = f(rmq[j-1][i*2], rmq[j-1][i*2+1])
   - in query: if(min > ...) min = ...
     => min = f(min, rmq[j][start or end])
   - also, in query: replace min=start by min=identity
   where f is any commutative operator with identity. for example,
   sum (identity=0), product (identity=1), gcd (identity=0, but make sure to handle
   gcd(0,0)), min (identity=INF), max (identity=-INF) etc */

/* LOGN is ceil(log2(N)) */
#define N 50000
#define LOGN 16

/* rmq[j][i] stores the minimum for range i*2^j to (i+1)*2^j-1, inclusive */
int *rmq[LOGN+1];
int rmq2[2*N];

/* a is the array we want to construct from */
/* if you need sentinels etc (when doing interval compression), make sure to
   include them in the array (within n) */
/* OK POJ 2637 (NCPC 2005 F "worst weather ever") (min), n<=50000 q<=10000, 219 ms, 2012-10-02 */
/* OK Timus 1846 (gcd), n<=100000, q<=100000, 0.171 s, 2012-11-18 */
/* OK Project Euler 485 (max), n<=100000000, 13.5 s, 2015-02-21 */
void construct(int *a,int n) {
	int i,j,p;
	for(j=n,p=i=0;j>1;i++,p+=j,j>>=1) rmq[i]=rmq2+p;
	rmq[i]=rmq2+p;
	for(i=0;i<n;i++) rmq[0][i]=i;
	for(j=1;(1<<j)<=n;j++) for(i=0;((i+1)<<j)<=n;i++) {
		if(a[rmq[j-1][i*2]]>a[rmq[j-1][i*2+1]]) rmq[j][i]=rmq[j-1][i*2+1]; /* (<) */
		else rmq[j][i]=rmq[j-1][i*2];
	}
}

/* return index of lowest value between start and end, inclusive.
   *a is the array with values, rmq contains index to smallest element */
/* it is the caller's responsibility to avoid querying empty ranges
   (start>end) */
/* OK POJ 2637 (NCPC 2005 F "worst weather ever") (min), n<=50000 q<=10000, 219 ms, 02.10.2012 */
/* OK Timus 1846 (gcd), n<=100000, q<=100000, 0.171 s, 18.11.2012 */
/* OK Project Euler 485 (max), n<=100000000, 13.5 s, 2015-02-21 */
int query(int *a,int start,int end) {
	int j,min=start;
	for(end++,j=0;start<end;j++,start>>=1,end>>=1) {
 		if(start&1) {
			if(a[min]>a[rmq[j][start]]) min=rmq[j][start]; /* (<) */
			start++;
		}
		if(end&1) {
			end--;
			if(a[min]>a[rmq[j][end]]) min=rmq[j][end]; /* (<) */
		}
	}
	return min;
}

/* change a[ix] to val, update all subintervals in O(log n) */
/* OK Timus 1846 (gcd), n<=100000, q<=100000, 0.171 s, 18.11.2012 */
void update(int *a,int n,int ix,int val) {
	int j,start=ix>>1;
	if(a[ix]==val) return;
	a[ix]=val;
	for(j=1;((start+1)<<j)<=n;j++,start>>=1) {	
		if(a[rmq[j-1][start*2]]>a[rmq[j-1][start*2+1]]) rmq[j][start]=rmq[j-1][start*2+1]; /* (<) */
		else rmq[j][start]=rmq[j-1][start*2];
	}
}

/* old, slower routine for updating, needs O(log^2 n),
   kept for completeness, historical reasons etc */
/* WARNING, not tested in competition code */
void update_old(int *a,int n,int ix,int val) {
	int j,start=ix>>1,end=(ix+1)>>1,iy,iz;
	if(a[ix]==val) return;
	a[ix]=val;
	if(start==end) end++;
	for(j=1;j<LOGN && (end<<j)<=n;j++) {
		iy=(ix-(start<<j)>0)?query(a,start<<j,ix-1):ix;
		if(a[iy]>a[ix]) iy=ix; /* (<) */
		iz=((end<<j)-ix>1)?query(a,ix+1,(end<<j)-1):ix;
		if(a[iy]>a[iz]) iy=iz; /* (<) */
		rmq[j][start]=iy;
		start>>=1;
		end>>=1;
		if(start==end) end++;
	}
}
