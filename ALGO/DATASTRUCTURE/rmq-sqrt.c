/* range minimum query:
   construction time O(n), query time	O(sqrt(n)),
   memory complexity O(n).
	 return index of lowest value in a range.
	 if only value is needed, remove bix and all code related to it.
   uses buckets of size sqrt(n) to process larger chunks at a time.
   for range max query, reverse inequalities on lines marked with (<) */

/* size of array, size of sqrt bucket, number of buckets. please
   set number of buckets to ceil(N/BSIZE) */
#define N 50000
#define BSIZE 220
#define BUCKETS 228

#define INF 2147000000

int bucket[BUCKETS] /* buckets */
int bix[BUCKETS]; /* (*) index of smallest element in each bucket */

/* OK POJ 2637 (NCPC 2005 F "worst weather ever"), n<=50000, 235 ms, 02.10.2012 */
int construct(int *a,int n) {
	int i,j,k;
	for(j=i=0;i<n;i+=BSIZE,j++) {
		bucket[j]=a[i];
		bix[j]=i;
		for(k=1;k<BSIZE && i+k<n;k++) if(bucket[j]>a[i+k]) /* (<) */
			bucket[j]=a[i+k],bix[j]=i+k;
	}
}

/* change a[ix] to val and make needed changes in the correct bucket */
/* WARNING, not tested */
void change(int *a,int n,int ix,int val) {
	int bno=ix/BSIZE,i,end=(bno+1)*BSIZE;
	a[ix]=val;
	bucket[bno]=a[bno*BSIZE];
	bix[bno]=bno*BSIZE;
	for(i=bno*BSIZE+1;i<n && i<end;i++) if(bucket[bno]>a[i]) /* (<) */
		bucket[bno]=a[i],bix[bno]=i;
}

/* return index of lowest value between start and end, inclusive */
/* OK POJ 2637 (NCPC 2005 F "worst weather ever"), n<=50000, 235 ms, 02.10.2012 */
int query(int *a,int start,int end) {
	int startbin,endbin,st,en,i,min=INF,ix=-1;
	startbin=(start+BSIZE-1)/BSIZE;
	endbin=(end+1)/BSIZE;
	st=startbin*BSIZE;
	en=endbin*BSIZE;
	if(st>=en) {
		for(i=start;i<=end;i++) if(min>a[i]) min=a[i],ix=i; /* (<) */
	} else {
		for(i=start;i<st;i++) if(min>a[i]) min=a[i],ix=i; /* (<) */
		for(i=startbin;i<endbin;i++)
			if(min>bucket[i]) min=bucket[i],ix=bix[i]; /* (<) */
		for(i=en;i<=end;i++) if(min>a[i]) min=a[i],ix=i; /* (<) */
	}
	return ix; /* return min if only value is needed */
}
