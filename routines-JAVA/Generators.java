// combinatorial generators and rank/unrank
public class Generators {
	// generate next permutation (String)
	// return "" if no successor
	String nextpermutation(String s) {
		char a[] = s.toCharArray();
		int n = a.length, j = n-2;
		while(j >= 0 && a[j] >= a[j+1]) j--;
		if(j<0) return "";
		int l = n-1, k = j+1;
		while(a[j] >= a[l]) l--;
		char t = a[j]; a[j] = a[l]; a[l]=t;
		for(l = n-1; k < l; k++, l--) {
			t = a[k]; a[k] = a[l]; a[l] = t;
		}
		return new String(a);
	}

	// generate next permutation (int[])
	// return false if no successor
	boolean nextpermutation(int[] a) {
		int n = a.length, j = n-2;
		while(j >= 0 && a[j] >= a[j+1]) j--;
		if(j<0) return false;
		int l = n-1, k = j+1;
		while(a[j] >= a[l]) l--;
		int t = a[j]; a[j] = a[l]; a[l] = t;
		for(l = n-1; k < l; k++, l--) {
			t = a[k]; a[k] = a[l]; a[l] = t;
		}
		return true;
	}

	// TODO permutation rank
	// TODO permutation unrank
	// side 55-56 i combinatorial algorithms generation

	// TODO minimal change permutation successor (trotter-johnson)
	// TODO trotter-johnson rank
	// TODO trotter-johnson unrank
	// side 57-63 i combinatorial algorithms generation
	// evt se knuth 7.2.noe for en enklere algoritme

	// generate next k-subset, don't call with input 0
	// generate next number with the same number of bits set
	// return 0 if no successor
	int nextksubset(int x) {
		int s = x & -x, r = x+s, o = x^r;
		o = (o>>2)/s;
		return r|o;
	}

	// TODO ksubsetlexrank
	// TODO ksubsetlexunrank
	// side 44-45 i combinatorial algorithms generation

	// generate all submasks (except 0) of a bitmask
	void allsubmasks(int mask) {
		for(int sub = mask; sub > 0; sub = (sub-1) & mask) {
			// visit submask sub
			System.out.printf("%4s\n",Integer.toString(sub,2));
		}
		// visit 0 separately if needed
	}

	// generate next n-tuple
	// return false if no successor
	boolean nextntuple(int a[],int lim[]) {
		int p = a.length-1;
		while(true) {
			a[p]++;
			if(a[p] == lim[p]) {
				a[p--] = 0;
				if(p < 0) return false;
			} else break;
		}
		return true;
	}

	// generate all unordered partitions of an integer: 4 (4, 3-1, 2-2, 2-1-1, 1-1-1-1)
	// TODO: make shorter (slower) version
	void allpartitions(int n) {
		int a[] = new int[n], m = 0, q, x;
		while(true) {
			a[m] = n;
			q = m-(n==1?1:0);
			while(true) {
				// visit a[0], ... , a[m]
				for(int i=0;i<=m;i++) System.out.print(a[i]+" "); System.out.println();
				if(q<0 || a[q] != 2) break;
				a[q--] = 1;
				a[++m] = 1;
			}
			if(q<0) break;
			x = a[q]-1;
			a[q] = x;
			n = m-q+1;
			m = q+1;
			while(n > x) {
				a[m++] = x;
				n -= x;
			}
		}
	}

	// TODO partition rank
	// TODO partition unrank
	// side 77-78 i combinatorial algorithms generation

	// generate all unordered partitions of an integer into m parts, m>1
	void allmparts(int n,int m) {
		int a[] = new int[m+1], j, s, x;
		a[0] = n-m+1;
		a[m] = -1;
		for(int i = 1; i<m; i++) a[i] = 1;
		while(true) {
			// visit a[0], ..., a[m-1]
			for(int i=0;i<m;i++) System.out.printf("%d%c",a[i],i==m-1?'\n':' ');
			if(a[1] < a[0]-1) {
				a[0]--;
				a[1]++;
				continue;
			}
			j = 2;
			s = a[0]+a[1]-1;
			while(a[j] > a[0]-2) s+=a[j++];
			if(j > m-1) break;
			x = a[j]+1;
			a[j--] = x;
			while(j>0) {
				a[j--] = x;
				s -= x;
			}
			a[0]=s;
		}
	}
	// shorter, slightly slower
	void allmparts2(int n,int m) {
		int a[] = new int[m+1], j, s, x;
		a[0] = n-m+1; a[m] = -1;
		for(int i = 1; i<m; i++) a[i] = 1;
		while(true) {
			// visit a[0], ..., a[m-1]
			for(int i=0;i<m;i++) System.out.printf("%d%c",a[i],i==m-1?'\n':' ');
			j = 1; s = a[0]-1;
			while(a[j] > a[0]-2) s+=a[j++];
			if(j > m-1) break;
			x = a[j]+1; a[j--] = x;
			while(j>0) { a[j--] = x; s -= x; }
			a[0]=s;
		}
	}

	// generate all unordered partitions of a set
	// 123, 12|3, 13|2, 1|23, 1|2|3 is represented by 000, 001, 010, 011, 012
	void allpartset(int n) {
		int a[] = new int[n], b[] = new int[n], m = 1, j;
		for(int i = 0; i<n; i++) b[i] = 1;
		while(true) {
			// visit a[0], ..., a[n-1]
			for(int i=0;i<n;i++) System.out.printf("%d%c",a[i],i==n-1?'\n':' ');
			if(a[n-1]!=m) {
				a[n-1]++;
				continue;
			}
			for(j = n-2; a[j]==b[j]; j--);
			if(j < 1) break;
			a[j]++;
			m = b[j]+(a[j]==b[j]?1:0);
			j++;
			while(j < n-1) {
				a[j] = 0;
				b[j++] = m;
			}
			a[n-1]=0;
		}
	}

	// TODO generate all unordered partitions of a set containing m partitions
	// knuth 7.2.1.5 exercise 17

	// TODO generate all partitions of a multiset

	// generate nth gray code
	int graycode(int i) { return i^(i>>1); }

	// given gray code, return its index
	int grayindex(int g) {
		int b = g^(g>>1);
		b = b^(b>>2);
		b = b^(b>>4);
		b = b^(b>>8);
		return b^(b>>16);
	}

	// TODO check knuth 7.2.1.x to find more generators

	// TODO check combinatorial algorithms generation for even more generators

	// example usage
	public static void main(String a[]) {
		new Generators().go();
	}
	void go() {
		int x1[]={1,2,2,5};
		do {
			for(int i=0;i<x1.length;i++) System.out.printf("%d%c",x1[i],i==x1.length-1?'\n':' ');
		} while(nextpermutation(x1));

		String x2="123x";
		do {
			System.out.println(x2);
		} while(!(x2=nextpermutation(x2)).equals(""));

		int x3[]={0,0,0},l3[]={2,2,3};
		do {
			for(int i=0;i<x3.length;i++) System.out.printf("%d%c",x3[i],i==x3.length-1?'\n':' ');
		} while(nextntuple(x3,l3));

		allsubmasks(11);

		allpartitions(7);

		allmparts(11,4);
		allmparts2(11,4);

		allpartset(4);

		for(int i=0;i<16;i++) System.out.printf("%4s -> %4s (%4s)\n",Integer.toString(i,2),Integer.toString(graycode(i),2),Integer.toString(grayindex(graycode(i)),2));
	}
}
