	BigInteger sqrt(BigInteger n) {
		double v=0;
		String s=n.toString();
		for(int i=0;i<s.length();i++) v=v*10+s.charAt(i)-48;
		v=Math.floor(Math.sqrt(v));
		BigInteger a=BigInteger.valueOf((long)v),xnew=a,xold;
		do {
			xold=xnew;
			xnew=xold.add(n.divide(xold)).divide(BigInteger.valueOf(2));
		} while(xnew.compareTo(xold)!=0);
		return xnew;
	}
