#include <stdio.h>

#define SAMESIGN(x,y) (((long long)x*y)>0)

/* 0: lines don't intersect, 1:lines intersect, 2:lines are colinear */
/* NB, collinear lines aren't handled very well. */
/* NB, this code is a ticking overflow-bomb */
/* OK NCPC 2010 problem J 19.08.2011 */
int linesintersect(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,
                   double *x,double *y) {
	int a1,a2,b1,b2,c1,c2,r1,r2,r3,r4;
	long long teller,nevner;
	a1=y2-y1; b1=x1-x2; c1=x2*y1-x1*y2;
	r3=a1*x3+b1*y3+c1; r4=a1*x4+b1*y4+c1;
	if(r3!=0 && r4!=0 && SAMESIGN(r3,r4)) return 0;
	a2=y4-y3; b2=x3-x4; c2=x4*y3-x3*y4;
	r1=a2*x1+b2*y1+c2; r2=a2*x2+b2*y2+c2;
	if(r1!=0 && r2!=0 && SAMESIGN(r1,r2)) return 0;
	nevner=a1*b2-a2*b1;
	if(!nevner) return 2;
	teller=(ll)b1*c2-(ll)b2*c1;
	*x=(double)teller/nevner;
	teller=(ll)a2*c1-(ll)a1*c2;
	*y=(double)teller/nevner;
	return 1;
}

#define SAMESIGN(x,y) (((x)>0 && (y)>0) || ((x)<0 && (y)<0))
#define ABS(x) ((x)<0?-(x):(x))

/* return signed triangle area*2 */
ll triarea2(int x1,int y1,int x2,int y2,int x3,int y3) {
	return (ll)x1*y2-(ll)x2*y1+(ll)x2*y3-(ll)x3*y2+(ll)x3*y1-(ll)x1*y3;
}

/* return just y/n if there is an intersection. checks if parallel line
   is between (x1,y1) and (x2,y2). */
/* OK spoj 9895 (nwerc 2011 problem I) 16.01.2012 */
int linesintersect(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4) {
	ll a1,a2,b1,b2,c1,c2,r1,r2,r3,r4;
	a1=y2-y1; b1=x1-x2; c1=(ll)x2*y1-(ll)x1*y2;
	r3=a1*x3+b1*y3+c1; r4=a1*x4+b1*y4+c1;
	if(r3!=0 && r4!=0 && SAMESIGN(r3,r4)) return 0;
	a2=y4-y3; b2=x3-x4; c2=x4*y3-x3*y4;
	r1=a2*x1+b2*y1+c2; r2=a2*x2+b2*y2+c2;
	if(r1!=0 && r2!=0 && SAMESIGN(r1,r2)) return 0;
	if(a1*b2-a2*b1) return 1;
	if(triarea2(x1,y1,x2,y2,x3,y3)) return 0;
	if(x1==x2) {
		if(y1<y3 && y3<y2) return 1;
		if(y1>y3 && y3>y2) return 1;
	} else {
		if(x1<x3 && x3<x2) return 1;
		if(x1>x3 && x3>x2) return 1;
	}
	return 0;
}
