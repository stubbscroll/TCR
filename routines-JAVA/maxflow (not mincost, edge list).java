/* 24.10.2008: SRM 422 div1-1000: OK (worst case: bfs 341 ms, hack 192 ms) */

class MaxFlow {
	int c[][],n,f[][],prev[],INF=1000000000,flag[];
	int to[],gs[],ge[];
	boolean taken[];
	MaxFlow(int cap[][]) {
		c=cap;
		n=c.length;
		f=new int[n][n];
		prev=new int[n];
		flag=new int[n];
		taken=new boolean[n];
		gs=new int[n]; ge=new int[n];
		Arrays.fill(ge,-1); Arrays.fill(gs,-1);
		int ne=0,ix=0;
		for(int i=0;i<n;i++) for(int j=0;j<i;j++) if(c[i][j]>0 || c[j][i]>0) ne+=2;
		to=new int[ne];
		for(int i=0;i<n;i++) for(int j=0;j<n;j++) if(c[i][j]>0 || c[j][i]>0) {
			ge[i]=ix+1;
			if(gs[i]<0) gs[i]=ix;
			to[ix++]=j;
		}
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
	boolean augmentbfs(int SOURCE,int SINK) {
		Arrays.fill(prev,-1);
		Arrays.fill(taken,false);
		LinkedList<Integer> q=new LinkedList<Integer>();
		q.add(SOURCE);
		taken[SOURCE]=true;
		while(q.size()>0) {
			int cur=q.poll();
			for(int k=gs[cur];k<ge[cur];k++) {
				int i=to[k];
				if(c[cur][i]>f[cur][i] && !taken[i]) {
					prev[i]=cur;
					if(i==SINK) return true;
					q.add(i);
					taken[i]=true;
				}
			}
		}
		return false;
	}
	boolean augmenthack(int SOURCE,int SINK) {
		Arrays.fill(prev,-1);
		Arrays.fill(flag,0);
		flag[SOURCE]=1;
		boolean done;
		do {
			done=true;
			for(int i=0;i<n;i++) if(flag[i]==1) {
				flag[i]=2;
				for(int k=gs[i];k<ge[i];k++) {
					int j=to[k];
					if(flag[j]<1 && c[i][j]>f[i][j]) {
						prev[j]=i;
						if(j==SINK) return true;
						flag[j]=1;
						done=false;
					}
				}
			}
		} while(!done);
		return false;
	}
}
