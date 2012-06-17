/* convex hull routine that output collinear points!
   warning, runtime is O(n^2).
*/

#define MAXP 444
#define INF 1000000
#define EPS 1e-6

typedef struct {
	int x,y;
} point_t;

point_t p[MAXP],h[MAXP];
int n,hn;

double distsq(double x1,double y1,double x2,double y2) {
	double dx=x1-x2,dy=y1-y2;
	return dx*dx+dy*dy;
}

/* OK UVa 596 n<=400 0.012 seconds 15.06.2012 */
void convexhull() {
	int i,hix=-INF,loy=-INF,ix=-1;
	int atx,aty;
	double pi=2*acos(0),dir,a,best,bestd;
	point_t t;
	for(i=0;i<n;i++) if(hix<p[i].x || (hix==p[i].x && loy>p[i].y)) hix=p[i].x,loy=p[i].y,ix=i;
	t=p[0],p[0]=p[ix],p[ix]=t;
	hn=ix=0;
	dir=pi/2;
	do {
		atx=p[ix].x; aty=p[ix].y;
		h[hn++]=p[ix];
		best=1e100; bestd=1e100;
		ix=-1;
		for(i=0;i<n;i++) if(p[i].x!=atx || p[i].y!=aty) {
			a=atan2(p[i].y-aty,p[i].x-atx);
			while(a+EPS<dir) a+=2*pi;
			if(fabs(best-a)<EPS && bestd>distsq(p[i].x,p[i].y,atx,aty)) best=a,ix=i,bestd=distsq(p[i].x,p[i].y,atx,aty);
			else if(fabs(best-a)>=EPS && best>a) best=a,bestd=distsq(p[i].x,p[i].y,atx,aty),ix=i;
		}
		if(ix<0) puts("error");
		dir=best;
	} while(ix);
}
