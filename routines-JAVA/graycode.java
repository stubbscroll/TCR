/* generate all gray codes from 0 to n-1 in O(n) time */
/* NB! the while loop has amortized run time O(1)! this routine is not slow! */
/* proof: 1/2 of the time the body runs 1 times
          1/4 of the time the body runs 2 times
          1/8 of the time the body runs 3 times
          ...
          1/256 of the time the body runs 8 times
   the sum is 1! */

public class graycode {
	int[] generate(int n) {
		int ret[]=new int[n];
		for(int i=1;i<n;i++) {
			int pos=0,v=i;
			while((v&1)<1) { v>>=1; pos++; }
			ret[i]=ret[i-1]^(1<<pos);
		}
		return ret;
	}
	public static void main(String a[]) {
		new graycode().entry();
	}
	void entry() {
		int N=256;
		int r[]=generate(N);
		for(int i=0;i<N;i++) System.out.printf("%3d %8s\n",i,Integer.toBinaryString(r[i]));

		// even easier:
		for(int i=0;i<N;i++) System.out.printf("%3d %8s\n",i,Integer.toBinaryString(i^(i>>1));
	}
}