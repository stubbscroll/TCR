/* calculate polygon area*2 */
int polygonarea(vi x,vi y)
{
   int i,a,n=x.si;

   for(a=0,i=1;i<=n;i++)
      a+=x[i-1]*y[i%n]-x[i%n]*y[i-1];
   return abs(a);
}

double calcdist3d(double x1,double y1,double z1,double x2,double y2,double z2)
{
   double dx=x1-x2,dy=y1-y2,dz=z1-z2;
   return sqrt(dx*dx+dy*dy+dz*dz);
}

double calcdist(double x1,double y1,double x2,double y2)
{
   double dx=x1-x2,dy=y1-y2;
   return sqrt(dx*dx+dy*dy);
}

/* calculate triangle area, based on side lengths */
double triarea(double s1,double s2,double s3)
{
   double p=(s1+s2+s3)/2;
   return sqrt(p*(p-s1)*(p-s2)*(p-s3));
}

