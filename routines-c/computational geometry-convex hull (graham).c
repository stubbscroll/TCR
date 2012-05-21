#define MAX 100000

typedef struct { int x,y,del,id; } point;
typedef long long ll;

point p[MAX],h[MAX];
int n,hn;

ll area2(point a,point b,point c)
{
   return ((ll)b.x-a.x)*((ll)c.y-a.y)-((ll)c.x-a.x)*((ll)b.y-a.y);
}

/* requires external array p for access to p[0] */
int compp(point *pi,point *pj)
{
   ll a=area2(p[0],*pi,*pj),x,y;

   if(a>0) return -1;
   else if(a<0) return 1;
   else {
      x=llabs((ll)pi->x-p[0].x)-llabs((ll)pj->x-p[0].x);
      y=llabs((ll)pi->y-p[0].y)-llabs((ll)pj->y-p[0].y);
      if(x<0 || y<0) { pi->del=1; return -1; }
      else if(x>0 || y>0) { pj->del=1; return 1; }
      else {
         if(pi->id>pj->id) pj->del=1;
         else pi->del=1;
         return 0;
      }
   }
}

/* graham's algorithm for 2d convex hull, O(n log n) */
int convexhull(point *p,int n,point *h)
{
   int i,j,m=0;

   /* find lowest point, swap it to pos 0 */
   for(i=1;i<n;i++) if(p[i].y<p[m].y || (p[i].y==p[m].y && p[i].x<p[m].x)) m=i;
   for(i=0;i<n;i++) { p[i].del=0; p[i].id=i; }
   if(m) {
      i=p[0].x; p[0].x=p[m].x; p[m].x=i;
      i=p[0].y; p[0].y=p[m].y; p[m].y=i;
   }
   for(i=1;i<n;i++) if(p[i].y==p[0].y && p[i].x==p[0].x) p[i].del=1;
   /* leave pos 0 alone */
   qsort(p+1,n-1,sizeof(point),compp);
   /* contract */
   for(i=j=0;i<n;i++) if(!p[i].del) { if(i!=j) p[j]=p[i]; j++; }
   n=j;
   /* find convex hull */
   h[0]=p[0]; h[1]=p[1]; hn=i=2;
   if(n<3) return n;
   while(i<n) {
      if(area2(h[hn-2],h[hn-1],p[i])>0) h[hn++]=p[i++];
      else hn--;
   }
   return hn;
}

