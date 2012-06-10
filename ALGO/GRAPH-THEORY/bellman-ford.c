#define MAXE 200
#define MAXV 128

int from[MAXE],to[MAXE],cost[MAXE];
int n,ne;
int dist[MAXV];

#define INF 1000000

/* bellman-ford! return 1 if no negative cycle, 0 if it has. */
/* yes, that goto is a significant speedup */
/* UVa 515 0.096 seconds n<=102 m<=202 10.06.2012 */
int bellmanford(int start) {
	int i,iter;
	for(i=0;i<n;i++) dist[i]=INF;
	dist[start]=0;
	for(iter=0;iter<n;iter++) {
		for(i=0;i<ne;i++) if(dist[to[i]]>dist[from[i]]+cost[i]) goto changed;
		return 1;
		for(;i<ne;i++) {
			if(dist[to[i]]>dist[from[i]]+cost[i]) {
		changed:
				dist[to[i]]=dist[from[i]]+cost[i];
			}
		}
	}
	return 0;
}
