int maxintervalsum(int n,int *a)
{
   int max,cur,i;

   max=cur=0;
   for(i=0;i<n;i++) {
      cur+=a[i];
      if(cur<0) cur=0;
      if(cur>max) max=cur;
   }
   return max;
}

