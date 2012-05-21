void printll(long long x)
{
   long long a=1000000000000000000LL; /* 18 zeroes */
   if(x<0) { putchar('-'); x=-x; }
   else if(!x) { putchar('0'); return; }
   while(a>x) a/=10;
   while(a) { putchar(48+x/a); x-=x/a*a; a/=10; }
}

void printull(unsigned long long x)
{
   unsigned long long a=10000000000000000000ULL; /* 19 zeroes */
   if(!x) { putchar('0'); return; }
   while(a>x) a/=10;
   while(a) { putchar(48+x/a); x-=x/a*a; a/=10; }
}

long long strtoll(char *s)
{
   long long v=0; int i=0,z=1;
   if(s[0]=='-') i++,z=-1;
   for(;s[i];i++) v=v*10+s[i]-48;
   return v*z;
}

unsigned long long strtoull(char *s)
{
   unsigned long long v=0; int i=0;
   for(;s[i];i++) v=v*10+s[i]-48;
   return v;
}

unsigned long long strtolonglong(char *s)
{
   unsigned long long l,m;
   int i;

   l=0;
   m=1;
   for(i=strlen(s)-1;i>=0;i--) {
      l+=(s[i]-'0')*m;
      m*=10;
   }
   return l;
}

void ulltostr(unsigned long long l,char *s)
{
   unsigned long long m=1e19,n=1e18;
   int j,i=0;

   do {
      j=l%m/n;
      if(j>0) break;
      m=n;
      n/=10;
   } while(n>1);
   do {
      j=l%m/n;
      s[i++]=j+48;
      m=n;
      n/=10;
   } while(n);
   s[i]=0;
}

