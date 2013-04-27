

/*	calculate triangle area given side lengths. this is the
		slightly more numerically stable version */
/*	OK UVa 10347 0.008 seconds 26.05.2012 */
double trianglearea(double a,double b,double c) {
	return sqrt((a+b-c)*(a-b+c)*(-a+b+c)*(a+b+c))/4;
}

/* calculate signed triangle area*2 based on coordinates */
/* OK UVa 10112 07.05.2012 */
/* OK UVa 10167 08.05.2012 */
int triarea2(int x1,int y1,int x2,int y2,int x3,int y3) {
	return (y3-y1)*(x2-x1)-(y2-y1)*(x3-x1);
}

/* used by pointintri */
int iabs(int a) {
	return a<0?-a:a;
}

/* check if x4,y4 is inside triangle by checking for increasing area! */
/* does not work for degenerate triangles if 4th point is collinear */
/* OK UVa 10112 07.05.2012 */
int pointintri(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4) {
	return iabs(triarea2(x1,y1,x2,y2,x3,y3))==
		iabs(triarea2(x1,y1,x2,y2,x4,y4))+
		iabs(triarea2(x2,y2,x3,y3,x4,y4))+
		iabs(triarea2(x3,y3,x1,y1,x4,y4));
}

/* given triangle vertices, find circumcenter (point having equal distance
   to all vertices */
void circumcenter(int x1,int y1,int x2,int y2,int x3,int y3,double *x4,double *y4) {
	double ax1=(x1+x2)*.5,ay1=(y1+y2)*.5,bx1=(x1+x3)*.5,bx2=(y1+y3)*.5;
	double dx1=x1-x2,dy1=y1-y2,dx2=x1-x3,dy2=y1-y3;
	double ax2=ax1+dy1,ay2=ay1-dx1,bx2=ax2+dy2,by2=ay2-dx2;
	double d=(ax1-ax2)*(by1-by2)-(ay1-ay2)*(bx1-bx2);
	if(d<1e-9 && d>-1e-9) return;
	*x4=(ax1*ay2-ay1*ax2)*(bx1-bx2)-(ax1-ax2)*(bx1*by2-by1*bx2);
	*y4=(ax1*ay2-ay1*ax2)*(by1-by2)-(ay1-ay2)*(bx1*by2-by1*bx2);
}
