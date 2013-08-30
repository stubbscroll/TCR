/* OK UVa 315, n<100, 0.019 seconds, 2013-08-06 */

/* find all articulation points in connected undirected graph
   (non-recursive version) */
/* usage:
   - add edges to from,to (set n, ne accordingly) (note that both
     directions are needed)
   - countingsort()
   - articulation(some node)
   - there are now acn elements in ac[] containing a list
     of articulation points (node numbers) */

#include <stdio.h>
#include <string.h>

#define MAXV 111
#define MAXE 99999

int n,ne;               /* number of nodes, number of edges */
int from[MAXE],to[MAXE];/* edge i: from[i] to[i] */
int gs[MAXV+1];         /* gs[i]: start of edges from i, gs[i+1]: end */

char vis[MAXV];         /* 1:node has been visited */
int ud[MAXV];           /* at which time node was visited */
int ulow[MAXV];         /* lowest time among neighbours */
int resn;               /* counter */

char isartic[MAXV];
int ac[MAXV];           /* list of articulation points */
int acn;                /* number of articulation points */

void dfsiter(int v,int p) {
	static int stz[MAXV*2+2];
	int sp=-2,at,w,*st=stz+2;
	vis[v]=1;
	ud[v]=ulow[v]=++resn;
	st[sp++]=p; st[sp++]=0;
	st[sp++]=v; st[sp++]=gs[v];
	while(sp) {
		sp-=2;
		v=st[sp]; at=st[sp+1]; p=st[sp-2];
		while(1) {
			if(at==gs[v+1]) {
				if(!sp) return;
				if(ulow[p]>ulow[v]) ulow[p]=ulow[v];
				if(ulow[v]>=ud[p] && !isartic[p]) isartic[p]=1,ac[acn++]=p;
				break;
			}
			w=to[at];
			if(w==p) at++;
			else if(!vis[w]) {
				vis[w]=1;
				p=st[sp]=v;
				st[sp+1]=at+1;
				sp+=2;
				at=gs[w]; v=w;
				ud[v]=ulow[v]=++resn;
			} else {
				if(ulow[v]>ud[w]) ulow[v]=ud[w];
				at++;
			}
		}
	}
}

void dfsvisit(int v) {
	int ch=0,i;
	vis[v]=1; ud[v]=ulow[v]=++resn;
	for(i=gs[v];i<gs[v+1];i++) if(!vis[to[i]]) dfsiter(to[i],v),ch++;
	if(ch>1) isartic[v]=1,ac[acn++]=v;
}

void articulation(int root) {
	resn=acn=0;
	memset(vis,0,n);
	memset(isartic,0,n);
	dfsvisit(0);
}

void countingsort() {
	static int newto[MAXE];
	int i,j;
	for(i=0;i<=n;i++) gs[i]=0;
	for(i=0;i<ne;i++) gs[from[i]]++;
	for(i=1;i<n;i++) gs[i]+=gs[i-1];
	gs[n]=ne;
	for(i=0;i<ne;i++) {
		j=--gs[from[i]];
		newto[j]=to[i];
	}
	for(i=0;i<ne;i++) to[i]=newto[i];
	for(i=0;i<n;i++) for(j=gs[i];j<gs[i+1];j++) from[j]=i;
}
