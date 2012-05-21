double pi,d2r;

/* distance between two points on the surface of a sphere, parameters are
 * latitude/longitude converted to radians (*d2r) and radius */
double distonsphere(double lai,double loi,double laj,double loj,double r)
{
   double dlon=loj-loi,dlat=laj-lai;
   double dla=sin(dlat/2),dlo=sin(dlon/2);
   double a=dla*dla+cos(lai)*cos(laj)*dlo*dlo;
   double d=2*atan2(sqrt(a),sqrt(1-a));
   return d*r;
}

void geoinit()
{
   pi=2*acos(0.0); d2r=pi/180;
}

/* calculate area of rectangle b[0-1]:upper left, b[2-3]:lower right */
int recarea(vi b)
{
   return (b[2]-b[0])*(b[3]-b[1]);
}

/* return intersection of two rectangles, actual rectangle returned */
vi recintersect(vi b,vi c)
{
   vi a(4,0);
   a[0]=b[0]>?c[0];
   a[1]=b[1]>?c[1];
   a[2]=b[2]<?c[2];
   a[3]=b[3]<?c[3];
   if(a[0]>=a[2] || a[1]>=a[3]) { a[2]=a[0]; a[3]=a[1]; }
   return a;
}

