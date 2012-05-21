	public int[] generatefactorial(int n)
	{
		int fact[]=new int[n];
		fact[0]=1;
		for(int i=1;i<n;i++) fact[i]=i*fact[i-1];
		return fact;
	}

	int fact[]=generatefactorial(10);

	// calculate rank of permutation (in string), 1-based
	// (only distinct elements)
	public int rank(String s) {
		int n=s.length(),v=0,a[]=new int[n];
		for(int i=0;i<n;i++) a[i]=s.charAt(i)-49;
		for(int i=0;i<n;i++) {
			v+=fact[n-i-1]*a[i];
			for(int j=i+1;j<n;j++) if(a[j]>a[i]) a[j]--;
		}
		return v;
	}

	// calculate permutation with rank v and length n, return 1-based string
	public String unrank(int v,int n) {
		char s[]=new char[n];
		boolean done[]=new boolean[n];
		for(int i=0;i<n;i++) {
			int div=v/fact[n-i-1];
			v-=div*fact[n-i-1];
			for(int j=0;j<n;j++) {
				if(done[j]) continue;
				if(div--==0) {
					s[i]=(char)(j+49);
					done[j]=true;
					break;
				}
			}
		}
		return new String(s);
	}
