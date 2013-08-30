/* OK UVa 315, n<100, 0.019 seconds, 2013-08-05 */

/* find all articulation points in connected undirected graph! */
/* usage:
   - add edges to from,to (set n, ne accordingly)
   - countingsort()
   - articulation(some node)
   - there are now acn elements in ac[] containing a list
     of articulation points (node numbers)

   this is faster, but a bit longer. use the other one in
   online competitions */

#define MAXV 111
#define MAXE 99999

int n,ne;               /* number of nodes, number of edges */
int from[MAXE],to[MAXE];/* edge i: from[i] to[i] */
int gs[MAXV+1];         /* gs[i]: start of edges from i, gs[i+1]: end */

char vis[MAXV];         /* 1: node is visited */
int ud[MAXV];           /* at which time node i is visited */
int ulow[MAXV];
int resn;               /* counter */

char isartic[MAXV];
int ac[MAXV];           /* list of articulation points */
int acn;                /* number of articulation points */

void dfsvisit(int v,int p) {
	int i,w;
	vis[v]=1;
	ud[v]=ulow[v]=++resn;
	for(i=gs[v];i<gs[v+1];i++) {
		w=to[i];
		if(w==p) continue;
		if(vis[w]) {
			if(ulow[v]>ud[w]) ulow[v]=ud[w];
		} else {
			dfsvisit(w,v);
			if(ulow[v]>ulow[w]) ulow[v]=ulow[w];
			if(ulow[w]>=ud[v] && !isartic[v]) isartic[v]=1,ac[acn++]=v;
		}
	}
}

void dfsvisitroot(int v) {
	int ch=0,i;
	vis[v]=1;
	ud[v]=ulow[v]=++resn;
	for(i=gs[v];i<gs[v+1];i++) if(!vis[to[i]]) dfsvisit(to[i],v),ch++;
	if(ch>1) isartic[v]=1,ac[acn++]=v;
}

void articulation(int root) {
	resn=acn=0;
	memset(vis,0,n);
	memset(isartic,0,n);
	dfsvisitroot(0);
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
