#include <stdio.h>
#include <string.h>

/* preflow-push maxflow with efficient edge lists
   based on http://en.wikipedia.org/wiki/Preflow_push
   runtime: dunno
*/
/* usage:
   - set MAXV,MAXE to guaranteed maximum values for the problem (MAXE
     includes back edges!)
   - set n to desired number of nodes, set ne=0
   - define edges by using addedge(), preferably from left to right.
     back edges are added automatically.
   - radixsort()
   - inverseedges()
   - for each edge and back edge, set cap[i]. set cap on back edges to 0.
     if there are actual edges both ways, set cap normally
   - finally, call maxflow()
*/
/* TODO implement "highest active vertex" selection rule, it runs in
   O(V^2 * sqrt(E)), and should potentially be always faster */
/* TODO investigate "gap heuristic" (wikipedia) */

/* OK UVa 563 0.992 seconds, n<=5002, 30.11.2012 */
/* OK UVa 820 0.080 seconds, n<=100, 29.11.2012 */
#define MAXV 500002
#define MAXE 13000000
#define INF 1000000000

int from[MAXE],to[MAXE];  /* graph */
int cap[MAXE];            /* capacity */
int gs[MAXV+1];           /* pointer to first node */
int inv[MAXE];            /* index to reverse edge */
int n,ne;

int f[MAXE];              /* flow */
int height[MAXV];         /* height of each node */
int excess[MAXV];         /* excess flow */

void push(int i) {
	int u=from[i],v=to[i];
	int t=excess[u]<cap[i]-f[i]?excess[u]:cap[i]-f[i];
	f[i]+=t;
	f[inv[i]]-=t;
	excess[u]-=t;
	excess[v]+=t;
}

void relabel(int u) {
	int i,v,m=INF;
	for(i=gs[u];i<gs[u+1];i++) if(cap[i]-f[i]>0) {
		v=to[i];
		if(m>height[v]) m=height[v];
	}
	height[u]=m+1;
}

void discharge(int *seen,int u) {
	int i,v;
	while(excess[u]>0) {
		if(seen[u]<gs[u+1]-gs[u]) {
			i=gs[u]+seen[u];
			v=to[i];
			if(cap[i]-f[i]>0 && height[u]>height[v]) push(i);
			else seen[u]++;
		} else {
			relabel(u);
			seen[u]=0;
		}
	}
}

int maxflow(int source,int sink) {
	static int q[MAXV];
	static int seen[MAXV];
	int i,p,u,h;
	memset(excess,0,sizeof(int)*n);
	memset(height,0,sizeof(int)*n);
	memset(seen,0,sizeof(int)*n);
	memset(f,0,sizeof(int)*ne);
	for(i=p=0;i<n;i++) if(i!=source && i!=sink) q[p++]=i;
	height[source]=n;
	excess[source]=INF;
	for(i=gs[source];i<gs[source+1];i++) push(i);
	p=0;
	while(p<n-2) {
		u=q[p];
		h=height[u];
		discharge(seen,u);
		if(height[u]>h) {
			h=q[p];
			while(p) q[p]=q[p-1],p--;
			q[0]=h;
		} else p++;
	}
	for(p=0,i=gs[sink];i<gs[sink+1];i++) p-=f[i];
	return p;
}

void radixsort() {
  static int newfrom[MAXE],newto[MAXE];
  int i,j;
  /* sort on to */
  memset(gs,0,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) gs[to[i]]++;
  for(i=1;i<n;i++) gs[i]+=gs[i-1];
  for(i=ne-1;i>=0;i--) {
    j=--gs[to[i]];
    newto[j]=to[i];
    newfrom[j]=from[i];
  }
  /* (stable) sort on from */
  memset(gs,0,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) gs[newfrom[i]]++;
  for(i=1;i<n;i++) gs[i]+=gs[i-1];
  gs[n]=ne;
  for(i=ne-1;i>=0;i--) {
    j=--gs[newfrom[i]];
    to[j]=newto[i];
    from[j]=newfrom[i];
  }
}

/* for each edge a->b, find index to b->a */
void inverseedges() {
  static int starts[MAXV+1];
  int i;
  memcpy(starts,gs,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) inv[i]=starts[to[i]]++;
}

/* add both directions of an edge */
void addedge(int a,int b) {
	from[ne]=a; to[ne++]=b;
	from[ne]=b; to[ne++]=a;
}
