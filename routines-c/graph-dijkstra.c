#define INF 1000000
#define MAXV 100

int g[MAXV][MAXV],n;
char d[MAXV];
int dist[MAXV][MAXV],pi[MAXV][MAXV];

/* g: adjacency matrix, n:number of vertices */
void dijkstra_adj(int s)
{
   char S[MAXV],Q[MAXV];
   int qn=n,i,j,u;

   for(i=0;i<n;i++) {
      dist[s][i]=INF;
      pi[s][i]=-1;
   }
   dist[s][s]=0;
   memset(S,0,n);
   memset(Q,0,n);
   while(qn--) {
      for(u=-1,i=0;i<n;i++) if(!Q[i] && (u<0 || dist[s][u]>dist[s][i])) u=i;
      if(u==-1) break;
      S[u]=Q[u]=1;
      for(i=0;i<n;i++) if(g[u][i]<INF) {
         if(dist[s][i]>dist[s][u]+g[u][i]) {
            dist[s][i]=dist[s][u]+g[u][i];
            pi[s][i]=u;
         }
      }
   }
}

/* old dijkstra follows: */

#define MAXV 26
#define INF 2000000000

int g[MAXV][MAXV];  /* weighted adjacency matrix, INF: no edge */
int deg[MAXV];      /* vertex degree */
int parent[MAXV];

int dijkstra(int start,int end)
{
   int i,j;
   int intree[MAXV];
   int distance[MAXV];
   int v,w,weight,dist;

   for(i=0;i<n;i++) {
      intree[i]=0;
      distance[i]=INF;
      parent[i]=-1;
   }

   distance[start]=0;
   v=start;

   while(intree[v]==0) {
      intree[v]=1;
      for(i=0;i<n;i++)
         if(g[i][v]<INF) {
            w=i;
            weight=g[i][v];
            if(distance[w]>distance[v]+weight) {
               distance[w]=distance[v]+weight;
               parent[w]=v;
            }
         }
      v=0;
      dist=INF;
      for(i=0;i<n;i++)
         if(intree[i]==0 && dist>distance[i]) {
            dist=distance[i];
            v=i;
         }
   }
   return distance[end];
}

