/* find all articulation points in connected undirected graph! */
/* NB, super duper efficient 2D version!
   - uses grid h[][] with coordinates between 1 and x/y inclusive!
     grid must use . for open cells, and must have border (sentinel),
     since bounds checking is omitted (saves time)
   - loops are unrolled, dfsvisit is cloned 4x (depending on dir) and parent
     direction is never tried
*/
/* usage:
   - add edges to from,to (set n, ne accordingly)
   - countingsort()
   - articulation(some node)
   - there are now acn elements in acx[],acy[] containing a list
     of articulation points (coordinates) */
/* not tested on competition problems, but behaves identically to the other
   articulation point routines on mortal coil */

#define MAX 150

char h[MAX][MAX];           /* . open, anything else wall */
char vis[MAX][MAX];         /* 1: node is visited */
int visn;
int ud[MAX][MAX];           /* at which time node i is visited */
int ulow[MAX][MAX];
int resn;                   /* counter */

char isartic[MAX][MAX];
int acx[MAX*MAX],acy[MAX*MAX];
int an;
int x,y;                    /* grid size */

#define DFS(dx,dy,navn) \
	w1=v1+dx; w2=v2+dy; \
	if(h[w1][w2]=='.') { \
		if(vis[w1][w2]) { \
			if(ulow[v1][v2]>ud[w1][w2]) ulow[v1][v2]=ud[w1][w2]; \
		} else { \
			navn(w1,w2); \
			if(ulow[v1][v2]>ulow[w1][w2]) ulow[v1][v2]=ulow[w1][w2]; \
			if(ulow[w1][w2]>=ud[v1][v2] && !isartic[v1][v2]) isartic[v1][v2]=1,acx[an]=v1,acy[an]=v2,an++; \
		} \
	}

void dfsvisitleft(int v1,int v2);
void dfsvisitup(int v1,int v2);
void dfsvisitdown(int v1,int v2);
void dfsvisitright(int v1,int v2) {
	int w1,w2;
	vis[v1][v2]=1;
	visn++;
	ud[v1][v2]=ulow[v1][v2]=++resn;
	DFS(1,0,dfsvisitright);
	DFS(0,1,dfsvisitdown);
	DFS(0,-1,dfsvisitup);
}

void dfsvisitdown(int v1,int v2) {
	int w1,w2;
	vis[v1][v2]=1;
	visn++;
	ud[v1][v2]=ulow[v1][v2]=++resn;
	DFS(1,0,dfsvisitright);
	DFS(0,1,dfsvisitdown);
	DFS(-1,0,dfsvisitleft);
}

void dfsvisitleft(int v1,int v2) {
	int w1,w2;
	vis[v1][v2]=1;
	visn++;
	ud[v1][v2]=ulow[v1][v2]=++resn;
	DFS(0,1,dfsvisitdown);
	DFS(-1,0,dfsvisitleft);
	DFS(0,-1,dfsvisitup);
}

void dfsvisitup(int v1,int v2) {
	int w1,w2;
	vis[v1][v2]=1;
	visn++;
	ud[v1][v2]=ulow[v1][v2]=++resn;
	DFS(1,0,dfsvisitright);
	DFS(-1,0,dfsvisitleft);
	DFS(0,-1,dfsvisitup);
}
#undef DFS

void dfsvisitroot(int v1,int v2) {
	int ch=0,w1,w2;
	vis[v1][v2]=1; visn++;
	ud[v1][v2]=ulow[v1][v2]=++resn;
	/* right */
	w1=v1+1; w2=v2;
	if(h[w1][w2]=='.' && !vis[w1][w2]) dfsvisitright(w1,w2),ch++;
	/* down */
	w1=v1; w2=v2+1;
	if(h[w1][w2]=='.' && !vis[w1][w2]) dfsvisitdown(w1,w2),ch++;
	/* left */
	w1=v1-1; w2=v2;
	if(h[w1][w2]=='.' && !vis[w1][w2]) dfsvisitleft(w1,w2),ch++;
	/* up */
	w1=v1; w2=v2-1;
	if(h[w1][w2]=='.' && !vis[w1][w2]) dfsvisitup(w1,w2),ch++;
	if(ch>1) isartic[v1][v2]=1,acx[an]=v1,acy[an]=v2,an++;
}

void articulation(int r1,int r2) {
	int i;
	an=resn=visn=0;
	for(i=0;i<x;i++) memset(vis[i+1],0,y+1),memset(isartic[i+1],0,y+1);
	dfsvisitroot(r1,r2);
}
