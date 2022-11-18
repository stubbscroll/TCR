#include <stdio.h>

// algorithm x (dancing links) for finding exact cover
// source: the art of computer programming volume 4b (donald knuth)

// 2022-11-18: langford(n) correct up to 16

#define MAXITEM 1000
#define MAX 10000

int name[MAXITEM];
int llink[MAXITEM];
int rlink[MAXITEM];
int top[MAX],*len=top;
int ulink[MAX];
int dlink[MAX];
int x[MAXITEM];        // solution

void hide(int p) {
	int q=p+1;
	while(q!=p) {
		int x=top[q],u=ulink[q],d=dlink[q];
		if(x<=0) q=u;
		else dlink[u]=d,ulink[d]=u,len[x]--,q++;
	}
}

void cover(int i) {
	int p=dlink[i];
	while(p!=i) hide(p),p=dlink[p];
	int l=llink[i],r=rlink[i];
	rlink[l]=r; llink[r]=l;
}

void unhide(int p) {
	int q=p-1;
	while(q!=p) {
		int x=top[q],u=ulink[q],d=dlink[q];
		if(x<=0) q=d;
		else dlink[u]=q,ulink[d]=q,len[x]++,q--;
	}
}

void uncover(int i) {
	int l=llink[i],r=rlink[i];
	rlink[l]=i; llink[r]=i;
	int p=ulink[i];
	while(p!=i) unhide(p),p=ulink[p];
}

long long solutions;

// a solution was found
// l is number of rows in solution
void visit(int l) {
	// here we can do something with x[0]..x[l-1]
	solutions++;
}

// n: number of items (items are in indexes 1..n, so pass n+1) (except i removed n)
// z: index of last spacer (actually not used currently)
void algorithmx(int z) {
	int l=0;
x2:
	if(rlink[0]==0) {
		visit(l);
		goto x8;
	}
// x3
	// minimum remaining values heuristic
	// traverse the linked list and pick the item with the least len
	int i=rlink[0];
	int best=len[i],bestix=i;
	while((i=rlink[i])!=0) {
		if(best>len[i]) best=len[i],bestix=i;
	}
	i=bestix;
//	printf("level %d, pick item %d\n",l,i);
// x4
	cover(i);
	x[l]=dlink[i];
x5:
	if(x[l]==i) goto x7;
	int p=x[l]+1;
	while(p!=x[l]) {
		int j=top[p];
		if(j<=0) p=ulink[p];
		else cover(j),p++;
	}
	l++;
	goto x2;
x6:
	p=x[l]-1;
	while(p!=x[l]) {
		int j=top[p];
		if(j<=0) p=dlink[p];
		else uncover(j),p--;
	}
	i=top[x[l]]; x[l]=dlink[x[l]];
	goto x5;
x7:
	uncover(i);
x8:
	if(l>0) {
		l--;
		goto x6;
	}
}

// helper functions for building the data structure ///////////////////////////

int lastd[MAXITEM];
int spacerid;
int nextpos;

// n: number of items
void inithelp(int n) {
	for(int i=1;i<=n;i++) len[i]=0;
	for(int i=0;i<=n;i++) llink[i]=(i+n)%(n+1);
	for(int i=0;i<=n;i++) rlink[i]=(i+1)%(n+1);
	for(int i=1;i<=n;i++) lastd[i]=i; // next index for setting dlink for given item
	nextpos=n+1;
	spacerid=0;
	top[nextpos++]=spacerid--;
}

// add row with m ones, a[i] holds item number (1-indexed)
void addrow(int m,int *a) {
	int lastspacer=nextpos-1;
	for(int i=0;i<m;i++) {
		int ix=a[i];
		len[ix]++;
		ulink[nextpos]=lastd[ix];
		dlink[lastd[ix]]=nextpos;
		lastd[ix]=nextpos;
		top[nextpos++]=ix;
	}
	// add trailing spacer
	dlink[lastspacer]=nextpos-1;
	ulink[nextpos]=lastspacer+1;
	top[nextpos++]=spacerid--;
}

// finalize data structure, return index to last spacer
// n: number of items
int finishhelp(int n) {
	for(int i=1;i<=n;i++) {
		ulink[i]=lastd[i];
		dlink[lastd[i]]=i;
	}
	return nextpos;
}

// tests! /////////////////////////////////////////////////////////////////////

// find all langford pairs for given n:
// number of ways to arrange 1, 1, 2, 2, ..., n, n so that there is one number
// between the two 1's, two numbers between the two 2's, ..., n numbers between
// the two n's.
// example: n=3 => 2 3 1 2 1 3
void langford(int n) {
	// we have 3n items to be covered:
	// numbers 1..n and slots 1..2n to be filled with numbers
	inithelp(3*n);
	// construct rows
	int row[3];
	for(int i=1;i<=n;i++) for(int j=0;j<2*n-i-1;j++) {
//		printf("add %d %d %d\n",i,j,j+i+1);
		row[0]=i;
		row[1]=n+j+1;
		row[2]=n+j+i+2;
		addrow(3,row);
	}
	int lastspacer=finishhelp(3*n);
//	for(int i=0;i<=3*n;i++) printf("%d: llink %d, rlink %d, len %d, ulink %d, dlink %d\n",i,llink[i],rlink[i],len[i],ulink[i],dlink[i]);
//	for(int i=0;i<lastspacer;i++) printf("%d: top %d, ulink %d, dlink %d\n",i,top[i],ulink[i],dlink[i]);
	solutions=0;
	algorithmx(lastspacer);
	printf("langford(%d): found %lld solutions\n",n,solutions);
}

void langford_all() {
	for(int i=3;i<=12;i++) if(i%4==0 || i%4==3) langford(i);
}

int main() {
	langford_all();
	return 0;
}
