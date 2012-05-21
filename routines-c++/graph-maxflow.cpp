//TODO: min-cost max-flow
//TODO: max-flow with edge list

//================== maximum flow =====================

// 10.02.2007, tested on srm 334 div 1 hard, ok (worst case 15 ms)

/* find the maximum flow from source to sink.
   uses custom search to find augmenting paths.
   input g is an adjacency
   matrix, where 0 indicates no edge. */

#define INF 1000000000

int maxflow(vvi &g,int source,int sink)
{
   int n=g.si,done,flow=0,done2,a,r;
   vi parent(n),min(n),t(n);
   vvi f=g;
   do {
      done=1;
      fir(n) { min[i]=INF; t[i]=0; parent[i]=-1; }
      t[source]=1;
      do {
         done2=1;
         fir(n) if(t[i]) {
            fjr(n) { 
               if(t[j] || !f[i][j]) continue;
               a=f[i][j];
               t[j]=1; parent[j]=i; done2=0;
               if(min[i]<a) min[j]=min[i]; else min[j]=a;
               if(j==sink) {
                  done=0; goto out;
               }
            }
         }
      } while(!done2);
      break;
   out:
      a=sink; r=min[sink];
      while(a!=-1) {
         int j=parent[a]; if(j<0) break;
         f[j][a]-=r; f[a][j]+=r;
         a=j;
      }
      flow+=r;
   } while(!done);
   return flow;
}

//================== bipartite matching =====================

// 14.02.2007, tested on srm 236 div 1 hard, ok

/* bipartite matching
   return the number of matches in bipartite graph
   g: edge matrix, n1*n2 elements: 1=edge exists, 0=no edge */

int bipartite(vvi &g)
{
   vvi f=g;
   int n1=g.si,n2=g[0].si,m=0;
   vi a(n1,0),b(n2,0),t(n1+n2),parent(n1+n2);
   fir(n1) {
      // find augmenting path from i
      int done,match;
      fjr(n1+n2) t[j]=0,parent[j]=-1;
      t[i]=1;
      do {
         done=1;
         fjr(n1) if(t[j]) {
            fkr(n2) if(!t[n1+k] && f[j][k]>0) {
               t[n1+k]=1; parent[n1+k]=j; done=0;
               if(!b[k]) {
                  a[i]=b[k]=1; m++; match=n1+k; goto out;
               }
            }
         }
         fjr(n2) if(t[n1+j]) {
            fkr(n1) if(!t[k] && f[k][j]<0) {
               t[k]=1; parent[k]=n1+j; done=0;
            }
         }
      } while(!done);
      continue;
   out:
      while(1) {
         int j=parent[match]; if(j<0) break;
         if(j>=n1) f[match][j-n1]=-f[match][j-n1];
         else f[j][match-n1]=-f[j][match-n1];
         match=j;
      }
   }
   return m;
}

// 14.02.2007, tested on srm 236 div 1 hard, ok (worst case 31 ms)

/* bipartite match (bisection)
   uses binary search to find the lowest value v such that there is a
     bipartite matching between n1 elements with no edge cost>v used
   t1: lower bound - 1 for binary search
   t3: upper bound for binary search
   g: weighted undirected bipartite graph with n1*n2 elements, n1<=n2 */

int bisectbipartite(int t1,int t3,vvi &g)
{
   int n1=g.si,n2=g[0].si;
   vvi h=vvi(n1,vi(n2,0));
//   vvi h=makevvi(n1,n2);
   while(t1<t3-1) {
      int t2=(t1+t3)>>1;
      fir(n1) fjr(n2) h[i][j]=(g[i][j]<=t2);
      int r=bipartite(h);
      if(r==n1) t3=t2;
      else t1=t2;
   }
   return t3;
}

