/* helper routine for insidepoly */
ll area2xy(int x1,int y1,int x2,int y2,int x3,int y3) {
	return ((ll)x2-x1)*((ll)y3-y1)-((ll)x3-x1)*((ll)y2-y1);
}

/* check if px,py is inside a simple polygon (not necessarily convex) */
/* handles |x|,|y|<10^9 */
/* warning, undefined result of point is on border */
/* OK UVa 109 25.05.2012 */
int insidepoly(int *x,int *y,int n,int px,int py) {
	int i,j,in=0;
	for(j=n-1,i=0;i<n;j=i++) {
		if(py>y[j] && py<=y[i] && area2xy(x[j],y[j],x[i],y[i],px,py)>0) in^=1;
		else if(py>=y[i] && py<y[j] && area2xy(x[i],y[i],x[j],y[j],px,py)>0) in^=1;
	}
	return in;
}

/* TODO given simple polygon and point, check if point is on border.
   can do this in O(n) by first checking if point coincide with vertex,
	 then check if the point is on a line. */

/* TODO check if point is inside convex polygon in O(log n) */
/* todo have four indexes: (a,b), the lower and upper points on left side,
   (c,d), the lower and upper points on right side)
*/
