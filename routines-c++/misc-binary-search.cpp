/* warning: set t1 to desired min val -1 */
ll bs(ll t1,ll t3)
{
   ll t2,t3;
   while(t1<t3-1) {
      t2=(t1+t3)>>1;
      ll r=calc(t2); // formula
      if(r>=some value) t3=t2;
      else t1=t2;
   }
   return t3;
}

