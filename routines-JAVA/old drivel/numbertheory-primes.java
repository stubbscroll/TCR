	// generate a boolean array such that b[i] is true for all prime numbers i in [0,n-1]
	boolean[] createsieve(int n) {
		boolean p[]=new boolean[n];
		Arrays.fill(p,true);
		p[0]=p[1]=false;
		for(int i=2;i*i<=n;i++) if(p[i]) for(int j=i*i;j<n;j+=i) p[j]=false;
		return p;
	}

	// given a boolean array, return an int array of all indices where b[i]=true
	int[] primelist(boolean b[]) {
		int n=b.length,c=0;
		for(int i=0;i<n;i++) if(b[i]) c++;
		int r[]=new int[c];
		c=0;
		for(int i=0;i<n;i++) if(b[i]) r[c++]=i;
		return r;
	}
