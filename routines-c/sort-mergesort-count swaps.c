/* globals a[], b[], sort a[], use temp b[] */
long long mergesort(int l,int r)
{
   int m,i,j,k,c,d,t;
   long long num;

   if(r-l==1) return 0;
   if(r-l==2) {
      if(a[l]>a[r-1]) {
         t=a[l]; a[l]=a[r-1]; a[r-1]=t;
         return 1;
      }
      return 0;
   }
   m=(r+l)/2;
   num=mergesort(l,m);
   num+=mergesort(m,r);
   memcpy(b+l,a+l,(r-l)*sizeof(int));
   i=l; j=l; k=m;
   while(i<r) {
      if(j==m) c=2000000000;
      else c=b[j];
      if(k==r) d=2000000000;
      else d=b[k];
      if(c<=d) j++;
      else {
         num+=m-j;
         k++; c=d;
      }
      a[i++]=c;
   }
   return num;
}

