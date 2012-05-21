/* the subset-sum problem:
   we are given a finite set S from N and a target t from N.
   find a subset S' from S whose elements sum up to t.
   return the actual subset S'.
*/

#define MAXSUB 524288
vi subsetsum(vi &m,int t)
{
   int n=m.si,max=1;
   char val[MAXSUB];
   int prv[MAXSUB];

   del(val,0);
   del(prv,-1);
   val[0]=1;
   fir(n) {
      frd(j,max-1,0) if(val[j] && !val[j+m[i]]) {
         val[j+m[i]]=1;
         prv[j+m[i]]=i;
      }
      max+=m[i];
   }
   vi r;
   if(val[t]) {
      while(t) {
         int u=prv[t];
         t-=m[u];
         r.pb(u);
      }
      sort(all(r));
   }
   return r;
}
