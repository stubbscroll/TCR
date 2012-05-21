	int fac[]=new int[20];
	void initfac() {
		fac[0]=1;
		for(int i=1;i<20;i++) fac[i]=fac[i-1]*i;
	}
	int rank(int[] b) {
		int r=0, f=b.length, a[]=b.clone();
		for(int i=0, k=f-1; i<f-1; i++, k--) {
			r+=a[i]*fac[k];
			for(int j=i; j<f; j++) if(a[j]>a[i]) a[j]--;
		}
		return r;
	}
	int[] unrank(int n, int f) {
		int[] r=new int[f], t=new int[f];
		for(int i=0; i<f; i++) t[i]=i;
		for(int i=0, k=f; i<f-1; i++, k--) {
			int v=fac[k-1];
			r[i]=t[n/v%k];
			for(int j=n/v%k+1; j<f; j++) t[j-1]=t[j];
		}
		r[f-1]=t[0];
		return r;
	}
