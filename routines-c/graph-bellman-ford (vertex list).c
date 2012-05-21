#define MAX 1000
#define INF 1000000000

int gv[MAX][MAX];
int gd[MAX][MAX];  
int gn[MAX];
int n,m;

int d[MAX],pi[MAX];

void initss(int start)
{
   int i;

   for(i=0;i<n;i++) { d[i]=INF; pi[i]=-1; }
   d[start]=0;
}

void relax(int u,int v,int i)
{
   if(d[v]>d[u]+gd[u][i]) { d[v]=d[u]+gd[u][i]; pi[v]=u; }
}

int bellmanford(int start)
{
   int i,j,k;

   initss(start);
   for(k=0;k<n-1;k++) for(i=0;i<n;i++) for(j=0;j<gn[i];j++) relax(i,gv[i][j],j);
   for(i=0;i<n;i++) for(j=0;j<gn[i];j++) if(d[gv[i][j]]>d[i]+gd[i][j]) return 0;
   return 1;
}

