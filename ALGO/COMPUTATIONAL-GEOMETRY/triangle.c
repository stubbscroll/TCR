

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
