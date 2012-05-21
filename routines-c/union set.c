#define MAXV 30000

int p[MAXV],rank[MAXV];

void linkset(int x,int y)
{
   if(rank[x]>rank[y]) p[y]=x;
   else {
      p[x]=y;
      if(rank[x]==rank[y]) rank[y]++;
   }
}

int findset(int x)
{
   if(x!=p[x]) p[x]=findset(p[x]);
   return p[x];
}

void unionset(int x,int y)
{
   linkset(findset(x),findset(y));
}

void initset(int n)
{
   int i;

   for(i=0;i<n;i++) { p[i]=i; rank[i]=0; }
}

