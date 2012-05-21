int x,y;
set<int> sx,sy;

ll calc(int tid)
{
   sx.clear(); sy.clear();
   sx.insert(0); sy.insert(0);
   sx.insert(x); sy.insert(y);

   // add coordinates
   int X,Y;
   if(X>0 && X<x) sx.insert(X);
   if(Y>0 && Y<y) sy.insert(Y);

   vi xx(sx.begin(),sx.end());
   vi yy(sy.begin(),sy.end());
   ll res=(ll)x*y;
   fir(xx.si-1) fjr(yy.si-1) {
      int x1,y1,x2,y2;
      // check if coordinates are desirable
      if(x1<=xx[i] && y1<=yy[j] && x2>=xx[i+1] && y2>=yy[j+1]) res-=ll(xx[i+1]-xx[i])*ll(yy[j+1]-yy[j]);
   }
   return res;
}

