/* generate all subsets of i elements taken from j elements
 * initialize */
int initsub(char *s,int i,int j)
{
   int k;

   for(k=0;k<i;k++) s[k]=k;
}

/* generate next subset, return false if there are no more */
int nextsub(char *s,int i,int j)
{
   int k,m;

   k=i-1; m=j;
   do {
      s[k]++;
      if(s[k]!=m) break;
      k--; m--;
   } while(k>=0);
   if(k==-1) return 0;
   for(k++;k<j;k++) s[k]=s[k-1]+1;
   return 1;
}

