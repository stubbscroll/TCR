
/*  given three circle radii, find radius of enclosed fourth circle */
/*  radius can be negative! */
/*  OK project euler 199 04.08.2011 */
double enclosedcircle(double r1,double r2,double r3) {
  return 1/(1/r1 + 1/r2 + 1/r3 + 2*sqrt(1/(r1*r2)+1/(r1*r3)+1/(r2*r3)));
}

/*	subroutine used in circletangent */
double dist2d(double x1,double y1,double x2,double y2) {
	double dx=x1-x2,dy=y1-y2;
	return sqrt(dx*dx+dy*dy);
}

/*	subroutine for calculating tangent to two circles */
void twocircletangentsub(double x1,double y1,int r1,double x2,double y2,int r2,double *tx1,double *ty1,double *tx2,double *ty2) {
	double a,b,d=dist2d(x1,y1,x2,y2),x=x2-x1,y=y2-y1,R,X,Y;
	int r=r2-r1;
	X=x/d; Y=y/d; R=r/d;
	/*	calculate a,b,c so that line is ax+by+c=0 */
	a=R*X-Y*sqrt(1-R*R);
	b=R*Y+X*sqrt(1-R*R);
	/*	follow the line's normal from the circle centres */
	*tx1=x1-a*r1;
	*ty1=y1-b*r1;
	*tx2=x2-a*r2;
	*ty2=y2-b*r2;
}

/*	given two circles and sides (0:left, 1:right, side1:first circle,
		side 2:last circle), find their tangent and return the two points
		where the tangent touches the circle in (x1,y1) and (x2,y2) */
/*	http://en.wikipedia.org/wiki/Tangent_lines_to_circles */
/*	OK IDI Open 2012 problem H (Holey Road) 21.05.2012 */
void twocircletangent(double x1,double y1,int r1,double x2,double y2,int r2,int side1,int side2,double *tx1,double *ty1,double *tx2,double *ty2) {
	if(side1+side2==2) return twocircletangentsub(x1,y1,r1,x2,y2,r2,tx1,ty1,tx2,ty2);
	else if(side1+side2==0) twocircletangentsub(x2,y2,r2,x1,y1,r1,tx2,ty2,tx1,ty1);
	else if(side2==1) twocircletangentsub(x1,y1,-r1,x2,y2,r2,tx1,ty1,tx2,ty2);
	else if(side1==1) twocircletangentsub(x1,y1,r1,x2,y2,-r2,tx1,ty1,tx2,ty2);
}

/*	return 1 if line intersects or touches circle, 0 otherwise */
/*	algorithm shamelessly borrowed from http://stackoverflow.com/questions/1073336/circle-line-collision-detection */
/*	OK IDI Open 2012 problem H (Holey Road) 21.05.2012 */
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
