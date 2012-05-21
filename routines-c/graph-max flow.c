#define MAXV 100
#define SOURCE 0
#define SINK 99

int n;

int maxflow()
{
   int i,j,done,flow,done2,a,r,z;
   char t[MAXV];
   int parent[MAXV];
   int min[MAXV];

   flow=0;
   do {
      done=1;
      memset(t,0,n);
      for(i=0;i<n;i++) { parent[i]=-1; min[i]=INF; }
      t[SOURCE]=1;
      do {
         done2=1;
         for(i=0;i<n;i++) if(t[i]) {
/*            for(j=0;j<n;j++) { if(t[j] || !f[i][j]) continue;*/ /* adjacency matrix */
            for(z=0;z<gn[i];z++) { j=gv[i][z]; if(t[j] || !f[i][j]) continue; /* vertex list */
               a=f[i][j];
               t[j]=1; parent[j]=i; done2=0;
               if(min[i]<a) min[j]=min[i]; else min[j]=a;
               if(j==SINK) {
                  done=0; goto out;
               }
            }
         }
      } while(!done2);
      break;
   out:
      i=SINK; r=min[SINK];
      while(i!=-1) {
         j=parent[i]; if(j==-1) break;
         f[j][i]-=r; f[i][j]+=r;
         i=j;
      }
      flow+=r;
   } while(!done);
   return flow;
}

