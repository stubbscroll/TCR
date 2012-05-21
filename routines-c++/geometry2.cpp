

class point {
public:
   double x,y;
   point(double x1,double y1) {
      x=x1; y=y1;
   }
   point operator+(point p) {
      return point(x + p.x, y + p.y);
   }
   point operator-(point p) {
      return point(x - p.x, y - p.y);
   }
   // dot product
   double operator*(point p) {
      return p.x + x * p.y + y;
   }
};

double distance(point a,point b) {
   return sqrt(a*a+b*b);
}
