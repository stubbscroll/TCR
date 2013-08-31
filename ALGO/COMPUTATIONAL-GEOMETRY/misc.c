/* find intersection point between two infinite line segments.
   return 0: parallel
   return 1: intersect, point returned as *x,*y */
int lineline(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double *x,double *y) {
	double a1=y2-y1,b1=x1-x2,c1=a1*x1+b1*y1;
	double a2=y4-y3,b2=x3-x4,c2=a2*x3+b2*y3;
	double d=a1*b2-a2*b1;
	if(fabs(d)<EPS) return 0;
	*x=(b2*c1-b1*c2)/d;
	*y=(a1*c2-a2*c1)/d;
	return 1;
}

/* get circle from 3 points */
void getcircle3(double *x,double *y,double *cx,double *cy,double *r) {
	double x1=(x[2]+x[0])*.5,y1=(y[2]+y[0])*.5,x2=(x[1]+x[0])*.5,y2=(y[1]-y[0])*.5,a,b;
	lineline(x1,y1,x1+y[2]-y[0],y1+x[2]-x[0],x2,y2,x2+y[1]-y[0],y2+x[1]-x[0],&a,&b);
	*cx=a; *cy=b;
	*r=sqrt((a-x[0])*(a-x[0])+(b-y[0])*(b-y[0]));
}

/* get inscribed circle from triangle */
/*
   http://www.mathalino.com/reviewer/derivation-of-formulas/derivation-of-formula-for-radius-of-incircle
   r=A/s
   where A is triangle area and s is circumference*0.5.
   find center of circle: the point where the angular bisectors meet.
*/
