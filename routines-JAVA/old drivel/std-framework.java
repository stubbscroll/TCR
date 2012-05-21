import java.io.*;
import java.math.*;
import java.util.*;

public class p1 {
	BufferedReader in=new BufferedReader(new InputStreamReader(System.in));
	StringTokenizer st=new StringTokenizer("");
	int INT(Object o) { return Integer.parseInt(o.toString()); }
	String L() throws Exception { return in.readLine(); }
	String STR() throws Exception {
		while(!st.hasMoreTokens()) st=new StringTokenizer(L());
		return st.nextToken();
	}
	int INT() throws Exception { return Integer.parseInt(STR()); }
	double DBL() throws Exception { return Double.parseDouble(STR()); }
	String PD(double d,int n) {
		return String.format("%."+n+"f",d).replace(",",".");
	}
	BigInteger BIG() throws Exception { return new BigInteger(STR()); }
	BigInteger ZERO=BigInteger.ZERO;
	BigInteger ONE=BigInteger.ONE;
	BigInteger TWO=BigInteger.valueOf(2);

	public static void main(String args[]) throws Exception {
		new p1().entry();
	}

	public void entry() throws Exception {
	}
}
