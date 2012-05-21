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

