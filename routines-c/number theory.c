/* calculate R(n) % mod */
/* it's faster to use powmod(10,n,9*mod) */
int repmod(int n,int mod)
{
   int i=30,v=1,t=10;

   while(i>=0 && !(n&(1<<i))) --i;
   for(--i;i>=0;--i) if(n&(1<<i)) {
      v=((ll)v*t%mod*10%mod+(ll)v*10+1)%mod;
      t=(ll)t*t%mod*10%mod;
   } else {
      v=((ll)v*t+v)%mod;
      t=(ll)t*t%mod;
   }
   return v;
}

int converttofraci(int *a,int l,int *p,int *q)
{
   int t;

   *p=a[l--];
   *q=1;
   while(l>=0) {
      t=*p;
      *p=*p*a[l--]+*q;
      *q=t;
   }
}

/* requires bigint */
void bigintconverttofrac(int *a,int l,char *p,char *q)
{
   char t[MAXD];

   sprintf(p,"%d",a[l--]);
   strcpy(q,"1");
   while(l>=0) {
      strcpy(t,p);
      bigintmuli(p,a[l--],p);
      bigintadd(q,p,p);
      strcpy(q,t);
   }
}
