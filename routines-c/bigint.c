#define MAXD 1000
int t[MAXD],t2[MAXD];

void bigintmul(char *a,char *b,char *r)
{
   int al,bl,i,j,i1,j1,m;

   al=strlen(a); bl=strlen(b);
   for(i=0;i<al+bl;i++) t[i]=0;
   for(i=al-1;i>=0;i--) {
      i1=al-i-1;
      for(j=bl-1;j>=0;j--) {
         j1=bl-j-1;
         m=(a[i]-'0')*(b[j]-'0');
         t[i1+j1]+=m;
      }
   }
   for(i=0;i<al+bl-1;i++)
      while(t[i]>=10) {
        t[i]-=10; t[i+1]++;
      }
   j=0;
   for(i=0;i<al+bl;i++)
      if(t[i]) j=i;
   for(i=0;i<j+1;i++)
      r[i]=t[j-i]+'0';
   r[i]=0;
}

void bigintadd(char *a,char *b,char *c)
{
   int a1,a2,p,q,i;

   a1=strlen(a)-1;
   a2=strlen(b)-1;
   if(a1<a2) p=a2; else p=a1;
   for(i=0;i<=p+1;i++) t[i]=0;
   p=0;
   do {
      if(a1<0 && a2<0) break;
      q=0;
      if(a1>=0) q=a[a1]-'0';
      if(a2>=0) q+=b[a2]-'0';
      t[p++]+=q;
      a1--; a2--;
   } while(1);
   for(i=0;i<p;i++)
      if(t[i]>9) {
         t[i]-=10;
         t[i+1]++;
         if(i==p-1) p++;
      }
   for(i=0;i<p;i++)
      c[p-i-1]=t[i]+'0';
   c[p]=0;
}

/* works only if a>=b */
void bigintsub(char *a,char *b,char *c)
{
   int a1,a2,p,q,i,last;

   a1=strlen(a)-1;
   a2=strlen(b)-1;
   if(a1<a2) p=a2; else p=a1;
   for(i=0;i<=p+1;i++) t[i]=0;
   p=0;
   do {
      if(a1<0 && a2<0) break;
      q=0;
      if(a1>=0) q=a[a1]-'0';
      if(a2>=0) q-=b[a2]-'0';
      t[p++]+=q;
      a1--; a2--;
   } while(1);
   last=0;
   for(i=0;i<p;i++) {
      if(t[i]<0) {
         t[i]+=10;
         t[i+1]--;
         if(i==p-1) p++;
      }
      if(t[i]>0) last=i;
   }
   last++;
   for(i=0;i<last;i++)
      c[last-i-1]=t[i]+'0';
   c[last]=0;
}

/* multiply a with an integer, works for b<200000000 */
void bigintmuli(char *a,int b,char *r)
{
   int al,i,j;

   al=strlen(a);
   for(i=0;i<al;i++) t[al-i-1]=(a[i]-48)*b;
   for(i=al;i<al+10;i++) t[i]=0;
   for(i=0;i<al+10;i++) if(t[i]>9) {
      t[i+1]+=t[i]/10;
      t[i]%=10;
   }
   i=al+9; while(!t[i] && i) i--;
   for(j=0;j<=i;j++) r[j]=t[i-j]+'0';
   r[j]=0;
}

/* divide a with an integer, works bad if d>600000000,
   change to long long to get bigger numbers */
void bigintdivi(char *a,int d,char *b)
{
   int al,i,carry,x;

   al=strlen(a);
   b[al]=0;
   carry=0;
   for(i=0;i<al;i++) {
      x=a[i]-'0';
      b[i]=(x+carry)/d+'0';
      carry=(x+carry)%d*10;
   }
   x=0;
   while(b[x]=='0') x++;
   if(x==al) x-=1;
   if(x)
      for(i=x;i<=al;i++)
         b[i-x]=b[i];   
}

int bigintmodi(char *a,int d)
{
   int res=0,i;
   for(i=0;a[i];i++) res=(res*10+a[i]-'0')%d;
   return res;
}

/* a<b: -1, a==b: 0, a>b: 1 */
int bigintcmp(char *a,char *b)
{
   int an=strlen(a),bn=strlen(b),i;

   if(an<bn) return -1;
   if(an>bn) return 1;
   for(i=0;a[i];i++) {
      if(a[i]<b[i]) return -1;
      else if(a[i]>b[i]) return 1;
   }
   return 0;
}

/* warning! this routine does not like leading zeroes */
void bigintmod(char *a,char *b,char *r)
{
   char c[MAXD];
   char d[MAXD];
   int i,cl,obl;

   /* division by zero */
   if(!strcmp(b,"0")) {
      strcpy(r,"0");
      return;
   }
   strcpy(d,a);
   /* no mod, a is less */
   i=bigintcmp(d,b);
   if(i==-1) {
      strcpy(r,d);
      return;
   } else if(i==0) {
      /* equal: mod=0 */
      strcpy(r,"0");
      return;
   }
   strcpy(c,b);
   cl=strlen(c);
   while(bigintcmp(a,c)>=0) {
      c[cl]='0'; c[cl+1]=0;
      cl++;
   }
   cl--;
   c[cl]=0;
   obl=strlen(b);
   while(1) {
      while(bigintcmp(d,c)>=0) bigintsub(d,c,d);
      if(cl==obl) break;
      cl--;
      if(cl<0) cl=1/0;
      c[cl]=0;
   }
   strcpy(r,d);
}

/* warning! this routine does not like leading zeroes */
void bigintdiv(char *a,char *b,char *r)
{
   char c[MAXD];
   char d[MAXD];
   int i,cl,el,obl;

   /* division by zero */
   if(!strcmp(b,"0")) {
      strcpy(r,"0");
      return;
   }
   strcpy(d,a);
   /* no div, a is less */
   i=bigintcmp(d,b);
   if(i==-1) {
      strcpy(r,"0");
      return;
   } else if(i==0) {
      /* equal: div=1 */
      strcpy(r,"1");
      return;
   }
   strcpy(c,b);
   cl=strlen(c);
   while(bigintcmp(a,c)>=0) {
      c[cl]='0'; c[cl+1]=0;
      cl++;
   }
   cl--;
   c[cl]=0; el=0;
   obl=strlen(b);
   while(1) {
      t2[el]=0;
      while(bigintcmp(d,c)>=0) {
         t2[el]++;
         bigintsub(d,c,d);
      }
      el++;
      if(cl==obl) break;
      cl--;
      if(cl<0) cl=1/0;
      c[cl]=0;
   }
   for(i=0;i<el;i++) r[i]=t2[i]+'0';
   r[i]=0;
}

int bigintisnull(char *a)
{
   return a[0]=='0' && !a[1];
}

char *bigintgcd(char *a,char *b)
{
   char t[MAXD];

   while(!bigintisnull(b)) {
      strcpy(t,b);
      bigintmod(a,b,b);
      strcpy(a,t);
   }
   return a;
}

void bigintrmzero(char *a,char *b)
{
   int i,j;

   for(i=0;a[i];i++) if(a[i]!='0') break;
   if(!a[i]) i--;
   if(i<0) i++;
   j=0;
   while(a[i]) b[j++]=a[i++];
   b[j]=0;
}

void bigintpowi(int a,int p,char *r)
{
   char b[MAXD],c[MAXD];

   if(a==1 || p==0) { strcpy(r,"1"); return; }
   strcpy(r,"1");
   sprintf(b,"%d",a);
   if(p&1 && p>=1) { strcpy(c,b); strcpy(r,c); }
   bigintmul(b,b,b); if(p&2) bigintmul(r,b,r);
   if(p<4) return;
   bigintmul(b,b,b); if(p&4) bigintmul(r,b,r);
   if(p<8) return;
   bigintmul(b,b,b); if(p&8) bigintmul(r,b,r);
   if(p<16) return;
   bigintmul(b,b,b); if(p&16) bigintmul(r,b,r);
   if(p<32) return;
   bigintmul(b,b,b); if(p&32) bigintmul(r,b,r);
   if(p<64) return;
   bigintmul(b,b,b); if(p&64) bigintmul(r,b,r);
   if(p<128) return;
   bigintmul(b,b,b); if(p&128) bigintmul(r,b,r);
   if(p<256) return;
   bigintmul(b,b,b); if(p&256) bigintmul(r,b,r);
   if(p<512) return;
   bigintmul(b,b,b); if(p&512) bigintmul(r,b,r);
   if(p<1024) return;
   bigintmul(b,b,b); if(p&1024) bigintmul(r,b,r);
   if(p<2048) return;
   bigintmul(b,b,b); if(p&2048) bigintmul(r,b,r);
   if(p<4096) return;
   bigintmul(b,b,b); if(p&4096) bigintmul(r,b,r);
   if(p<8192) return;
   bigintmul(b,b,b); if(p&8192) bigintmul(r,b,r);
   if(p<16384) return;
   bigintmul(b,b,b); if(p&16384) bigintmul(r,b,r);
   if(p<32768) return;
   bigintmul(b,b,b); if(p&32768) bigintmul(r,b,r);
   if(p<65536) return;
   bigintmul(b,b,b); if(p&65536) bigintmul(r,b,r);
   if(p<131072) return;
   bigintmul(b,b,b); if(p&131072) bigintmul(r,b,r);
   if(p<262144) return;
   bigintmul(b,b,b); if(p&262144) bigintmul(r,b,r);
   if(p<524288) return;
   bigintmul(b,b,b); if(p&524288) bigintmul(r,b,r);
   if(p<1048576) return;
   bigintmul(b,b,b); if(p&1048576) bigintmul(r,b,r);
   if(p<2097152) return;
   bigintmul(b,b,b); if(p&2097152) bigintmul(r,b,r);
   if(p<4194304) return;
   bigintmul(b,b,b); if(p&4194304) bigintmul(r,b,r);
   if(p<8388608) return;
   bigintmul(b,b,b); if(p&8388608) bigintmul(r,b,r);
   if(p>=16777216) p=1/0;
}

