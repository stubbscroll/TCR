#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// algorithm x (dancing links) for finding exact cover.
// supports secondary items that can be covered <=1 times (but not twice),
// items that aren't included in the horizontal doubly linked list (that is,
// items not reachable from node 0) become secondary.
// source: the art of computer programming volume 4b (donald knuth)

// 2022-11-18: langford(n) correct up to 16
// 2022-11-19: n-queens: correct up to 16 (uses secondary items)
// 2022-11-21: sudoku: tried 224838 puzzles, 1 solution found on all of them
//             (haven't actually inspected any of the solutions to see if
//             they're correctly solved)
// 2022-11-22: 16x16 sudoku: tried 8 puzzles, found unique solutions
//             (didn't inspect these either)
// 2023-01-26: pack pentominoes into rectangles: correct (i think)

#define MAXITEM 1050
#define MAX 20000

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
	if(n>=MAXITEM) printf("error, increase MAXITEM and recompile\n"),exit(0);
	numitems=n;
	int p=n-m; // primary items
	for(int i=1;i<=n;i++) len[i]=0;
	for(int i=0;i<=p;i++) llink[i]=(i+p)%(p+1);
	for(int i=0;i<=p;i++) rlink[i]=(i+1)%(p+1);
	for(int i=1;i<=n;i++) dlink[i]=ulink[i]=i;
	if(n>p) {
		// let secondary items form their own doubly linked list
		for(int i=p+1;i<=n+1;i++) llink[i]=i-1;
		for(int i=p+1;i<=n+1;i++) rlink[i]=i+1;
		llink[p+1]=n+1,rlink[n+1]=p+1;
	}
	nextpos=n+1;
	spacerid=0;
	top[nextpos++]=spacerid--;
}

// add row with m ones, a[i] holds item number (1-indexed)
void addrow(int m,int *a) {
	if(nextpos+m>=MAX) printf("error, increase MAX to at least %d and recompile\n",nextpos+m+1),exit(0);
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

// find all ways to place n non-attacking queens on a n*n chessboard
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

// solve a lot of sudoku puzzles
void sudoku() {
	// sudoku setup stolen from knuth vol 4b
	// options have the form p_ij r_ik c_jk b_xk
	// where 0 <= i,j < 9, 1 <= k <= 9, x=3*int(i/3)+int(j/3)
	// where i,j are the cell coordinates (i is y-axis)
	// and k is the digit to be placed
	// there are 324 items (81 for each of p, r, c, b, and all are
	// primary) and 729 options (starting clues reduce these amounts)
	// test cases from here:
	// https://drive.google.com/drive/folders/1e_hQNQiJVxhHP_5WBQhBNur9dnS5ml3E
	// i used tarek_pearly6000, top_50k_toughest, 17puz49157, sudoku_diabolical
	FILE *f=fopen("sudoku.txt","r");
	if(!f) puts("error, sudoku.txt not found"),exit(0);
	char s[1000];
	int good=0,total=0;
	while(fgets(s,998,f)) {
		int map[4][9][9]; // get item number from coordinates
		char ok[4][9][9]; // 1: item is not deleted by starting clues
		memset(map,-1,sizeof(map));
		memset(ok,1,sizeof(ok));
		for(int l=0;l<81;l++) if(s[l]!='0') {
			int d=s[l]-'0';
			int i=l/9,j=l%9;
			ok[0][i][j]=0;    // cell i,j taken
			ok[1][i][d-1]=0;  // digit d taken in row i
			ok[2][j][d-1]=0;  // digit d taken in column j
			int x=3*(i/3)+j/3;
			ok[3][x][d-1]=0;  // digit d taken in box x
		}
		// find unfilled squares
		int items=1;
		for(int l=0;l<81;l++) if(s[l]=='0') {
			int i=l/9,j=l%9;
			if(ok[0][i][j]) map[0][i][j]=items++;
			for(int k=1;k<=9;k++) {
				if(ok[1][i][k-1] && map[1][i][k-1]<0) map[1][i][k-1]=items++;
				if(ok[2][j][k-1] && map[2][j][k-1]<0) map[2][j][k-1]=items++;
				int x=3*(i/3)+j/3;
				if(ok[3][x][k-1] && map[3][x][k-1]<0) map[3][x][k-1]=items++;
			}
		}
		inithelp(items-1,0);
		// create options
		int options=0;
		for(int i=0;i<9;i++) for(int j=0;j<9;j++) for(int k=1;k<=9;k++) {
			int row[4];
			int x=3*(i/3)+j/3;
			if(map[0][i][j]<0) continue; // cell i,j already filled out
			if(map[1][i][k-1]<0) continue; // row i already has digit k in clues
			if(map[2][j][k-1]<0) continue; // column j already has digit k in clues
			if(map[3][x][k-1]<0) continue; // box x already has digit k in clues
			row[0]=map[0][i][j];
			row[1]=map[1][i][k-1];
			row[2]=map[2][j][k-1];
			row[3]=map[3][x][k-1];
			addrow(4,row);
			options++;
		}
		solutions=0;
		algorithmx();
		if(solutions==1) good++;
		total++;
	}
	printf("sudoku: %d puzzles tried, %d with unique solution\n",total,good);
}

void sudoku16x16() {
	// setup similar to 9x9 sudoku
	// 4*256=1024 items
	// 16*16*16=4096 options (starting clues reduce these amounts)
	// test cases taken from:
	// https://stackoverflow.com/questions/70934747/16x16-sudoku-solver-using-hex-characters
	// https://github.com/jtortorelli/sudoku16/tree/master/example_puzzles
	// http://acm.ro/2006/problems.htm (problem A)
	// 4*16*16=1024 items, 16*16*16=4096 options
	FILE *f=fopen("sudoku16.txt","r");
	if(!f) puts("error, sudoku16.txt not found"),exit(0);
	char s[1000];
	int good=0,total=0;
	while(fgets(s,998,f)) {
		int map[4][16][16]; // get item number from coordinates
		char ok[4][16][16]; // 1: item is not deleted by starting clues
		memset(map,-1,sizeof(map));
		memset(ok,1,sizeof(ok));
		// convert input to correct format
		// this also canonically sorts values by occurrence
		char z[128];
		int id='A';
		for(int i=0;i<128;i++) z[i]=-1;
		for(int i=0;i<256;i++) {
			if(s[i]=='.') s[i]='-';
			else if(s[i]!='-' && s[i]>=0 && s[i]<128 && z[(int)s[i]]<0) z[(int)s[i]]=id++;
		}
		for(int i=0;i<256;i++) if(z[(int)s[i]]>=0) s[i]=z[(int)s[i]];
		for(int l=0;l<256;l++) if(s[l]!='-') {
			int d=s[l]-'A'+1;
			int i=l/16,j=l%16;
			ok[0][i][j]=0;    // cell i,j taken
			ok[1][i][d-1]=0;  // digit d taken in row i
			ok[2][j][d-1]=0;  // digit d taken in column j
			int x=4*(i/4)+j/4;
			ok[3][x][d-1]=0;  // digit d taken in box x
		}
		// find unfilled squares
		int items=1;
		for(int l=0;l<256;l++) if(s[l]=='-') {
			int i=l/16,j=l%16;
			if(ok[0][i][j]) map[0][i][j]=items++;
			for(int k=1;k<=16;k++) {
				if(ok[1][i][k-1] && map[1][i][k-1]<0) map[1][i][k-1]=items++;
				if(ok[2][j][k-1] && map[2][j][k-1]<0) map[2][j][k-1]=items++;
				int x=4*(i/4)+j/4;
				if(ok[3][x][k-1] && map[3][x][k-1]<0) map[3][x][k-1]=items++;
			}
		}
		inithelp(items-1,0);
		// create options
		int options=0;
		for(int i=0;i<16;i++) for(int j=0;j<16;j++) for(int k=1;k<=16;k++) {
			int row[4];
			int x=4*(i/4)+j/4;
			if(map[0][i][j]<0) continue; // cell i,j already filled out
			if(map[1][i][k-1]<0) continue; // row i already has digit k in clues
			if(map[2][j][k-1]<0) continue; // column j already has digit k in clues
			if(map[3][x][k-1]<0) continue; // box x already has digit k in clues
			row[0]=map[0][i][j];
			row[1]=map[1][i][k-1];
			row[2]=map[2][j][k-1];
			row[3]=map[3][x][k-1];
			addrow(4,row);
			options++;
		}
		solutions=0;
		algorithmx();
		if(solutions==1) good++;
		total++;
	}
	printf("sudoku16: %d puzzles tried, %d with unique solution\n",total,good);
}

// start of big example (polyominoes) /////////////////////////////////////////

#define MAXPIECE 50
#define MAXSIZE 7
#define MAXGRID 40

char piece[MAXPIECE][8][MAXSIZE][MAXSIZE+1];
int pieces;                  // number of pieces
int piecerot[MAXPIECE];      // number of rotations/symmetries
int piecex[MAXPIECE][8]; // [i][j] x-size of piece i, rotation j
int piecey[MAXPIECE][8]; // y-size of piece

char map[MAXGRID][MAXGRID+1];  // 0:cell is available, 1:it's not
int cellid[MAXGRID][MAXGRID];  // id for each cell
int mapx,mapy;               // map size

void makemap(int x,int y) {
	mapx=x; mapy=y;
	for(int i=0;i<x;i++) for(int j=0;j<=y;j++) map[i][j]=0;
}

void generatecellid() {
	int id=0;
	for(int i=0;i<mapx;i++) for(int j=0;j<mapy;j++) {
		if(map[i][j]) cellid[i][j]=-1;
		else cellid[i][j]=id++;
	}
}

// copy from p to q
void copypiece(char p[MAXSIZE][MAXSIZE+1],char q[MAXSIZE][MAXSIZE+1],int x,int y) {
	for(int i=0;i<x;i++) for(int j=0;j<y;j++) q[i][j]=p[i][j];
}

// add symmetry of piece with index ix
// (only adds it if it doesn't exist)
void addpiece(char p[MAXSIZE][MAXSIZE+1],int x,int y,int ix) {
	for(int k=0;k<piecerot[ix];k++) {
		if(x!=piecex[ix][k] || y!=piecey[ix][k]) continue; // different
		for(int i=0;i<x;i++) for(int j=0;j<y;j++) if(piece[ix][k][i][j]!=p[i][j]) goto different;
		return; // rotation exists
	different:;
	}
	if(piecerot[ix]>=8) printf("error, can't have more than 8 symmetries"),exit(0);
	copypiece(p,piece[ix][piecerot[ix]],x,y);
	piecex[ix][piecerot[ix]]=x;
	piecey[ix][piecerot[ix]]=y;
	piecerot[ix]++;
}

// add a piece
// rot=1: create all possible rotations and symmetries of given piece
void createpiece(char p[MAXSIZE][MAXSIZE+1],int x,int y,int rot) {
	if(pieces>=MAXPIECE) printf("error, increase MAXPIECE and recompile\n"),exit(0);
	piecerot[pieces]=0;
	addpiece(p,x,y,pieces);
	if(rot) {
		for(int r=1;r<8;r++) {
			if(r==4) {
				// flip
				for(int i=0;i<x;i++) for(int j=0;j+j<y;j++) {
					char t=p[i][j]; p[i][j]=p[i][y-j-1]; p[i][y-j-1]=t;
				}
			}
			// rotate 90 degrees
			char q[MAXSIZE][MAXSIZE+1];
			for(int i=0;i<x;i++) for(int j=0;j<y;j++) q[j][x-i-1]=p[i][j];
			int t=x; x=y; y=t;
			copypiece(q,p,x,y);
			addpiece(p,x,y,pieces);
		}
	}
	pieces++;
}

void createpentominoes() {
	pieces=0;
	char T[MAXSIZE][MAXSIZE+1]={"###",".#.",".#."};
	char U[MAXSIZE][MAXSIZE+1]={"#.#","###"};
	char V[MAXSIZE][MAXSIZE+1]={"#..","#..","###"};
	char W[MAXSIZE][MAXSIZE+1]={"#..","##.",".##"};
	char X[MAXSIZE][MAXSIZE+1]={".#.","###",".#."};
	char Y[MAXSIZE][MAXSIZE+1]={"..#.","####"};
	char Z[MAXSIZE][MAXSIZE+1]={"##.",".#.",".##"};
	char F[MAXSIZE][MAXSIZE+1]={".##","##.",".#."};
	char I[MAXSIZE][MAXSIZE+1]={"#####"};
	char L[MAXSIZE][MAXSIZE+1]={"####","#..."};
	char P[MAXSIZE][MAXSIZE+1]={"##","##","#."};
	char N[MAXSIZE][MAXSIZE+1]={"##..",".###"};
	createpiece(T,3,3,1);
	createpiece(U,2,3,1);
	createpiece(V,3,3,1);
	createpiece(W,3,3,1);
	createpiece(X,3,3,1);
	createpiece(Y,2,4,1);
	createpiece(Z,3,3,1);
	createpiece(F,3,3,1);
	createpiece(I,1,5,1);
	createpiece(L,2,4,1);
	createpiece(P,3,2,1);
	createpiece(N,2,4,1);
}

void packpolyominoes() {
	// items (all primary):
	// one for each piece, one for each cell to be covered
	int count=0;
	for(int i=0;i<mapx;i++) for(int j=0;j<mapy;j++) if(!map[i][j]) count++;
	inithelp(pieces+count,0);
	// options: try all rotations and symmetries of all pieces on all cells
	int row[100],rn;
	for(int p=0;p<pieces;p++) for(int r=0;r<piecerot[p];r++) {
		for(int i=0;i<mapx-piecex[p][r]+1;i++) for(int j=0;j<mapy-piecey[p][r]+1;j++) {
			// check if all cells under piece are available
			for(int k=0;k<piecex[p][r];k++) for(int l=0;l<piecey[p][r];l++) {
				if(piece[p][r][k][l]=='#' && map[i+k][j+l]) goto fail;
			}
			// piece can be placed, make option
			rn=0;
			row[rn++]=p+1;
			for(int k=0;k<piecex[p][r];k++) for(int l=0;l<piecey[p][r];l++) {
				if(piece[p][r][k][l]=='#') {
					if(rn>=100) printf("option has too many items\n"),exit(0);
					row[rn++]=pieces+1+cellid[i+k][j+l];
				}
			}
			addrow(rn,row);
		fail:;
		}
	}

	// solve
	solutions=0;
	algorithmx();
}

// pack pentominoes in x*y grid
void pentominoes(int x,int y) {
	createpentominoes();
	makemap(x,y);
	generatecellid();
	packpolyominoes();
	printf("pentominoes(%d,%d): %lld solutions\n",x,y,solutions);
}

// pack pentominoes in chessboard minus 2x2 center
void pentominoes_chess() {
	createpentominoes();
	makemap(8,8);
	map[3][3]=1; map[3][4]=1; map[4][3]=1; map[4][4]=1;
	generatecellid();
	packpolyominoes();
	printf("pentominoes on chessboard minus center: %lld solutions\n",solutions);
}

// number of solutions agree with this webpage
// https://jansipke.nl/yasumi-puzzle/
void polyominoes() {
	pentominoes(3,20);
	pentominoes(4,15);
	pentominoes(5,12);
	pentominoes(6,10);
	// sanity test: transposed grids should return the same answer
	pentominoes(20,3);
	pentominoes(15,4);
	pentominoes(12,5);
	pentominoes(10,6);
	pentominoes_chess();
}

int main() {
	langford_all();
	nqueens_all();
	sudoku();
	sudoku16x16();
	polyominoes();
	return 0;
}
