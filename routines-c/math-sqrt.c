/* precision-safe square root of signed 64-bit int */

ll llsqrt(ll n)
{
   ll t1=1,t2,t3=3037000500LL;
   while(t1+1<t3) {
      t2=(t1+t3)/2;
      if(t2*t2<=n) t1=t2;
      else t3=t2;
   }
   return t1;
}

