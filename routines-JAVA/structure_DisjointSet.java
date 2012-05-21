import java.math.*;
import java.util.*;

/* 23.04.2009: All these has similar runtime on UVa 11503, Rand was marginally higher, I think */

/* 09.02.2009: SPOJ 3188 OK */
/* 23.04.2009: UVa 11503 OK */
/* 17.10.2009: UVA 11710 OK */
class DisjointSet {
	int n,r[],p[];
	DisjointSet(int N) {
		n=N;
		r=new int[n];
		p=new int[n];
		Arrays.fill(p,-1);
	}
	int root(int n) {
		if(p[n]<0) return n;
		return p[n]=root(p[n]);
	}
	void merge(int x,int y) {
		int a=root(x),b=root(y);
		if(a==b) return;
		if(r[b]>=r[a]) {
			p[a]=b;
			r[b]+=(r[a]==r[b]?1:0);
		} else p[b]=a;
	}
}

class DisjointSetRand {
	int n,p[];
	DisjointSetRand(int N) {
		n=N;
		p=new int[n];
		Arrays.fill(p,-1);
	}
	int root(int n) {
		if(p[n]<0) return n;
		return p[n]=root(p[n]);
	}
	void merge(int x,int y) {
		int a=root(x),b=root(y);
		if(a==b) return;
		if(Math.random()<0.5) p[a]=b;
		else p[b]=a;
	}
}

/* 18.10.2007: SRM 368 div1-500 OK */
/* 11.09.2008: fixed a bug that made findset slower... */
/* 23.04.2009: cleaned up, made interface similar */
class UnionSet {
	int p[],rank[],size;
	public UnionSet(int size) {
		p=new int[size];
		rank=new int[size];
		this.size=size;
		for(int i=0;i<size;i++) p[i]=i;
	}
	void merge(int x,int y) {
		x=root(x); y=root(y);
		if(rank[x]>rank[y]) p[y]=x;
		else {
			p[x]=y;
			if(rank[x]==rank[y]) rank[y]++;
		}
	}
	int root(int x) {
		if(x!=p[x]) p[x]=root(p[x]);
		return p[x];
	}
}

public class structure_DisjointSet {

	public static void main(String a[]){new structure_DisjointSet().go();}
	void go() {
		DisjointSet u=new DisjointSet(10000); // size
	}
}
