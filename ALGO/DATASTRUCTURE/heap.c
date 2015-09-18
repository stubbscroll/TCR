#include <stdio.h>

/* min-heap! warning, 1-indexed, index 0 is unused */
/* heapn starts at 1 (number of elements is 0, +1) */

/* OK UVa 851 17.08.2011 */
/* OK UVa 929 16.08.2011 */
/* OK UVa 10181 17.08.2011 */
/* OK UVa 10986 16.08.2011 */
/* OK UVa 11995 n<=1000 2013-06-12 */
/* OK UVa-live 4374 (NEERC 2008 problem D) 16.08.2011 */
/* OK SPOJ 3465 DRIVE (NEERC 2008 problem D) 16.08.2011 */
/* OK TJU 3132 (NEERC 2008 problem D) 16.08.2011 */
/* OK UVa 11367 (NCPC 2007 problem F) 29.09.2011 */
/* OK NCPC 2011 problem F 01.10.2011 */

#define MAXH 1000000

int heap[MAXH];   /* heap value (on which it is sorted)  */
int heapid[MAXH]; /* heap id */
int heapn;        /* (number of elements in heap)+1 */

/* insert a new value to the heap */
/* warning, function will choke if heap is full */
void heapinsert(int val,int id) {
	int pos=heapn++;
	/* change to val> for maxheap */
	while(pos>1 && val<heap[pos>>1]) {
		heap[pos]=heap[pos>>1];
		heapid[pos]=heapid[pos>>1];
		pos>>=1;
	}
	heap[pos]=val; heapid[pos]=id;
}

/* remove a value from the heap and put it in *val,*id */
/* don't call if heap is empty! */
void heapremove(int *val,int *id) {
	int pos=1,next=1,newval;
	int newid;
	*val=heap[1]; *id=heapid[1];
	newval=heap[--heapn];
	newid=heapid[heapn];
	while((pos<<1)<=heapn) {
		/* take lowest of left and right children */
		if((pos<<1)==heapn) next=pos<<1;
		/* change to <heap[...] for maxheap */
		else next=(pos<<1)+(heap[pos<<1]>heap[(pos<<1)+1]);
		/* change to <=newval for maxheap */
		if(heap[next]>=newval) break;
		heap[pos]=heap[next];
		heapid[pos]=heapid[next];
		pos=next;
	}
	heap[pos]=newval;
	heapid[pos]=newid;
}
