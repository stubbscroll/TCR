/* stuff that contains circle goes to this file, even if it also
   contains triangles, lines, polygons etc */

#define EPS 1e-6

/* given triangle with side lengths a,b,c, return the
   radius of the inscribed circle */
/* triangle_area=radius*perimeter*.5 */
/* OK project euler 195 11.11.2012 */
double trirad(double a,double b,double c) {
	double k=(a+b+c)*.5;
	return sqrt(k*(k-a)*(k-b)*(k-c))/k;
}

/* given three circle radii, find radius of enclosed fourth circle */
/* radius can be negative! */
/* OK project euler 199 04.08.2011 */
double enclosedcircle(double r1,double r2,double r3) {
  return 1/(1/r1 + 1/r2 + 1/r3 + 2*sqrt(1/(r1*r2)+1/(r1*r3)+1/(r2*r3)));
}

/* subroutine used in circletangent and circlecircleintersection */
double dist2d(double x1,double y1,double x2,double y2) {
	double dx=x1-x2,dy=y1-y2;
	return sqrt(dx*dx+dy*dy);
}

/* subroutine for calculating tangent to two circles */
void twocircletangentsub(double x1,double y1,double r1,double x2,double y2,double r2,double *tx1,double *ty1,double *tx2,double *ty2) {
	double a,b,d=dist2d(x1,y1,x2,y2),x=x2-x1,y=y2-y1,R,X,Y;
	double r=r2-r1;
	X=x/d; Y=y/d; R=r/d;
	/* calculate a,b,c so that line is ax+by+c=0 */
	a=R*X-Y*sqrt(1.-R*R);
	b=R*Y+X*sqrt(1.-R*R);
	/* follow the line's normal from the circle centres */
	*tx1=x1-a*r1;
	*ty1=y1-b*r1;
	*tx2=x2-a*r2;
	*ty2=y2-b*r2;
}

/* given two circles and sides (0:left, 1:right, side1:first circle,
   side 2:last circle), find their tangent and return the two points
   where the tangent touches the circle in (x1,y1) and (x2,y2) */
/* http://en.wikipedia.org/wiki/Tangent_lines_to_circles */
/* OK IDI Open 2012 problem H (Holey Road) 21.05.2012 */
/* OK Project Euler 246 18.11.2012 */
void twocircletangent(double x1,double y1,double r1,double x2,double y2,double r2,int side1,int side2,double *tx1,double *ty1,double *tx2,double *ty2) {
	if(side1+side2==2) return twocircletangentsub(x1,y1,r1,x2,y2,r2,tx1,ty1,tx2,ty2);
	else if(side1+side2==0) twocircletangentsub(x2,y2,r2,x1,y1,r1,tx2,ty2,tx1,ty1);
	else if(side2==1) twocircletangentsub(x1,y1,-r1,x2,y2,r2,tx1,ty1,tx2,ty2);
	else if(side1==1) twocircletangentsub(x1,y1,r1,x2,y2,-r2,tx1,ty1,tx2,ty2);
}

/* given point px,py and ellipse with center x,y, semi-axes a,b and side
   (0 left, 1 right), return tangent to ellipse that goes through point in
   *tx,*ty. uses twocircletangent(), twocircletangentsub(), dist2d() by
   scaling the x-axis to transform ellipse into a circle */
/* OK Project Euler 246 18.11.2012 */
void ellipsetangent(double px,double py,double x,double y,double a,double b,int side,double *tx,double *ty) {
	double s=b/a,dx,dy;
	twocircletangent(px*s,py,0.,x*s,y,b,0,side,&dx,&dy,tx,ty);
	*tx/=s;
}

/* TODO make a variant that returns intersection points */
/* return 1 if line intersects or touches circle, 0 otherwise */
/* algorithm shamelessly borrowed from http://stackoverflow.com/questions/1073336/circle-line-collision-detection */
/* OK IDI Open 2012 problem H (Holey Road) 21.05.2012 */
int linesegmentcircleintersect(double x1,double y1,double x2,double y2,double cx,double cy,int r) {
	double dx=x2-x1,dy=y2-y1;
	double fx=x1-cx,fy=y1-cy;
	double a=dx*dx+dy*dy;
	double b=2*(fx*dx+fy*dy);
	double c=fx*fx+fy*fy-r*r;
	double D=b*b-4*a*c;
	double t1,t2;
	/*	no collision */
	if(D<0) return 0;
	/*	check if line segment actually intersects or touches */
	D=sqrt(D);
	t1=(-b+D)/(2*a);
	t2=(-b-D)/(2*a);
	return (t1>=0 && t1<=1) || (t2>=0 && t2<=1);
}

/*	check if endless line intersects circle, return 1 if yes, 0 if no */
/*	OK UVa 12454 0.232 seconds 22.05.2012 */
int linecircleintersect(double x1,double y1,double x2,double y2,double cx,double cy,double r) {
	double dx=x2-x1,dy=y2-y1;
	double fx=x1-cx,fy=y1-cy;
	double a=dx*dx+dy*dy;
	double b=2*(fx*dx+fy*dy);
	double c=fx*fx+fy*fy-r*r;
	double D=b*b-4*a*c;
	return D>=0;
}

/* equality check used in circlecircleintersection */
double deq(double a,double b) {
	return fabs(a-b)<EPS;
}

/* circle-circle intersection! return:
   0 if no intersection
   1 if circle is contained in other circle
   2 if circles are the same (infinitely many intersection points
   3 and *x3 *y3 *x4 *y4: intersections (points 3 and 4 can be equal)
   http://local.wasp.uwa.edu.au/~pbourke/geometry/2circle/
*/
/* OK UVa 453 07.06.2012 */
int circlecircleintersect(double x0,double y0,double r0,double x1,double y1,double r1,
		double *x3,double *y3,double *x4,double *y4) {
	double d=dist2d(x0,y0,x1,y1),a,h,x2,y2;
	if(d>r0+r1+EPS) return 0;
	if(d+EPS<fabs(r0-r1)) return 1;
	if(deq(d,0) && deq(r0,r1)) {
		/* check nasty case where circles are equal with r=0 */
		if(!deq(r0,0)) return 2;
		*x3=*x4=x0;
		*y3=*y4=y0;
		return 3;
	}
	a=(r0*r0-r1*r1+d*d)/(2*d);
	h=r0*r0-a*a<EPS?0:sqrt(r0*r0-a*a);
	x2=x0+a*(x1-x0)/d;  y2=y0+a*(y1-y0)/d;
	*x3=x2+h*(y1-y0)/d; *y3=y2-h*(x1-x0)/d;
	*x4=x2-h*(y1-y0)/d; *y4=y2+h*(x1-x0)/d;
	return 3;
}
