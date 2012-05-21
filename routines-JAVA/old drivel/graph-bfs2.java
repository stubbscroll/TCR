	// bfs2: uses two queues
	// - does not need to have distance pushed on the queue
	// - edges in graph can be both 0 and 1 (like moves and pushes in Sokoban)
	// moves of cost 0 are pushed to q2
	// moves of cost 1 are pushed to q1
	void bfs2() {
		LinkedList<Integer> q1=new LinkedList<Integer>(),q2;
		boolean visited[]=new boolean[MAX]; // MAX=max number of states
		int moves=1;
		// insert start element into queue, 
		q1.add(0);
		visited[0]=true;
		while(q1.size()>0) {
			q2=q1;
			q1=new LinkedList<Integer>();
			while(q2.size()>0) {
				// for each move: ny is the new state
					int ny;
					// if ny is the goal node:
					if(ny==goal) return moves;
					if(visited[ny]) continue;
					// mark as visited
					visited[ny]=true;
					// update distance table with moves
					q1.add(ny);
			}
			moves++;
		}
	}
