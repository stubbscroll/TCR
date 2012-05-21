/* union set-find */

/* 18.10.2007: SRM 368 div1-500 OK */
/* 11.09.2008: fixed a bug that made findset slower... */

class UnionSet {
	public int p[],rank[],size;
	public UnionSet(int size) {
		p=new int[size];
		rank=new int[size];
		this.size=size;
		for(int i=0;i<size;i++) p[i]=i;
	}
	private void linkset(int x,int y) {
		if(rank[x]>rank[y]) p[y]=x;
		else {
			p[x]=y;
			if(rank[x]==rank[y]) rank[y]++;
		}
	}
	public int findset(int x) {
		if(x!=p[x]) p[x]=findset(p[x]);
		return p[x];
	}
	public void unionset(int x,int y) {
		linkset(findset(x),findset(y));
	}
	public int countSets() {
		TreeSet<Integer> tree = new TreeSet<Integer>();
		for(int i=0;i<size;i++) tree.add(findset(i));
		return tree.size();
	}
}
