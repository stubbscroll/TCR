	/* solve Ax=b */
	/* n rows, m columns in A, o columns in b */
	/* return: 0: no solutions, 1: one solution, 2: free variables */
	/* OK UVa 766 25.06.2012 */
	int gauss(BigFraction a[][],int n,int m,int o) {
		int c=0,z=m+o,j;
		boolean fri=false;
		BigFraction zero=new BigFraction(0);
		for(int i=0;i<m;i++) {
			for(j=c;j<n;j++) if(!zero.equals(a[j][i])) break;
			if(j==n) { fri=true; break; }
			/* swap? */
			if(j>c) for(int k=i;k<z;k++) {
				BigFraction t=a[j][k];
				a[j][k]=a[c][k];
				a[c][k]=t;
			}
			/* find multiplicative inverse of a[c][i] */
			BigFraction x=a[c][i].inv();
			for(int k=i;k<z;k++) a[c][k]=a[c][k].mul(x);
			/* subtract multiples of this row */
			for(j=0;j<n;j++) if(j!=c && !zero.equals(a[j][i])) {
				x=a[j][i];
				for(int k=0;k<z;k++) a[j][k]=a[j][k].sub(a[c][k].mul(x));
			}
			c++;
		}
		/* detect no solution: rows with 0=b */
	ok:
		for(int i=0;i<n;i++) {
			for(j=0;j<m;j++) if(!zero.equals(a[i][j])) continue ok;
			for(;j<z;j++) if(!zero.equals(a[i][j])) return 0;
		}
		return 1+(fri?1:0);
	}

	class BigFraction implements Comparable<BigFraction> {
		BigInteger teller;
		BigInteger nevner;
		static BigInteger gcd(BigInteger a,BigInteger b) { return b.compareTo(BigInteger.ZERO)==0?a:gcd(b,a.mod(b)); }
		static BigInteger lcm(BigInteger a,BigInteger b) { return a.divide(gcd(a,b)).multiply(b); }
		BigFraction(long a) { teller=BigInteger.valueOf(a); nevner=BigInteger.ONE; }
		BigFraction(int a) { teller=BigInteger.valueOf(a); nevner=BigInteger.ONE; }
		BigFraction(int a,int b) { teller=BigInteger.valueOf(a); nevner=BigInteger.valueOf(b); }
		BigFraction(BigInteger a) { teller=a; nevner=BigInteger.ONE; }
		BigFraction(BigInteger a, BigInteger b) { teller=a;	nevner=b; }
		BigFraction(BigFraction a) { teller=a.teller; nevner=a.nevner; }
		BigFraction fix() {
			BigFraction res=new BigFraction(this);
			BigInteger g=gcd(res.teller,res.nevner);
			res.teller=res.teller.divide(g);
			res.nevner=res.nevner.divide(g);
			return res;
		}
		BigFraction mul(BigFraction b) {
			return (new BigFraction(teller.multiply(b.teller),nevner.multiply(b.nevner))).fix();
		}
		BigFraction div(BigFraction b) {
			return (new BigFraction(teller.multiply(b.nevner),nevner.multiply(b.teller))).fix();
		}
		BigFraction add(BigFraction b) {
			return (new BigFraction(teller.multiply(b.nevner).add(nevner.multiply(b.teller)),nevner.multiply(b.nevner))).fix();
		}
		BigFraction sub(BigFraction b) {
			return (new BigFraction(teller.multiply(b.nevner).subtract(nevner.multiply(b.teller)),nevner.multiply(b.nevner))).fix();
		}
		BigFraction inv() {
			return new BigFraction(nevner,teller);
		}
		public int compareTo(BigFraction o) {
			BigInteger left=teller.multiply(o.nevner);
			BigInteger right=nevner.multiply(o.teller);
			return left.compareTo(right);
		}
		public boolean equals(BigFraction o) {
			BigInteger left=teller.multiply(o.nevner);
			BigInteger right=nevner.multiply(o.teller);
			return left.equals(right);
		}
		public String toString() {
			if(nevner.compareTo(BigInteger.ONE)==0) return ""+teller;
			else return teller+"/"+nevner;
		}
	}
