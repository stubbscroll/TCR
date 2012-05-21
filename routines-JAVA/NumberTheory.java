import java.util.*;

public class NumberTheory {

	// return boolean array: for indices [2,n):
	// false: prime, true: not prime
	// OK 07.02.2009 euler 231
	boolean[] createsieve(int n) {
		boolean s[] = new boolean[n];
		for(int i=2; i*i<n; i++) if(!s[i]) for(int j=i*i; j<n; j+=i) s[j]=true;
		return s;
	}

	// generate list of prime numbers from sieve array
	// OK 07.02.2009 euler 231
	int[] getprimes(boolean s[]) {
		int n=0;
		for(int i=2; i<s.length; i++) if(!s[i]) n++;
		int p[]=new int[n], q=0;
		for(int i=2; i<s.length; i++) if(!s[i]) p[q++]=i;
		return p;
	}

	// factorize n, self-contained, slow-ish
	// returns: array 0: the factors, array 1: count of each factor
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

	// TODO factorize n, use sieve/primes array

	// TODO number of divisors of n, fast

	// TODO number of divisors of n, slow+short

	// TODO sum of divisors of n, fast

	// TODO sum of divisors of n, slow+short

	// return all divisors of n
	// sorting is optional
	void divbtr(int p,int v,int f[][], ArrayList<Integer> divs) {
		if(p==f[0].length) { divs.add(v); return; }
		for(int i=0; i<=f[1][p]; i++) {
			divbtr(p+1, v, f, divs);
			v *= f[0][p];
		}
	}

	int[] divisors(int n) {
		int f[][] = factorize(n);
		ArrayList<Integer> divs = new ArrayList<Integer>();
		divbtr(0, 1, f, divs);
		Collections.sort(divs);
		int r[] = new int[divs.size()],p=0;
		for(Integer e:divs) r[p++] = e;
		return r;
	}

	// TODO miller-rabin primality test (deterministic)

	// TODO factorize n using pollard's rho (brent), including wrapper

	int gcd(int a,int b) {
		int t;
		while(b!=0) { t=b; b=a%b; a=t; }
		return a;
	}
	// extended euclid, find x,y such that gcd(a,b)=xa+yb
	// take from factorization and primality testing, chapter 1
	int x,y;
	int extendedeuclid(int a,int b) {
		int[] u=new int[]{1,0,a},v=new int[]{0,1,b};
		while(v[2]!=0) {
			int q=u[2]/v[2];
			for(int i=0;i<3;i++) { int t=v[i]; v[i]=u[i]-q*v[i]; u[i]=t; }
		}
		x=u[0]; y=u[1];
		return u[2];
	}

	// VIKTIG gjør ting i denne rekkefølgen for å unngå overflow!
	// gcd(a,b) vil dele a (pr def), så dette er trygt!
	int lcm(int a,int b) {
		return a/gcd(a,b)*b;
	}

	// find value such that a * value = 1 (mod b)
	int inverse(int a,int b) {
		int[] u=new int[]{1,0,a},v=new int[]{0,1,b};
		while(v[2]!=0) {
			int q=u[2]/v[2];
			for(int i=0;i<3;i++) {
				int t=v[i]; v[i]=u[i]-q*v[i]; u[i]=t;
			}
		}
		return (u[0]%b+b)%b;
	}

	// multiply two longs
	long longmulmod(long a,long b,long mod) {
		long res=0;
		while(a>0) {
			if((a&1)>0) res=(res+b)%mod;
			a>>=1;
			b=(b<<1)%mod;
		}
		return res;
	}

	// find value such that a * value = 1 (mod b), long version
	long longinverse(long a,long b) {
		long[] u=new long[]{1,0,a},v=new long[]{0,1,b};
		while(v[2]!=0) {
			long q=u[2]/v[2];
			for(int i=0;i<3;i++) {
				long t=v[i]; v[i]=u[i]-longmulmod(q,v[i],b); u[i]=t;
			}
		}
		return (u[0]%b+b)%b;
	}


	// TODO return phi(n)

	// generate all phi(1), ..., phi(n-1)
	// call phigen
	// TODO test on euler 214
	int phi[];
	boolean sieve[];
	int primes[],limit;
	int[] phigen(int n) {
		phi=new int[n];
		limit=n;
		primes=getprimes(createsieve(n));
		phibtr(1,1,0);
		return phi;
	}
	void phibtr(int n,int t,int p) {
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
	public long factorialpowdiv(long N,long num) {
		long res=0,n=num;
		while(n<=N) { res+=N/n; n*=num; }
		return res;
	}

	// return the number of times d divides n!
	// OK 07.02.2009 euler 231
	public int factorialdiv(int n,int d) {
		int r=0;
		while(n>0) { r+=n/d; n/=d; }
		return r;
	}

	// return n^k % mod
	// make a faster/better/shorter routine later
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

	// TODO convert sqrt(n) to continued fraction

	// TODO convert fraction to continued fraction

	// TODO convert continued fraction to fraction

	// TODO solve pell's equation x^2-Dy^2=1, where D is squarefree

	// TODO pythagorean triples

	// TODO chinese remainder theorem

	// digit sum
	// uncomment to get recursive digitsum, always <10
	int digitsum(int n) {
		int r=0;
		while(n>0) { r+=r&10; n/=10; }
	//	if(r>9) return digitsum(r);
		return r;
	}

	public static void main(String args[]) {
		new NumberTheory().go();
	}
	void go() {
		int d=extendedeuclid(123,344);
		System.out.printf("%d = %d * %d + %d * %d\n",d,x,123,y,344);
		int p[]=getprimes(createsieve(1000));
		for(int i=0;i<p.length;i++)System.out.println(p[i]);
	}
}
