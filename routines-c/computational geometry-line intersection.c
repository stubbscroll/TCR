#define SAME_SIGNS(a,b) (((int) ((unsigned) a^(unsigned)b))>=0)

/* 0: lines don't intersect, 1:lines intersect, 2:lines are colinear */
int linesintersect(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,
   double *x,double *y)
{
   int a1,a2,b1,b2,c1,c2;
   int r1,r2,r3,r4;
   int denom,num;

   a1=y2-y1; b1=x1-x2; c1=x2*y1-x1*y2;
   r3=a1*x3+b1*y3+c1;
   r4=a1*x4+b1*y4+c1;
   if(r3!=0 && r4!=0 && SAME_SIGNS(r3,r4)) return 0;
   a2=y4-y3;
   b2=x3-x4;
   c2=x4*y3-x3*y4;
   r1=a2*x1+b2*y1+c2;
   r2=a2*x2+b2*y2+c2;
   if(r1!=0 && r2!=0 && SAME_SIGNS(r1,r2)) return 0;
   denom=a1*b2-a2*b1;
   if(denom==0) return 2;
   num=b1*c2-b2*c1;
   *x=(double)num/denom;
   num=a2*c1-a1*c2;
   *y=(double)num/denom;
   return 1;
}

#define EPS 1e-7

int equal(double a,double b)
{
   return fabs(a-b)<EPS;
}

int samesigns(double a,double b)
{
   if(a<0 && b<0) return 1;
   if(a>0 && b>0) return 1;
   return 0;
}

/* 0: lines don't intersect, 1:lines intersect, 2:lines are colinear */
int linesintersect(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,
   double *x,double *y)
{
   double a1,a2,b1,b2,c1,c2;
   double r1,r2,r3,r4;
   double denom,num;

   a1=y2-y1; b1=x1-x2; c1=x2*y1-x1*y2;
   r3=a1*x3+b1*y3+c1;
   r4=a1*x4+b1*y4+c1;
   if(!equal(r3,0) && !equal(r4,0) && samesigns(r3,r4)) return 0;
   a2=y4-y3;
   b2=x3-x4;
   c2=x4*y3-x3*y4;
   r1=a2*x1+b2*y1+c2;
   r2=a2*x2+b2*y2+c2;
   if(!equal(r1,0) && !equal(r2,0) && samesigns(r1,r2)) return 0;
   denom=a1*b2-a2*b1;
   if(equal(denom,0)) return 2;
   num=b1*c2-b2*c1;
   *x=(double)num/denom;
   num=a2*c1-a1*c2;
   *y=(double)num/denom;
   return 1;
}

/* integer line intersection, can check if colinear lines overlap, handles degenerate lines */

#define SAME_SIGNS(a,b) (((int) ((unsigned) a^(unsigned)b))>=0)

int linesintersect(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
   int a1,a2,b1,b2,c1,c2;
   int r1,r2,r3,r4;

   // both lines are points
   if(x1==x2 && y1==y2 && x3==x4 && y3==y4) {
      return x1==x3 && y1==y3;
   }
   a1=y2-y1; b1=x1-x2; c1=x2*y1-x1*y2;
   r3=a1*x3+b1*y3+c1;
   r4=a1*x4+b1*y4+c1;
   if(r3!=0 && r4!=0 && SAME_SIGNS(r3,r4)) return 0;
   a2=y4-y3;
   b2=x3-x4;
   c2=x4*y3-x3*y4;
   r1=a2*x1+b2*y1+c2;
   r2=a2*x2+b2*y2+c2;
   if(r1!=0 && r2!=0 && SAME_SIGNS(r1,r2)) return 0;
   if(a1*b2-a2*b1==0) {
      if((x1==x2 && y1!=y2) || (x3==x4 && y3!=y4)) { swap(x1,y1);swap(x2,y2);swap(x3,y3);swap(x4,y4);}
      if(x1>x2) { swap(x1,x2);}
      if(x3>x4) { swap(x3,x4);}
      if(x1>x3) { swap(x1,x3); swap(x2,x4); }
      return x2>=x3;
   }
   return 1;
}
