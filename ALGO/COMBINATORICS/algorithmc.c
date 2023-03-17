#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// algorithm c: exact covering with colours, extends algorithm x
// secondary items (and only them!) can now have colours, and it's possible to
// choose two options having the same secondary item, as long as they have the
// same colour
// 0=no colour, positive integer=given colour, -1=internally used to
// represent that a colour has been assigned to an item

// add some interesting examples (better than the word squares), for example
// the 5x5 number square thingy with prime numbers
// add masyu and slitherlink and others to whichever variant of the algorithm
// they can be solved by (seems like a modification of algorithm c is needed)

#define MAXITEM 1050
#define MAX 40000

int name[MAXITEM];     // optional, name each item
int llink[MAXITEM];
int rlink[MAXITEM];
int top[MAX],*len=top;
int ulink[MAX];
int dlink[MAX];
int colour[MAX];
int x[MAXITEM];        // solution

void printstate();

void hide(int p) {
	int q=p+1;
	while(q!=p) {
		int x=top[q],u=ulink[q],d=dlink[q];
		if(x<=0) q=u;
		else {
			if(colour[q]>=0) dlink[u]=d,ulink[d]=u,len[x]--;
			q++;
		}
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
		else {
			if(colour[q]>=0) dlink[u]=q,ulink[d]=q,len[x]++;
			q--;
		}
	}
}

void uncover(int i) {
	int l=llink[i],r=rlink[i];
	rlink[l]=i; llink[r]=i;
	int p=ulink[i];
	while(p!=i) unhide(p),p=ulink[p];
}

void purify(int p) {
	int c=colour[p],i=top[p],q=dlink[i];
	colour[i]=c; 
	while(q!=i) {
		if(colour[q]==c) colour[q]=-1;
		else hide(q);
		q=dlink[q];
	}
}

void unpurify(int p) {
	int c=colour[p],i=top[p],q=ulink[i];
	while(q!=i) {
		if(colour[q]<0) colour[q]=c;
		else unhide(q);
		q=ulink[q];
	}
}

void commit(int p,int j) {
	if(colour[p]==0) cover(j);
	else if(colour[p]>0) purify(p);
}

void uncommit(int p,int j) {
	if(colour[p]==0) uncover(j);
	else if(colour[p]>0) unpurify(p);
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
void algorithmc() {
	int l=0;
c2:
	if(rlink[0]==0) {
		visit(l);
		goto c8;
	}
// c3
	// minimum remaining values heuristic
	// traverse the linked list and pick the item with the least len
	int i=rlink[0];
	int best=len[i],bestix=i;
	while((i=rlink[i])!=0) {
		if(best>len[i]) best=len[i],bestix=i;
	}
	i=bestix;
// c4
	cover(i);
	x[l]=dlink[i];
c5:
	if(x[l]==i) goto c7;             
	int p=x[l]+1;
	while(p!=x[l]) {
		int j=top[p];
		if(j<=0) p=ulink[p];
		else commit(p,j),p++;
	}
	l++;
	if(l==MAXITEM) printf("sanity error, l grew too large\n");
	goto c2;
c6:
	p=x[l]-1;
	while(p!=x[l]) {
		int j=top[p];
		if(j<=0) p=dlink[p];
		else uncommit(p,j),p--;
	}
	i=top[x[l]]; x[l]=dlink[x[l]];
	goto c5;
c7:
	uncover(i);
c8:
	if(l>0) {
		l--;
		goto c6;
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
	if(n>=MAXITEM) printf("error, increase MAXITEM and recompile\n"),exit(0);
	numitems=n;
	int p=n-m; // primary items
	for(int i=1;i<=n;i++) len[i]=0;
	for(int i=0;i<=p;i++) llink[i]=(i+p)%(p+1);
	for(int i=0;i<=p;i++) rlink[i]=(i+1)%(p+1);
	for(int i=1;i<=n;i++) dlink[i]=ulink[i]=i;
	for(int i=1;i<=n;i++) colour[i]=0;
	if(n>p) {
		// let secondary items form their own doubly linked list
		for(int i=p+1;i<=n+1;i++) llink[i]=i-1;
		for(int i=p+1;i<=n+1;i++) rlink[i]=i+1;
		llink[p+1]=n+1,rlink[n+1]=p+1;
	}
	nextpos=n+1;
	spacerid=0;
	colour[nextpos]=0;
	top[nextpos++]=spacerid--;
}

// add row with m items, a[i] holds item number (1-indexed), c[i] holds colour
// c[i]==0 means no colour is specified
void addrow(int m,int *a, int *c) {
	if(nextpos+m>=MAX) printf("error, increase MAX to at least %d and recompile\n",nextpos+m+1),exit(0);
	int lastspacer=nextpos-1;
	for(int i=0;i<m;i++) {
		int ix=a[i];
		if(ix<1 || ix>numitems) printf("illegal item number %d, max is %d\n",ix,numitems);
		if(ix<=llink[0] && colour[0]>0) printf("illegal item, primary items can't have colour\n");
		len[ix]++;
		top[nextpos]=ix;
		dlink[nextpos]=ix;
		ulink[nextpos]=ulink[ix];
		dlink[ulink[ix]]=nextpos;
		ulink[ix]=nextpos;
		colour[nextpos]=c[i];
		nextpos++;
	}
	// add trailing spacer
	dlink[lastspacer]=nextpos-1;
	ulink[nextpos]=lastspacer+1;
	colour[nextpos]=0; // colour for spacer must be nonnegative
	top[nextpos++]=spacerid--;
}

// tests! /////////////////////////////////////////////////////////////////////

void printstate() {
	for(int i=0;i<=numitems+1;i++) printf("%d: llink %d rlink %d\n",i,llink[i],rlink[i]);
	for(int i=0;i<nextpos;i++) printf("%d: len %d top %d ulink %d dlink %d colour %d\n",i,len[i],top[i],ulink[i],dlink[i],colour[i]);
}

// extremely simple example to ensure that the algorithm works
// kept because i needed it for debugging
// should have 1 solution: items 2 and 4

void simpleexample() {
	// primary items: p,q,r (1-3)
	// secondary items: x,y (4-5)
	inithelp(5,2);
	int row[4];
	int c[4];
	// item 1: p q x y:A
	for(int i=0;i<4;i++) c[i]=0;
	row[0]=1; row[1]=2; row[2]=4; row[3]=5; c[3]='A';
	addrow(4,row,c);
	// item 2: p r x:A y
	for(int i=0;i<4;i++) c[i]=0;
	row[0]=1; row[1]=3; row[2]=4; row[3]=5; c[2]='A';
	addrow(4,row,c);
	// item 3: p x:B
	for(int i=0;i<2;i++) c[i]=0;
	row[0]=1; row[1]=4; c[1]='B';
	addrow(2,row,c);
	// item 4: q x:A
	row[0]=2; row[1]=4; c[0]=0; c[1]='A';
	addrow(2,row,c);
	// item 5: r y:B
	row[0]=3; row[1]=5; c[0]=0; c[1]='B';
	addrow(2,row,c);
	solutions=0;
	algorithmc();
	printf("simpleexample: %lld ways\n",solutions);
}

// trial division (slow), but it's not the bottleneck
int isprime(int n) {
	if(n%2==0) return 0;
	for(int i=3;i*i<=n;i+=2) if(n%i==0) return 0;
	return 1;
}

// n*n square with n horizontal and n vertical prime numbers
// i have no idea whether my solution is correct
// finds 2*2: 57, 3*3: 6068, 4*4: 5528716 solutions
// the sequence 57,6068 is not in oeis, which is a slightly bad sign
void primesq(int n) {
	int lo=1,hi=10,q;
	for(int i=1;i<n;i++) lo*=10,hi*=10;
	// items: n rows, n columns (primary), n*n cells (secondary)
	inithelp((n+2)*n,n*n);
	// create options: try every n-digit prime in each row and column
	for(int p=lo;p<hi;p++) if(isprime(p)) {
		for(int d=0;d<n;d++) {
			int row[n+1],c[n+1];
			// row/column d
			// d=0: p can't contain the digit 0
			// d==n-1: p can only contain digits 1,3,7,9
			q=p;
			while(q) {
				if(d==0 && q%10==0) goto done;
				if(d==n-1 && q%10!=1 && q%10!=3 && q%10!=7 && q%10!=9) goto done;
				q/=10;
			}
			// an option has n+1 items: row/col item, n cell items (with colour=digit)
			// if we want unique primes in grid: add a (n+2)th item, the prime
			row[0]=1+d; c[0]=0;
			q=p;
			for(int i=n,j=0;i>0;i--,j++) {
				row[i]=j+1+2*n+n*d;
				c[i]=q%10+'0';
				q/=10;
			}
			addrow(n+1,row,c);
			row[0]=1+d+n;
			for(int i=n,j=0;i>0;i--,j++) row[i]=j*n+1+2*n+d;
			addrow(n+1,row,c);
		done:;
		}
	}
	solutions=0;
	algorithmc();
	printf("%d*%d square of primes: %lld ways\n",n,n,solutions);
}

void primesquare() {
	// 5*5 has around 6e14 solutions, not very tractable
	for(int n=2;n<=4;n++) primesq(n);
}

int main() {
	simpleexample();
	primesquare();
	return 0;
}
