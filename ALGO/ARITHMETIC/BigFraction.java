/*	tested on Project Euler 184 on 24.06.2011 */
	public static class BigFraction implements Comparable<BigFraction> {
		BigInteger a,b;
		public static BigFraction ZERO=new BigFraction(0,1);
		public int compareTo(BigFraction o) {
			return a.multiply(o.b).compareTo(b.multiply(o.a));
		}
		public boolean equals(BigFraction o) {
			return a.equals(o.a) && b.equals(o.b);
		}
		private void minimize() {
			BigInteger g=a.gcd(b);
			if(!g.equals(BigInteger.ONE)) {
				a=a.divide(g); b=b.divide(g);
			}
			if(b.compareTo(BigInteger.ZERO)<0) {
				a=BigInteger.ZERO.subtract(a);
				b=BigInteger.ZERO.subtract(a);
			}
		}
		BigFraction(BigInteger c,BigInteger d) {
			a=c;
			b=d;
			minimize();
		}
		BigFraction(int c,int d) {
			a=BigInteger.valueOf(c);
			b=BigInteger.valueOf(d);
			minimize();
		}
		BigFraction add(BigFraction d) {
			BigInteger x=a.multiply(d.b).add(b.multiply(d.a));
			BigInteger y=b.multiply(d.b);
			return new BigFraction(x,y);
		}
		BigFraction multiply(BigFraction d) {
			return new BigFraction(a.multiply(d.a),b.multiply(d.b));
		}
		BigFraction divide(BigFraction d) {
			return new BigFraction(a.multiply(d.b),b.multiply(d.a));
		}
		BigFraction pow(int n) {
			if(n<0) return new BigFraction(b,a).pow(-n);
			return new BigFraction(a.pow(n),b.pow(n));
		}
		BigFraction subtract(BigFraction d) {
			BigInteger x=a.multiply(d.b).subtract(b.multiply(d.a));
			BigInteger y=b.multiply(d.b);
			return new BigFraction(x,y);
		}
		public String toString() {
			return String.format("%s/%s",a,b);
		}
	}

/* it seems i wrote another version without knowing about the above */
/* OK UVa 766 25.06.2012 */
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
