	int SOURCE,SINK;
	int prev[];
	void initflow(int N) {
		prev=new int[N];
	}
	// (bipartite) maxflow which finds a path of weight 1 for each call.
	// globals: prev[]: predecessors
	// SOURCE,SINK:     source,sink
	// worst case: O(N^2)
	int bipartiteflow(int g[][],int N) {
		Queue<Integer> q=new LinkedList<Integer>();
		int t[]=new int[N];
		for(int i=0;i<N;i++) prev[i]=-1;
		q.offer(SOURCE);
		t[SOURCE]=1;
		boolean done=false;
		while(q.size()>0 && !done) {
			int from=q.poll();
			for(int to=0;to<N;to++) if(t[to]<1 && g[from][to]>0) {
				prev[to]=from;
				if(to==SINK) { done=true; break; }
				t[to]=1;
				q.offer(to);
			}
		}
		if(!done) return 0;
		int b=SINK,a;
		while((a=prev[b])>-1) {
			g[a][b]--;
			g[b][a]++;
			b=a;
		}
		return 1;
	}

	// print the graph weights with 1 digit
	void printgraph(int g[][],int N)
	{
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) System.out.print(g[i][j]>9?"+":g[i][j]);
			System.out.println("");
		}
	}
