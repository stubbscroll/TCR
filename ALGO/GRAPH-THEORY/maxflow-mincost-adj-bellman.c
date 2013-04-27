#define MAXV 102
#define INF 1000000000

/* easy-to-use mincost maxflow for smaller graphs, optimized for
   code size and ease of typing in. doesn't support multiple edges between two
   nodes.
   graph should not have negative cost cycles!
   usage: define capacities in f[][] and costs in cost[][]. remember negative
   costs for back edges!
*/

/* OK UVa 10746 n<=42, 0.004 seconds, 01.12.2012 */
/* OK Topcoder SRM 506 div 1-600 n<=103, 10 ms worst case, 01.12.2012 */
/* OK Topcoder TCO13-R1A div1-1000 n<=452, 67 ms worst case, 23.02.2013 */
int f[MAXV][MAXV];
int cost[MAXV][MAXV];
int n;

int prev[MAXV],dist[MAXV],q[MAXV*MAXV];

int augment(int source,int sink) {
	static char vis[MAXV];
	int cur,i,qs,qe;
	memset(vis,0,n);
	memset(dist,120,sizeof(int)*n);
	memset(prev,-1,sizeof(int)*n);
	dist[source]=qs=qe=0;
	q[qe++]=source;
	while(qs<qe) {
		cur=q[qs++];
		vis[cur]=0;
		for(i=0;i<n;i++) if(f[cur][i]>0) {
			if(dist[i]>dist[cur]+cost[cur][i]) {
				dist[i]=dist[cur]+cost[cur][i];
				prev[i]=cur;
				if(!vis[i]) q[qe++]=i,vis[i]=1;
			}
		}
	}
	return dist[sink];
}

int mincost(int source,int sink) {
	int res=0,cur,pos,flow,p;
	while((cur=augment(source,sink))<INF) {
		pos=sink,flow=INF;
		while((p=prev[pos])>-1) {
			if(flow>f[p][pos]) flow=f[p][pos];
			pos=prev[pos];
		}
		res+=cur*flow;
		pos=sink;
		while((p=prev[pos])>-1) {
			f[p][pos]-=flow;
			f[pos][p]+=flow;
			pos=prev[pos];
		}
	}
	return res;
}
