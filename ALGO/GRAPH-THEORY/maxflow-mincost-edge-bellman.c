#define MAXV 102
#define MAXE 20000
#define INF 1000000000

/* min-cost maxflow! this routine is not optimized for sparse graphs,
   as augment() always takes at least n operations.
   usage: see maxflow-tripartite, but in addition set cost[] for each edge.
   remember negative costs on back edges!

   TODO change augment to be faster on shorter augmenting paths (clean up
   dist/vis/prev with list of visited nodes)
*/

/* OK Topcoder SRM 506 div 1-600 worst case 11 ms n<=103 30.11.2012 */
int from[MAXE],to[MAXE];  /* graph */
int gs[MAXV+1];           /* pointer to first node */
int f[MAXE];              /* flow/capacity/residual graph */
int cost[MAXE];           /* cost per flow unit */
int inv[MAXE];            /* index to reverse edge */
int n,ne;                 /* number of nodes, edges */

int prev[MAXV],dist[MAXV],q[MAXE];

int augment(int source,int sink) {
	static char vis[MAXV];
	int cur,ix,i,qs,qe;
	memset(vis,0,n);
	memset(dist,120,sizeof(int)*n);
	memset(prev,-1,sizeof(int)*n);
	dist[source]=qs=qe=0;
	q[qe++]=source;
	while(qs<qe) {
		cur=q[qs++];
		vis[cur]=0;
		for(ix=gs[cur];ix<gs[cur+1];ix++) if(f[ix]>0) {
			i=to[ix];
			if(dist[i]>dist[cur]+cost[ix]) {
				dist[i]=dist[cur]+cost[ix];
				prev[i]=ix;
				if(!vis[i]) q[qe++]=i,vis[i]=1;
			}
		}
	}
	return dist[sink];
}

int mincost(int source,int sink) {
	int res=0,cur,pos,flow,ix;
	while((cur=augment(source,sink))<INF) {
		pos=sink,flow=INF;
		while((ix=prev[pos])>-1) {
			if(flow>f[ix]) flow=f[ix];
			pos=from[ix];
		}
		res+=cur*flow;
		pos=sink;
		while((ix=prev[pos])>-1) {
			f[ix]-=flow;
			f[inv[ix]]+=flow;
			pos=from[ix];
		}
	}
	return res;
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
