#include <stdio.h>

// algorithm x (dancing links) for finding exact cover.
// supports secondary items that can be covered <=1 times (but not twice),
// items that aren't included in the horizontal doubly linked list (llink/rlink)
// become secondary.
// source: the art of computer programming volume 4b (donald knuth)

// 2022-11-18: langford(n) correct up to 16
// 2022-11-19: n-queens 

#define MAXITEM 1000
#define MAX 10000

int name[MAXITEM];     // optional, name each item
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

// number of items not passed, can be deduced from data structures,
// for example in llink[0]
void algorithmx() {
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

int spacerid;
int nextpos;
int numitems;

// n: total number of items (primary + secondary)
// m: number of secondary items
// primary items appear before secondary ones in the data structures
void inithelp(int n,int m) {
	if(n>=MAXITEM) printf("error, increase MAXITEM and recompile\n");
	numitems=n;
	int p=n-m; // primary items
	for(int i=1;i<=n;i++) len[i]=0;
	for(int i=0;i<=p;i++) llink[i]=(i+p)%(p+1);
	for(int i=0;i<=p;i++) rlink[i]=(i+1)%(p+1);
	for(int i=1;i<=n;i++) dlink[i]=ulink[i]=i;
	// let secondary items form its own doubly linked list
	for(int i=p+1;i<=n;i++) llink[i]=i-1;
	for(int i=p+1;i<=n;i++) rlink[i]=i+1;
	if(n>p) llink[p+1]=n,rlink[n]=p+1;
	nextpos=n+1;
	spacerid=0;
	top[nextpos++]=spacerid--;
}

// add row with m ones, a[i] holds item number (1-indexed)
void addrow(int m,int *a) {
	if(nextpos+m>=MAX) printf("error, increase MAX and recompile\n");
	int lastspacer=nextpos-1;
	for(int i=0;i<m;i++) {
		int ix=a[i];
		if(ix<1 || ix>numitems) printf("illegal item number %d, max is %d\n",ix,numitems);
		len[ix]++;
		top[nextpos]=ix;
		dlink[nextpos]=ix;
		ulink[nextpos]=ulink[ix];
		dlink[ulink[ix]]=nextpos;
		ulink[ix]=nextpos;
		nextpos++;
	}
	// add trailing spacer
	dlink[lastspacer]=nextpos-1;
	ulink[nextpos]=lastspacer+1;
	top[nextpos++]=spacerid--;
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
	inithelp(3*n,0);
	// construct rows
	int row[3];
	for(int i=1;i<=n;i++) for(int j=0;j<2*n-i-1;j++) {
		row[0]=i;
		row[1]=n+j+1;
		row[2]=n+j+i+2;
		addrow(3,row);
	}
	solutions=0;
	algorithmx();
	printf("langford(%d): found %lld solutions\n",n,solutions);
}

void langford_all() {
	for(int i=3;i<=12;i++) if(i%4==0 || i%4==3) langford(i);
}

// find all ways to place n queens on a n*n chessboard
void nqueens(int n) {
	// items: 6n-2 items total, 2n primary, 4n-2 secondary
	// 1 to n: n rows
	// n-1 to 2n: n columns
	// 2n+1 to 4n-1: 2n-1 diagonals from lower left to upper right (secondary)
	// 4n to 6n-2: 2n-1 diagonals from upper left to lower right (secondary)
	inithelp(6*n-2,4*n-2);
	int row[4];
	for(int x=1;x<=n;x++) for(int y=1;y<=n;y++) {
		// option: place queen on (x,y)
		row[0]=y;
		row[1]=n+x;
		row[2]=2*n+x+y-1;
		row[3]=4*n-1+x-y+n;
		addrow(4,row);
	}
	solutions=0;
	algorithmx();
	printf("nqueens(%d): found %lld solutions\n",n,solutions);
}

void nqueens_all() {
	for(int i=1;i<=14;i++) nqueens(i);
}

int main() {
	langford_all();
	nqueens_all();
	return 0;
}
