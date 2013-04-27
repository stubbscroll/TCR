#include <stdio.h>
#include <string.h>

#define MAXV 100
#define INF 1000000000

/* maxflow! uses adjacency matrix */

int f[MAXV][MAXV]; /* capacity, 0 means no edge */
int n;

/* slower maxflow, but shorter to type in. faster search than bfs for
   adjacency matrices, but slow array clearing */
/* OK UVa 753 0.028 seconds, n<=502, 13.08.2011 */
/* OK UVa 820 0.032 seconds, n<=100, 12.08.2011 */
/* OK UVa 10330 0.048 seconds, n<=202, 13.08.2001 */
/* OK UVa 10480 0.004 seconds, n<=50, 14.08.2011 */
int maxflow(int source,int sink) {
  int i,j,done,flow=0,done2,a,r;
  static char t[MAXV];
  static int parent[MAXV],min[MAXV];
  do {
    done=1;
    memset(t,0,n);
    memset(parent,-1,n*sizeof(int));
    memset(min,126,n*sizeof(int));
    t[source]=1;
    do {
      done2=1;
      for(i=0;i<n;i++) if(t[i]) for(j=0;j<n;j++) if(!t[j] && f[i][j]>0) {
        a=f[i][j];
        t[j]=1; parent[j]=i; done2=0;
        if(min[i]<a) min[j]=min[i]; else min[j]=a;
        if(j==sink) { done=0; goto out; }
      }
    } while(!done2);
    break;
  out:
    i=sink; r=min[sink];
    while(i!=-1) {
      j=parent[i]; if(j==-1) break;
      f[j][i]-=r; f[i][j]+=r;
      i=j;
    }
    flow+=r;
  } while(!done);
  return flow;
}

/* faster maxflow, uses BFS and smart array clearing */
/* OK UVa 753 0.020 seconds, n<=502, 13.08.2011 */
/* OK UVa 820 0.020 seconds, n<=100, 12.08.2011 */
/* OK UVa 10330 0.032 seconds, n<=202, 13.08.2001 */
/* OK UVa 10480 0.004 seconds, n<=50, 14.08.2011 */
int fastmaxflow(int source,int sink) {
  int i,j,done,flow=0,done2,a,r,qe=0,k;
  static char t[MAXV];
  static int parent[MAXV],min[MAXV],q[MAXV];
  memset(t,0,n);
  memset(parent,-1,n*sizeof(int));
  memset(min,126,n*sizeof(int));
  do {
    done=1;
    t[source]=1;
    q[qe++]=source;
    do {
      done2=1;
      for(k=0;k<qe;k++) for(i=q[k],j=0;j<n;j++) if(!t[j] && f[i][j]>0) {
        a=f[i][j];
        t[j]=1; parent[j]=i; done2=0;
        q[qe++]=j;
        if(min[i]<a) min[j]=min[i]; else min[j]=a;
        if(j==sink) { done=0; goto out; }
      }
    } while(!done2);
    break;
  out:
    i=sink; r=min[sink];
    while(i!=-1) {
      j=parent[i]; if(j==-1) break;
      f[j][i]-=r; f[i][j]+=r;
      i=j;
    }
    flow+=r;
    while(qe) {
      j=q[--qe];
      t[j]=0; parent[j]=-1; min[j]=INF;
    }
  } while(!done);
  return flow;
}

#define MAXE 1000
int cutfrom[MAXE],cutto[MAXE],cutn;

/* find cheapest minimum cut in a graph (that is, set of edges of minimum
   weight resulting in two disjoint subgraphs with the source and sink
   separated).
   do a bfs from the source to find all reachable nodes.
   the mincut is the set of edges connecting reachable nodes to unreachable
   nodes.
   edges included in the min cut are in cutfrom[],cutto[],cutn.
   maxflow() must be run first */
/* OK UVa 10480 0.004 seconds, n<=50, 14.08.2011 */
void mincut(int source) {
  static int q[MAXV];
  static char t[MAXV];
  int qe=0,i,j,k;
  cutn=0;
  memset(t,0,n);
  q[qe++]=source;
  t[source]=1;
  for(k=0;k<qe;k++) for(i=q[k],j=0;j<n;j++) if(!t[j] && f[i][j]>0) {
    q[qe++]=j; t[j]=1;
  }
  for(i=0;i<n;i++) if(t[i]) for(j=0;j<n;j++) if(!t[j] && f[j][i]>0) {
    cutfrom[cutn]=i; cutto[cutn++]=j;
  }
}
