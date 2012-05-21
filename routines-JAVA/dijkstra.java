

	/* definer en klasse for node, compareTo må sortere på dist! */

	int dijkstra() {
		TreeSet<Node> q=new TreeSet<Node>();
		Node z[]=new Node[n];
		- for alle noder i z: sett z[start].dist=0, resten til INF
		- for alle noder i z: q.add(z[i])

		while(q.size()>0) {
			Node u=q.first();
			if u er avslutt return u.dist
			q.remove(u);
			- for alle v som grenser til u:
				int d=u.dist + dist(u,v);
				if(d<z[v].dist) {
					q.remove(z[v]);
					z[v].dist=d;
					q.add(z[v]);
				}
			}
			
		}
		return z[slutt].dist;
	}
