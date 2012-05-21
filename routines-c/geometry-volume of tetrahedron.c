double dotproduct3d(double x1,double y1,double z1,double x2,double y2,double z2)
{
   return x1*x2+y1*y2+z1*z2;
}

void crossproduct3d(double x1,double y1,double z1,double x2,double y2,double z2,
   double *x3,double *y3,double *z3)
{
   *x3=y1*z2-y2*z1;
   *y3=z1*x2-z2*x1;
   *z3=x1*y2-x2*y1;
}

double volumetetrahedron(double x1,double y1,double z1,double x2,double y2,double z2,
   double x3,double y3,double z3,double x4,double y4,double z4)
{
   double x,y,z;

   crossproduct3d(x2-x1,y2-y1,z2-x1,x3-x1,y3-y1,z3-z1,&x,&y,&z);
   return fabs(dotproduct3d(x,y,z,x4-x1,y4-y1,z4-z1))/6;
}

