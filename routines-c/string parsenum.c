int parsenumslow(char *s,int *a)
{
   int p=0,q,n=0;

   while(sscanf(s+p,"%d%n",&a[n],&q)==1) p+=q,n++;
   return n;
}

int parsenum(char *s,int *a)
{
   int n=0,v=0,w=0,l=strlen(s),i;

   for(i=0;i<=l;i++) if(s[i]=='-') w=-1;
   else if(s[i]>='0' && s[i]<='9') {
      if(!w) w=1;
      v=v*10+s[i]-48;
   } else if(w) {
      a[n++]=v*w;
      w=v=0;
   }
   return n;
}

