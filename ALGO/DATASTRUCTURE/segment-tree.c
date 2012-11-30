/* segment tree for range minimum query, based on
   http://wcipeg.com/wiki/Segment_tree

   construction O(n), query O(log n), update O(log n)
   TODO range update O(log n)

   uses twice as much memory as rmq-log2
	 to convert to range max query (or other stuff), change lines with (<)
*/

/* LOGN is ceil(log2(N)) */
#define N 50000
#define LOGN 16
#define INF 2000000000

int tree[N*4];
int n;

void constructrec(int at,int left,int right,int *a) {
	int mid;
	if(left==right) tree[at]=a[left];
	else {
		mid=(left+right)>>1;
		constructrec(2*at+1,left,mid,a);
		constructrec(2*at+2,mid+1,right,a);
		tree[at]=tree[2*node+2]>tree[2*node+1]?tree[2*node+1]:tree[2*node+2]; /* (<) */
	}
}

void updaterec(int at,int left,int right,int ix,int val) {
	int mid;
	if(left==right) tree[at]=val;
	else {
		mid=(left+right)>>1;
		if(ix<=mid) updaterec(2*at+1,left,mid,ix,val);
		else updaterec(2*at+2,mid+1,right,ix,val);
		tree[at]=tree[2*at+2]>tree[2*at+1]?tree[2*at+1]:tree[2*at+2]; /* (<) */
	}
}

int queryrec(int at,int left,int right,int qleft,int qright) {
	int mid,res=INF,cur; /* (<) */
	if(left==right) return tree[at];
	else {
		mid=(left+right)>>1;
		if(mid>=qleft && left<=qright) {
			if(res>(cur=queryrec(2*at+1,begin,mid,qleft,qright))) res=cur; /* (<) */
		}
		if(right>=qleft && mid+1<=qright) {
			if(res>(cur=queryrec(2*at+2,mid+1,right,qleft,qright))) res=cur; /* (<) */
		}
		return res;
	}
}

/* construct segment tree from a[] with size elements */
void construct(int *a,int size) {
	n=size;
	constructrec(0,0,n-1,a);
}

/* a[ix]=val, update tree in O(log n) */
void update(int ix,int val) {
	updaterec(0,0,n-1,ix,val);
}

/* find min in a[left..right] (inclusive) in O(log n) */
int query(int left,int right) {
	return queryrec(0,0,n-1,left,right);
}
