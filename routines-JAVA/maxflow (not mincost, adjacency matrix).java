/* 24.10.2008: SRM 422 div1-1000: OK (worst case: bfs 1461 ms, hack 1100 ms) */

class MaxFlow {
	int c[][],n,f[][],prev[],INF=1000000000,flag[];
	boolean taken[];
	MaxFlow(int cap[][]) {
		c=cap;
		n=c.length;
		f=new int[n][n];
		prev=new int[n];
		flag=new int[n];
		taken=new boolean[n];
	}
	int maxflow(int SOURCE,int SINK) {
		int res=0;
		while(augmentbfs(SOURCE,SINK)) {
			int flyt=INF,pos=SINK;
			while(prev[pos]>-1) {
				flyt=Math.min(flyt,c[prev[pos]][pos]-f[prev[pos]][pos]);
				pos=prev[pos];
			}
			pos=SINK;
			while(prev[pos]>-1) {
				f[prev[pos]][pos]+=flyt;
				f[pos][prev[pos]]-=flyt;
				pos=prev[pos];
			}
			pos=SINK;
			res+=flyt;

		}
		return res;
	}
	// runtime: 1461 ms
	boolean augmentbfs(int SOURCE,int SINK) {
		Arrays.fill(prev,-1);
		Arrays.fill(taken,false);
		LinkedList<Integer> q=new LinkedList<Integer>();
		q.add(SOURCE);
		taken[SOURCE]=true;
		while(q.size()>0) {
			int cur=q.poll();
			for(int i=0;i<n;i++) if(c[cur][i]>f[cur][i] && !taken[i]) {
				prev[i]=cur;
				if(i==SINK) return true;
				q.add(i);
				taken[i]=true;
			}
		}
		return false;
	}
	// runtime: 1100 ms
	boolean augmenthack(int SOURCE,int SINK) {
		Arrays.fill(prev,-1);
		Arrays.fill(flag,0);
		flag[SOURCE]=1;
		boolean done;
		do {
			done=true;
			for(int i=0;i<n;i++) if(flag[i]==1) {
				flag[i]=2;
				for(int j=0;j<n;j++) if(flag[j]<1 && c[i][j]>f[i][j]) {
					prev[j]=i;
					if(j==SINK) return true;
					flag[j]=1;
					done=false;
				}
			}
		} while(!done);
		return false;
	}
}