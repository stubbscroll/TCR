/* range minimum query:
   construction time O(n), query time O(log n), update O(log^2 n),
   memory complexity O(n).
	 return index of lowest value in a range. if there is a tie, one of the
   lowest indices are returned (not necessarily the earlist).
   if only value is needed, change construct to store value instead.
   for range max query, reverse inequalities on lines marked with (<) */

/* LOGN is ceil(log2(N)) */
#define N 50000
#define LOGN 16

/* rmq[j][i] stores the minimum for range i*2^j to (i+1)*2^j-1, inclusive */
int *rmq[LOGN+1];
int rmq2[2*N];

/* a is the array we want to construct from */
/* if a needs a sentinel value at the end (when using sweepline),
   make sure n includes this! */
/* OK POJ 2637 (NCPC 2005 F "worst weather ever"), n<=50000 q<=10000, 219 ms, 02.10.2012 */
void construct(int *a,int n) {
	int i,j,p;
	for(j=n,p=i=0;j>1;i++,p+=j,j>>=1) rmq[i]=rmq2+p;
	rmq[i]=rmq2+p;
	for(i=0;i<n;i++) rmq[0][i]=i;
	for(j=1;(1<<(j+1))-1<=n;j++) for(i=0;(i<<j)<n;i++) {
		if(a[rmq[j-1][i*2]]>a[rmq[j-1][i*2+1]]) rmq[j][i]=rmq[j-1][i*2+1]; /* (<) */
		else rmq[j][i]=rmq[j-1][i*2];
	}
}

/* return index of lowest value between start and end, inclusive.
   *a is the array with values, rmq contains index to smallest element */
/* it is the caller's responsibility to avoid querying empty ranges
   (start>end) */
/* OK POJ 2637 (NCPC 2005 F "worst weather ever"), n<=50000 q<=10000, 219 ms, 02.10.2012 */
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

/* change a[ix] to val, need to update every subinterval containing ix */
/* WARNING, not tested in competition code */
void update(int *a,int n,int ix,int val) {
	int j,start=ix>>1,end=(ix+1)>>1,iy,iz;
	if(a[ix]==val) return;
	a[ix]=val;
	if(start==end) end++;
	for(j=1;j<LOGN && (end<<j)<=n;j++) {
		iy=(ix-(start<<j)>0)?query(a,start<<j,ix-1):ix;
		if(a[ix]<a[iy]) iy=ix; /* (<) */
		iz=((end<<j)-ix>1)?query(a,ix+1,(end<<j)-1):ix;
		if(a[iz]<a[iy]) iy=iz; /* (<) */
		rmq[j][start]=iy;
		start>>=1;
		end>>=1;
		if(start==end) end++;
	}
}

/* reksten's version here, it actually gives wrong answer on POJ 2637.
   maybe my contruction is incompatible. */
/* TODO try to understand reksten's rmq-query, it is shorter and neater */
int magic(int *a,int start,int end) {
	int min=start,i,j;
	end++;
	while(start<end) {
		for(i=0,j=start;start+(1<<(i+1))<end && (j&1)==0;j>>=1,i++);
		min=min>a[rmq[i][j]]?a[rmq[i][j]]:min;
		start+=1<<i;
	}
	return min;
}
