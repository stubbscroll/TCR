/* Range minimum query <O(n),O(log n)> */
/* 07.11.2007: algbio event OK */

class RMQ {
	int height[], log, n2, a[];
	public RMQ(int height[]) {
		int n = height.length; log = 1;
		for(int i = 1; i < n; i *= 2, log++);
		n2 = (n-1) / log * log + log;
		a = new int[n2];
		this.height = new int[n2];
		for(int i = 0; i < n; i++) this.height[i] = height[i];
		for(int i = 0; i < n2; i += log) {
			int min = i;
			for(int j = 1; j < log; j++)
				if(this.height[min] > this.height[i+j]) min = i+j;
			a[i] = min;
		}
		for(int i=1, diff=log; i<log; i++, diff*=2) for(int j = 0; j<n2-diff; j+=log) {
			if(this.height[a[i+j-1]] <= this.height[a[i+j+diff-1]])
				a[i+j] = a[i+j-1];
			else a[i+j] = a[i+j+diff-1];
		}
	}
	/* return the index of the lowest value in [a..b) */
	public int query(int x,int y) {
		int x1 = x, y1 = ++y;
		if(x % log > 0) x1 += log - x % log;
		if(y % log > 0) y1 -= y % log;
		if(x1 + log >= y1) return brute(x,y);
		int start = (x1==x ? x1 : brute(x,x1));
		int end = (y1==y ? y1-1 : brute(y1,y));
		int inc = 0,p = log,d = y1-x1;
		while(d > p+p) { p <<= 1; inc++; }
		int m1 = a[x1+inc], m2 = a[y1-p+inc];
		if(height[start] > height[m1]) start = m1;
		if(height[start] > height[m2]) start = m2;
		if(height[start] > height[end]) start = end;
		return start;
	}
	public int brute(int a,int b) {
		int min=a;
		for(a++;a<b;a++) if(height[min]>height[a]) min=a;
		return min;
	}
}
