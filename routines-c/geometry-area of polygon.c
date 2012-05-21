/* return area*2 */
int polygonarea(struct point_s *p,int n)
{
   int i,a;

   for(a=0,i=1;i<=n;i++)
      a+=p[i-1].x*p[i%n].y-p[i%n].x*p[i-1].y;
   return a;
}

/* area of triangle, given its side lengths */
/* warning, numerically unstable, later replace with linear algebra version */
double areatrianglesides(int a,int b,int c)
{
   return sqrt((a+b+c)*(b+c-a)*(c+a-b)*(a+b-c))*0.25;
}

/* return triangle area*2 */
int trianglearea(int x1,int y1,int x2,int y2,int x3,int y3)
{
   return x1*y2 - x2*y1 +
          x2*y3 - x3*y2 +
          x3*y1 - x1*y3;
}
