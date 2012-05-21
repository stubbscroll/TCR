import java.math.*;
import java.awt.*;
import java.awt.geom.*;

public class Geometry {

	double EPS = 1e-9;

	// line in parametric form: ax+by=c
	// TODO include derivation of conversion formula
	class ParametricLine implements Comparable<ParametricLine> {
		double a,b,c;
		ParametricLine(double A,double B,double C) { a=A; b=B; c=C; }
		// convert (x1,y1)-(x2,y2) line to parametric
		ParametricLine(Line2D.Double L) {
			a=L.y2 - L.y1;
			b=L.x1 - L.x2;
			c=a*L.x1 + b*L.y1;
		}
		public int compareTo(ParametricLine o) {
			if(a<o.a) return -1;
			else if(a>o.a) return 1;
			else if(b<o.b) return -1;
			else if(b>o.b) return 1;
			else if(c<o.c) return -1;
			else if(c>o.c) return 1;
			return 0;
		}
	}
	// OK 27.09.2009 SRM 183 div 1 medium
	// OK 27.09.2009 UVA 11681

	// circle: center (point, p) and radius r
	class Circle implements Comparable<Circle> {
		Point2D.Double p;
		double r;
		Circle(Point2D.Double P, double R) {
			p=P;
			r=R;
		}
		public int compareTo(Circle o) {
			if(r<o.r) return -1;
			else if(r>o.r) return 1;
			else if(p.x<o.p.x) return -1;
			else if(p.x>o.p.x) return 1;
			else if(p.y<o.p.y) return -1;
			else if(p.y>o.p.y) return 1;
			return 0;
		}
	}
	// OK 27.09.2009 SRM 183 div 1 medium
	// OK 27.09.2009 UVA 11681

	// return signed 2*area of triangle, given points
	// accomplished by translating such that point a=(0,0) and using
	// the standard polynomial area formula
	// (equivalent to z-magnitude of cross product)
	double signedtrianglearea2(Point2D.Double a,Point2D.Double b,Point2D.Double c) {
		return (c.x-a.x)*(b.y-a.y)-(b.x-a.x)*(c.y-a.y);
	}
	// OK 27.09.2009 SRM 183 div 1 medium
	// OK 27.09.2009 UVA 11681

	// 2D determinant
	double det2D(double a1,double a2,double b1,double b2) {
		return a1*b2-a2*b1;
	}
	// OK 27.09.2009 SRM 183 div 1 medium
	// OK 27.09.2009 UVA 11681

	// find the intersection point of two lines (not segments)
	// return null if parallel or overlapping
	Point2D.Double intersectlines(ParametricLine a,ParametricLine b) {
		double det=det2D(a.a, b.a, a.b, b.b);
		if(Math.abs(det)<EPS) return null;
		return new Point2D.Double(
			(b.b*a.c - a.b*b.c)/det,
			(a.a*b.c - b.a*a.c)/det
		);
	}
	// OK 27.09.2009 SRM 183 div 1 medium
	// OK 27.09.2009 UVA 11681 but NOT with EPS=1e-9

	// find the line perpendicular to the bisection of a line
	ParametricLine linebisectperp(Line2D.Double L) {
		double dx=L.x1-L.x2, dy=L.y2-L.y1;
		double x2=(L.x1+L.x2)*.5, y2=(L.y1+L.y2)*.5;
		return new ParametricLine(new Line2D.Double(
			x2, y2, x2+dy, y2+dx));
	}
	// OK 27.09.2009 SRM 183 div 1 medium
	// OK 27.09.2009 UVA 11681

	// find circle from 3 points
	Circle circle3p(Point2D.Double a, Point2D.Double b, Point2D.Double c){
		if (0 == signedtrianglearea2(a, b, c)) return null;
		ParametricLine m1 = linebisectperp(new Line2D.Double(a, b));
		ParametricLine m2 = linebisectperp(new Line2D.Double(b, c));
		Point2D.Double ix = intersectlines(m1, m2);
		return new Circle(ix, ix.distance(a));
	}
	// OK 27.09.2009 SRM 183 div 1 medium
	// OK 27.09.2009 UVA 11681

}
