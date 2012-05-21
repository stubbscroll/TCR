struct point_s {
   int x,y;
};

int pnpoly(struct point_s *p,int n,int ix,int iy)
{
   double x=ix,y=iy,xpi,ypi,ypj,xpj;
   int i,j,c=0;

   if(n<3) return 0;
   for(i=0,j=n-1;i<n;j=i++) {
      xpi=p[i].x;
      ypi=p[i].y;
      xpj=p[j].x;
      ypj=p[j].y;
      if((((ypi<=y) && (y<ypj)) ||
         ((ypj<=y) && (y<ypi))) &&
         (x < (xpj - xpi) * (y - ypi) / (ypj - ypi) + xpi))
         c=!c;
   }
   return c;
}

int online(struct point_s *p,int n,int x,int y)
{
   int i,j,a,b;
   double slope,y1;

   for(i=0;i<n;i++) {
      j=(i+1)%n;
      if(p[i].x==p[j].x) {
         /* horizontal */
         if(p[i].x!=x) continue;
         a=MIN(p[i].y,p[j].y);
         b=MAX(p[i].y,p[j].y);
         if(y>=a && y<=b) return 1;
      } else if(p[i].y==p[j].y) {
         /* vertical */
         if(p[i].y!=y) continue;
         a=MIN(p[i].x,p[j].x);
         b=MAX(p[i].x,p[j].x);
         if(x>=a && x<=b) return 1;
      } else {
         slope=(double)(p[j].x-p[i].x)/(p[j].y-p[i].y);
         y1=p[i].y+(x-p[i].x)/slope;
         if(fabs(y1-y)<EPS) return 1;
      }
   }
   return 0;
}

int inside(struct point_s *p,int n,int x,int y)
{
   int i;

   /* check if x,y is identical to a point in p[] */
   for(i=0;i<n;i++) if(p[i].x==x && p[i].y==y) return 1;
   if(online(p,n,x,y)) return 1;
   if(n==1) return 0;
   return pnpoly(p,n,x,y);
}

int pointonline(double x,double y,double x1,double y1,double x2,double y2)
{
   int i,j;
   double slope,yc,t,a,b;

   if(equal(x1,x2)) {
      /* horizontal */
      if(!equal(x1,x)) return 0;
      a=(y1<y2) ? y1 : y2;
      b=(y1>y2) ? y1 : y2;
      if(y>=a && y<=b) return 1;
   } else if(equal(y1,y2)) {
      /* vertical */
      if(!equal(y1,y)) return 0;
      a=(x1<x2) ? x1 : x2;
      b=(x1>x2) ? x1 : x2;
      if(x>=a && x<=b) return 1;
   } else {
      slope=(double)(x2-x1)/(y2-y1);
      yc=y1+(x-x1)/slope;
      if(fabs(yc-y)<EPS) {
         if((equal(x1,x2) && y1>y2) || x1>x2) {
            SWAP(x1,x2) SWAP(y1,y2)
         }
         return (x>=x1 && x<=x2);
      }
   }
   return 0;
}

int linecircleintersect(int sx,int sy,int sr,double rx,double ry,double dax,double day,
         double *hitx,double *hity,double *dist)
{
   double slope,y,a,b,c,root,y1,y2,x1,x2,dx,dy,d[2];

   if(equal(dax,0)) 
      /* x-direction is null */
      return linecircleintersect(sy,sx,sr,ry,rx,day,dax,hity,hitx,dist);
   else {
      slope=day/dax;
      /* find y-coord for x=0 */
      y=ry-rx*slope;
      /* so, line is: f(x) = y+x*slope */
      /* circle is:   g(x) = sqrt(sr-(x-sx)^2)+sy */
      /* intersection at (slope^2+1)*x^2 + ((2*y-2*sy)*slope-2*sx)*x +
                          y^2 - 2*sy*y - sr*sr + sx^2 + sy^2  = 0 */
      a=slope*slope+1;
      b=(2*y-2*sy)*slope-2*sx;
      c=y*y-2*sy*y-sr*sr+sx*sx+sy*sy;
      root=b*b-4*a*c;
      if(root<0) return 0;
      if(equal(root,0)) {
         x1=-b/(2*a); y1=y+x1*slope;
         if(pointonline(x1,y1,rx,ry,rx+dax*EXT,ry+day*EXT)) {
            *hitx=x1; *hity=y1;
            dx=x1-rx; dy=y1-ry;
            *dist=sqrt(dx*dx+dy*dy);
            return 1;
         }
         return 0;
      }
      x1=(-b+sqrt(root))/(2*a);
      x2=(-b-sqrt(root))/(2*a);
      y1=y+x1*slope;
      y2=y+x2*slope;
      dx=x1-rx; dy=y1-ry;
      d[0]=sqrt(dx*dx+dy*dy);
      if(!pointonline(x1,y1,rx,ry,rx+dax*EXT,ry+day*EXT)) d[0]+=BINF;
      dx=x2-rx; dy=y2-ry;
      d[1]=sqrt(dx*dx+dy*dy);
      if(!pointonline(x2,y2,rx,ry,rx+dax*EXT,ry+day*EXT)) d[1]+=BINF;
      if(d[0]<INF && d[0]<d[1] && !equal(d[0],0)) {
         *hitx=x1; *hity=y1; *dist=d[0];
      } else if(d[1]<INF && d[1]<=d[0] && !equal(d[1],0)) {
         *hitx=x2; *hity=y2; *dist=d[1];
      } else return 0;
      return 1;
   }
   return 0;
}

