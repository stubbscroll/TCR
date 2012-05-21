class NumberTheory {
	/* generate sieve array up to n */
	/* 25.10.2008: Euler 214: OK */
	static boolean[] createsieve(int n) {
		boolean sieve[]=new boolean[n];
		Arrays.fill(sieve,true);
		sieve[0]=sieve[1]=false;
		for(int i=2;i*i<=n;i++) for(int j=i*i;j<n;j+=i) sieve[j]=false;
		return sieve;
	}
	/* generate prime numbers from sieve array */
	/* 25.10.2008: Euler 214: OK */
	static int[] getprimes(boolean sieve[]) {
		int n=0;
		for(int i=0;i<sieve.length;i++) if(sieve[i]) n++;
		int primes[]=new int[n],p=0;
		for(int i=0;i<sieve.length;i++) if(sieve[i]) primes[p++]=i;
		return primes;
	}
	static int gcd(int a,int b) {
		int t;
		while(b!=0) { t=b; b=a%b; a=t; }
		return a;
	}
	/* generate all phi(1)..phi(n) */
	/* 25.10.2008: Euler 214: OK */
	static int phi[];
	static boolean sieve[];
	static int primes[],limit;
	static int[] phigen(int n) {
		phi=new int[n];
		limit=n;
		primes=NumberTheory.getprimes(NumberTheory.createsieve(n));
		phibtr(1,1,0);
		return phi;
	}
	static void phibtr(int n,int t,int p) {
		phi[n]=t;
		while(p<primes.length && (long)n*primes[p]<limit) {
			int n2=n*primes[p],t2=t*(primes[p]-1);
			phibtr(n2,t2,p+1);
			while((long)n2*primes[p]<limit) {
				n2*=primes[p];
				t2*=primes[p];
				phibtr(n2,t2,p+1);
			}
			p++;
		}
	}
	// return the largest k such that num^k divides N!
	public long factorialdiv(long N,long num) {
		long res=0,n=num;
		while(n<=N) { res+=N/n; n*=num; }
		return res;
	}
	// return n^k % mod
	// make a faster routine later
	public int powmod(int n,long k,int mod) {
		int i=62,v=n,ans=1;
		if(k<1) return 1;
		while(i>=0 && 0==(k&(1L<<i))) --i;
		for(int j=0;j<=i;j++) {
			if(0<(k&(1<<j))) ans=(int)((long)ans*v%mod);
			v=(int)((long)v*v%mod);
		}
		return ans;
	}

	// array 0: the factors, array 1: count of each factor
	// routine doesn't require pre-calculated prime numbers
	// SRM 409 div 1 600: OK
	int[][] factorize(int n) {
		int r[][]=new int[2][],add=1,cc;
		ArrayList<Integer> f=new ArrayList<Integer>();
		ArrayList<Integer> c=new ArrayList<Integer>();
		for(int i=2;i*i<=n;i+=add,add=2) if(n%i==0) {
			cc=1; n/=i;
			while(n%i==0) { n/=i; cc++; }
			f.add(i);
			c.add(cc);
		}
		if(n>1) { f.add(n); c.add(1); }
		r[0]=new int[f.size()]; r[1]=new int[f.size()];
		for(int i=0;i<f.size();i++) { r[0][i]=f.get(i); r[1][i]=c.get(i); }
		return r;
	}
	// return the number of times d divides n!
	// SRM 409 div 1 600: OK
	int factorialdiv(int n,int d) {
		int r=0;
		while(n>0) { r+=n/d; n/=d; }
		return r;
	}

}
