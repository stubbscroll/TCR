import java.io.*;
import java.math.*;
import java.util.*;

/* 15.01.2010: SGU 517 OK */

/*	the function tatami(n,m) calculates the number of tatami tilings for an n*m rectangle.
	a tatami tiling of a rectangle is a tiling of 1x2 and 2x1 pieces such that no four
	pieces share a corner. */

public class Combinatorics_Tilings {
	BufferedReader in=new BufferedReader(new InputStreamReader(System.in));
	StringTokenizer st=new StringTokenizer("");
	String LINE() throws Exception { return in.readLine(); }
	String STR() throws Exception {
		while(!st.hasMoreTokens()) st=new StringTokenizer(LINE());
		return st.nextToken();
	}
	int INT() throws Exception { return Integer.parseInt(STR()); }
	long LONG() throws Exception { return Long.parseLong(STR()); }
	double DOUBLE() throws Exception { return Double.parseDouble(STR()); }
	String PD(double d,int n) {
		return String.format("%."+n+"f",d).replace(",",".");
	}

	public static void main(String args[]) throws Exception {
		new Combinatorics_Tilings().go();
	}

	BigInteger f[]=new BigInteger[3001];

	void fac() {
		f[0]=BigInteger.ONE;
		for(int i=1;i<3001;i++) f[i]=f[i-1].multiply(new BigInteger(""+i));
	}
	BigInteger binom(int n,int m) {
		if(m>n || m<0) return BigInteger.ZERO;
		return f[n].divide(f[m]).divide(f[n-m]);
	}
	public void go() throws Exception {
		fac();
		int m=INT(),n=INT();
//		for(int n=0;n<1001;n++)for(int m=0;m<1001;m++) System.out.println(n+" "+m+": "+tatami(n,m));
		System.out.println(tatami(n,m));
	}
	BigInteger TWO=BigInteger.ONE.add(BigInteger.ONE);
	BigInteger tatami(int n,int m) {
		if(n*m%2==1) return BigInteger.ZERO;
		if(m>n) { int t=n; n=m; m=t; }
		if(m==1) return BigInteger.ONE;
		if(m==0) return BigInteger.ZERO;
		BigInteger res=BigInteger.ZERO;
		if(m==2) for(int j=0;j<=n-2*j+1;j++) res=res.add(binom(n-2*j+1,j));
		else if(m%2==1) {
			for(int j=0;j<=(n-2*j)/(m-1);j++) if((n-2*j)%(m-1)==0) res=res.add(binom((n-2*j)/(m-1),j).multiply(TWO));
		} else for(int j=0;;j++) {
			boolean done=true;
			if(j<=(n-2*j)/(m-1)) {
				done=false;
				if((n-2*j)%(m-1)==0) res=res.add(binom((n-2*j)/(m-1),j).multiply(TWO));
			}
			if(j<=(n-2*j-1)/(m-1)) {
				done=false;
				if((n-2*j-1)%(m-1)==0) res=res.add(binom((n-2*j-1)/(m-1),j));
			}
			if(j<=(n-2*-m+2)/(m-1)) {
				done=false;
				if((n-2*j-m+2)%(m-1)==0) res=res.add(binom((n-2*j-m+2)/(m-1),j));
			}
			if(j<=(n-2*j-m)/(m-1)) {
				done=false;
				if((n-2*j-m)%(m-1)==0) res=res.add(binom((n-2*j-m)/(m-1),j));
			}
			if(done) break;
		}
		return res;
	}

	/* OK 15.01.2010: Project Euler 256 */

	/* return true if no tatami tilings exist of an n*m rectangle */
	boolean haszero(int n,int m) {
		if(m>n) { int t=n; n=m; m=t; }
		if(m==1) return false;
		if(m==0) return true;
		if(m==2) return false;
		if(m%2==1) {
			if(n*m%2==1) return true;
			for(int j=0;j<=(n-2*j)/(m-1);j++) if((n-2*j)%(m-1)==0) return false;
			return true;
		} else {
			for(int j=0;;j++) {
				boolean done=true;
				if(j<=(n-2*j)/(m-1)) {
					done=false;
					if((n-2*j)%(m-1)==0) return false;
				}
				if(j<=(n-2*j-1)/(m-1)) {
					done=false;
					if((n-2*j-1)%(m-1)==0) return false;
				}
				if(j<=(n-2*-m+2)/(m-1)) {
					done=false;
					if((n-2*j-m+2)%(m-1)==0) return false;
				}
				if(j<=(n-2*j-m)/(m-1)) {
					done=false;
					if((n-2*j-m)%(m-1)==0) return false;
				}
				if(done) break;
			}
			return true;
		}
	}
}
