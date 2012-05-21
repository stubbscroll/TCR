//TODO: numerically stable area of triangle, given sides

//TODO (maybe): circle-circle intersection
//TODO (maybe): line-circle intersection

/* determinant, type must be vector<vector<type>>,
   where type is int, long long, double or similar. no integer-overflow. */

template<class T> inline double det2d(T a) { return (double)a[0][0]*a[1][1]-(double)a[1][0]*a[0][1]; }

/* determinant, takes 4 values, returns value of same type. can overflow! */
// 17.02.2007, tested on srm 183 div 1 medium ok

template<class T> inline T det2d(T a1,T a2,T b1,T b2) { return a1*b2-a2*b1; }

/* distance in 2d between two points.
   datatype must be a pair of integer, long long, double or
   a similar type. */
// 17.02.2007, tested on srm 183 div 1 medium ok

template<class T> inline double dist2d(T a,T b) { double dx=a.first-b.first,dy=a.second-b.second; return sqrt(dx*dx+dy*dy); }

/* convert line given by (x1,y1),(x2,y2) to the form Ax+By=C. can overflow! */
// 17.02.2007, tested on srm 183 div 1 medium ok

template<class T> inline void linetoeq(pair<T,T> a,pair<T,T> b,T &A,T &B,T &C) { A=b.second-a.second; B=a.first-b.first; C=A*a.first+B*a.second; }

/* find the perpendicular to the bisection of a line, the type T must be a
   pair of int, long long, double or similar. */
// 17.02.2007, tested on srm 183 div 1 medium ok

template<class T> inline void linebisectperp(T a,T b,T &A,T &B) { A=mp((a.first+b.first)/2.0,(a.second+b.second)/2.0); double dx=b.first-a.first,dy=b.second-a.second; B=mp(A.first+dy,A.second-dx); }

/* line segment intersection
   0:don't intersect, 1:intersect
   2:parallel, but not touching, 3:parallel and touching
   NB! parallel-things not tested */
// 18.02.2007, tested on srm 250 div 1 hard ok

int linesegintersect(pdd p1,pdd p2,pdd p3,pdd p4,pdd &c)
{
   double a1,a2,b1,b2,c1,c2,r1,r2,r3,r4;
   linetoeq(p1,p2,a1,b1,c1);
   r3=a1*p3.first+b1*p3.second-c1;
   r4=a1*p4.first+b1*p4.second-c1;
   if(fabs(r3)>EPS && fabs(r4)>EPS && (r3<0)==(r4<0)) return 0;
   linetoeq(p3,p4,a2,b2,c2);
   r1=a2*p1.first+b2*p1.second-c2;
   r2=a2*p2.first+b2*p2.second-c2;
   if(fabs(r1)>EPS && fabs(r2)>EPS && (r1<0)==(r2<0)) return 0;
   double det=det2d(a1,a2,b1,b2);
   if(fabs(det)<EPS) {
      if(fabs(p1.first-p1.second)<EPS) {
         #define SWAPP(p) swap(p.first,p.second)
         SWAPP(p1); SWAPP(p2); SWAPP(p3); SWAPP(p4);
         #undef SWAPP
         if(p1.first>p2.first) swap(p1,p2);
         if(p3.first>p4.first) swap(p3,p4);
         if(p1.first>p3.first) swap(p1,p3);
         return 2+(p2.first-p3.first>EPS);
      }
      return 2;
   } else {
      c=mp((b2*c1-b1*c2)/det,(a1*c2-a2*c1)/det);
      return 1;
   }
}

/* line intersection
   return 1 if lines are parallel, 0 otherwise */
// 17.02.2007, tested on srm 183 div 1 medium ok

int linesintersect(pdd p1,pdd p2,pdd p3,pdd p4,pdd &c)
{
   double a1,b1,c1,a2,b2,c2;
   linetoeq(p1,p2,a1,b1,c1);
   linetoeq(p3,p4,a2,b2,c2);
   double det=det2d(a1,a2,b1,b2);
   if(fabs(det)<EPS) return 1;
   else {
      c=mp((b2*c1-b1*c2)/det,(a1*c2-a2*c1)/det);
      return 0;
   }
}

/* polygon area */
// 17.02.2007, tested on srm 166 div 2 hard ok (worst case 4 ms)
// 18.02.2007, tested on srm 250 div 1 hard ok
// 18.02.2007, tested on srm 249 div 1 hard ok
// 18.02.2007, tested on tccc2004-q1 medium ok (worst case 1 ms)
// 18.02.2007, tested on UVa 137 ok

template<class T> inline double polygonarea(T p)
{
   int n=p.si; if(n<3) return 0; double a=(double)p[0].first*p[n-1].second-(double)p[0].second*p[n-1].first;
   fir(n-1) a+=(double)p[i+1].first*p[i].second-(double)p[i+1].second*p[i].first;
   return fabs(a)*.5;
}

/* calculate circle from 3 points, return centre and radius */
// 17.02.2007, tested on srm 183 div 1 medium ok (worst case 34 ms)

void findcircle(pdd p1,pdd p2,pdd p3,pdd &c,double &r)
{
   pdd m1,m2,e1,e2;
   linebisectperp(p1,p2,m1,e1);
   linebisectperp(p2,p3,m2,e2);
   if(linesintersect(m1,e1,m2,e2,c)) r=1e100;
   else r=dist2d(c,p1);
}

/* signed triangle area *2 */
// 18.02.2007, tested on srm 250 div 1 hard ok

inline double signedtrianglearea2(pdd a,pdd b,pdd c)
{
   return (b.first-a.first)*(c.second-a.second)-(c.first-a.first)*(b.second-a.second);
}

/* convex hull, O(n log n) */
// 18.02.2007, tested on srm 250 div 1 hard ok
// 18.02.2007, tested on srm 249 div 1 hard ok (worst case 4 ms)
// 18.02.2007, tested on UVa 137 ok

pdd CP0;

bool convexcompare(const pdd &a,const pdd &b)
{
   pdd A=a,B=b;
   double aa=signedtrianglearea2(CP0,A,B);
   if(aa>EPS) return true;
   else if(aa<-EPS) return false;
   else {
      double x=fabs(a.first-CP0.first)-fabs(b.first-CP0.first);
      double y=fabs(a.second-CP0.second)-fabs(b.second-CP0.second);
      return(x>0 || y>0);
   }
}

vpdd convexhull2d(vpdd p)
{
   int m=0,n=p.si,j=2;
   if(!n) return p;
   fr(i,1,n) if(p[i].second<p[m].second || (p[i].second==p[m].second && p[i].first<p[m].first)) m=i;
   if(m) swap(p[0],p[m]);
   CP0=p[0];
   sort(p.begin()+1,p.end(),convexcompare);
   fr(i,2,n) if(fabs(signedtrianglearea2(CP0,p[j-1],p[i]))>EPS) p[j++]=p[i];
   p.resize(j); n=j;
   vpdd h;
   fir(min(2,n)) h.pb(p[i]);
   j=2;
   if(n<3) return h;
   while(j<n) {
      if(signedtrianglearea2(h[h.si-2],h[h.si-1],p[j])>EPS) h.pb(p[j++]);
      else h.pop_back();
   }
   return h;
}

/* check if point is inside polygon, returns true if strictly inside */
// 18.02.2007, tested on srm 250 div 1 hard ok
// 18.02.2007, tested on srm 187 div 2 hard ok
// 18.02.2007, tested on UVa 137 ok

template<class T> bool pointinpolygon(T a,vector<T> b)
{
   int n=b.si,c=0;
   fir(n) {
      int ix=i,iy=(i+1)%n;
      if(b[ix].second>b[iy].second) swap(ix,iy);
      if(a.second>b[ix].second-EPS && a.second+EPS<b[iy].second && signedtrianglearea2(a,b[ix],b[iy])>EPS) c^=1;
   }
   return c==1;
}

/* calculate intersection of two convex polygons, lazy version */
// 18.02.2007, tested on srm 250 div 1 hard ok (worst case 2 ms)
// 18.02.2007, tested on UVa 137 ok

template<class T> vector<pdd> convexpolygonintersect(vector<T> a,vector<T> b)
{
   vpdd p; int n=a.si,m=b.si;
   fir(n) fjr(m) {
      pdd c;
      if(linesegintersect(a[i],a[(i+1)%n],b[j],b[(j+1)%m],c)==1) p.pb(c);
   }
   fir(n) if(pointinpolygon(a[i],b)) p.pb(a[i]);
   fir(m) if(pointinpolygon(b[i],a)) p.pb(b[i]);
   return convexhull2d(p);
}

/* distance from line to point */
// 18.02.2007, tested on srm 187 div 2 hard ok
// 20.02.2007, tested on srm 313 div 1 hard ok

template<class T> inline double linepointdist(T a,T b,T p) { return fabs(det2d(b.first-a.first,b.second-a.second,p.first-a.first,p.second-a.second))/dist2d(a,b); }

/* distance from line segment to point */
// 18.02.2007, tested on srm 187 div 2 hard ok
// 20.02.2007, tested on srm 313 div 1 hard ok (worst case 10 ms)

template<class T> inline double tridot(T a,T b,T c) { return (b.first-a.first)*(c.first-b.first)+(b.second-a.second)*(c.second-b.second); }
template<class T> inline double linesegpointdist(T a,T b,T p)
{
   double d=tridot(a,b,p);
   if(d>EPS) return -1e100;
   d=tridot(b,a,p);
   if(d>EPS) return -1e100;
   return linepointdist(a,b,p);
}

/* check if point is on polygon boundary */
// 18.02.2007, tested on srm 187 div 2 hard ok

template<class T> bool pointonpolygon(T a,vector<T> b)
{
   int n=b.si;
   fir(n) {
      int j=(i+1)%n;
      double d=linesegpointdist(b[i],b[j],a);
      if(fabs(d)<EPS) return true;
   }
   return false;
}

