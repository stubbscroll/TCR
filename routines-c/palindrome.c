char pal[2501][2501]; /* [a][b]: there is a palindrome starting at a and ending
                         at b, inclusive */

void findpalin(int a,int b,char *s)
{
   int n=strlen(s);

   while(a>0 && b<n-1 && s[a-1]==s[b+1]) { a--; b++; }
   pal[a][b]=1;
}

/* find maximal palindromes */
void palin(char *s)
{
   int n=strlen(s),i;

   memset(pal,0,sizeof(pal));
   for(i=0;i<n;i++) {
      /* for each position, find the longest palindrome */
      findpalin(i,i,s);
      if(i<n-1 && s[i]==s[i+1]) findpalin(i,i+1,s);
   }
}

