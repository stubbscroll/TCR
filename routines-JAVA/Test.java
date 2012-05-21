import java.math.*;
public class Test {
	public static void main(String[]a) {
		double x=0.0/0.0;
		double t=Math.atan(x);
		double X=Math.cos(t);
		double Y=Math.sin(t);
		System.out.printf("%f %f %f %f\n",x,t,X,Y);
	}
}