/* 05.11.2007: SRM 372 div1-1000 OK, worst case 1346 ms (102 nodes) */
/* 12.05.2011: SRM 506 div1-1000 OK, worst case 26 ms (103 nodes) */

/*  NB! remember to set cost[j][i]=-cost[i][j]! flow can go both ways!

    Usage: Maxflow mf = new Maxflow(cost,cap) where cost, cap are
           adjacency matrices containing cost and cap.
    mincost(source,sink) returns the maxflow and least cost. */

class Maxflow {
	int cost[][],cap[][],n,INF=Integer.MAX_VALUE/2,f[][],source,sink;
	int prev[],dist[];
   	public Maxflow(int cost[][],int cap[][]) {
		this.cost=cost; this.cap=cap; n=cost.length;
		f=new int[n][n]; dist=new int[n]; prev=new int[n];
	}
	// [0]=flow found, [1]=total cost
	public int[] mincost(int source,int sink) {
		int res[]=new int[2],cur;
		this.source=source; this.sink=sink;
		while((cur=augment())<INF) {
			int pos=sink,flow=INF,p;
			while((p=prev[pos])>-1) {
				flow=Math.min(flow,cap[p][pos]-f[p][pos]);
				pos=p;
			}
			res[1]+=cur*flow;
			pos=sink; res[0]+=flow;
			while((p=prev[pos])>-1) {
				f[p][pos]+=flow;
				f[pos][p]-=flow;
				pos=p;
			}
		}
		return res;
	}
	public int augment() {
		Arrays.fill(dist,INF); Arrays.fill(prev,-1); dist[source]=0;
		boolean t[]=new boolean[n];
		LinkedList<Integer> q=new LinkedList<Integer>();
		q.add(source);
		while(q.size()>0) {
			int cur=q.removeFirst();
			t[cur]=false;
			for(int i=0;i<n;i++) if(cap[cur][i]>f[cur][i]) {
				if(dist[i]>dist[cur]+cost[cur][i]) {
					dist[i]=dist[cur]+cost[cur][i];
					prev[i]=cur;
					if(!t[i]) { q.add(i); t[i]=true; }
				}
			}
		}
		return dist[sink];
	}
}
