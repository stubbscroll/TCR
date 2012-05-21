double dotproduct(double x1,double y1,double z1,double x2,double y2,double z2)
{
   return x1*x2+y1*y2+z1*z2;
}

void crossproduct(double x1,double y1,double z1,double x2,double y2,double z2,
   double *x,double *y,double *z)
{
   *x=y1*z2-z1*y2;
   *y=z1*x2-x1*z2;
   *z=x1*y2-y1*z2;
}

