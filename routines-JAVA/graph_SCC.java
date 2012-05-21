import java.util.*;

/* DFS (without recursion), toposort, SCC, all in O(N) */
/* 25.10.2009: UVa 11709 OK */

public class graph_SCC {

	int n,ne; 		// #nodes, #edges
	int from[],to[];// edge i: from[i] to[i]
	int gs[];		// size n+1:	gs[i] start of edges from i
					//				gs[i+1] end of edges from i (exclusive)
	int vis[];		// during dfs: 0=unvisited 1=under processing 2=finalised
	int res[];		// finalising order, from dfs
	int resn;		// teller
	boolean cycle;	// true: graph can't be toposorted

	// sort on from, don't care about to
	void countingsort() {
		gs=new int[n+1];
		for(int i=0;i<ne;i++) gs[from[i]]++;
		for(int i=1;i<n;i++) gs[i]+=gs[i-1];
		gs[n]=ne;
		int newfrom[]=new int[ne];
		int newto[]=new int[ne];
		for(int i=0;i<ne;i++) {
			int num=from[i],til=--gs[num];
			newfrom[til]=from[i];
			newto[til]=to[i];
		}
		from=newfrom;
		to=newto;
	}

	// before calling dfs: initialize vis[] to 0, res[] to 0, resn to 0
	// cycle to false (if we want to detect cycle)
	void dfsvisit(int u) {
		Stack<Integer> stack = new Stack<Integer>();
		stack.push(u); stack.push(gs[u]);
		vis[u]=1;
	outer:
		while(stack.size()>0) {
			int at=stack.pop(); u=stack.pop();
		inner:
			while(true) {
				if(at==gs[u+1]) {
					vis[u]=2;
					res[resn++]=u;
					continue outer;
				} else {
					int v=to[at];
					if(vis[v]==1) { cycle=true; at++; } // may want to quit here if cycle
					else if(vis[v]==0) {
						vis[u]=1;
						stack.push(u);
						stack.push(at+1);
						at=gs[v]; u=v;
					} else at++;
				}
			}
		}
	}

	// result in res[]
	void toposort() {
		resn=0; cycle=true;
		Arrays.fill(vis,0);
		for(int i=0;i<n;i++) if(vis[i]<1) dfsvisit(i); // may want to quit here if cycle
	}

	// return the number of strongly connected components
	int scc() {
		vis=new int[n];
		res=new int[n];
		// dfs(G)
		resn=0;
		for(int i=0;i<n;i++) if(vis[i]<1) dfsvisit(i);
		// compute G^T
		int resbak[]=res;
		for(int i=0;i<ne;i++) { int t=from[i]; from[i]=to[i]; to[i]=t; }
		countingsort();
		// call dfs(G^T)
		int nc=0;
		vis=new int[n];
		res=new int[n];
		resn=0;
		for(int i=n-1;i>-1;i--) if(vis[resbak[i]]<1) {
			dfsvisit(resbak[i]);
			nc++;
		}
		return nc;
	}

	/*	usage (the first step common for all algoritms
		read n,m (number of nodes, number of edges)
		create from[], to[], vis[]
		read edges into from[], to[]
		countingsort() to sort edges on from and to create gs[]

		for toposort:	create vis[]
						run toposort
		res[] contains list of the nodes in their backwards topological
		sorted order. res[0]=last node in toposort etc

		for scc:		run scc()
	*/

}