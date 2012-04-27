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
