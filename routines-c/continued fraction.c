int a[10000];

/* calculate the continued fraction of b1/b2, the
   nominator is always 1 */
int contfrac(int b1,int b2)
{
   int i=b1/b2,n=0;

   b1%=b2;
   while(b1!=1) {
      i=b1; b1=b2; b2=i;
      i=b1/b2;
      a[n++]=i;
      b1%=b2;
   }
   a[n++]=b2;
   return n;
}

