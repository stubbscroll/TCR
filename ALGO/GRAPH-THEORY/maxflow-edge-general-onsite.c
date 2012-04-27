#include <stdio.h>
#include <string.h>

#define MAXV 5002
#define MAXE 200000
#define INF 1000000000

/*  maxflow! uses edge lists and assumes that cost=1 everywhere.
    uses bfs to find augmenting paths.
    however, memory usage is MAXV*MAXV.
    
    in offsite contests, please use maxflow-edge-general. this routine
    is rather short, and is suitable for onsite contests.

    usage: (no need to init f[][]!)
    - set MAXV,MAXE to guaranteed maximum values for the problem
    - set n to desired number of nodes, set ne=0
    - add edges using the macro addedge(from,to,capacity), no need to set
      backedges
    - call countingsort(), then maxflow(source,sink)

    nb! the above only works if an edge only occurs once and in one
    direction. if both (a,b), (b,a) exist or multiple (a,b) exist, then
    init f[][] first, and update f[][], from[], to[] manually instead of
    using the macro
*/
int f[MAXV][MAXV];      /*  initial flow, 0 means no edge */
int n,ne;               /*  number of nodes, number of edges */
int from[MAXE],to[MAXE];/*  edge i: from[i] to[i] */
int gs[MAXV+1];         /*  gs[i]: start of edges from i, gs[i+1]: end */

/*  OK UVa 563 0.124 seconds, n<=5002, 13.08.2011 */
int maxflow(int source,int sink) {
  int i,j,done,flow=0,done2,a,r,qe=0,k,l;
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
      for(k=0;k<qe;k++) if(t[i=q[k]]) for(l=gs[i];l<gs[i+1];l++) {
        j=to[l];
        if(!t[j] && f[i][j]>0) {
          a=f[i][j];
          t[j]=1; parent[j]=i; done2=0;
          q[qe++]=j;
          if(min[i]<a) min[j]=min[i]; else min[j]=a;
          if(j==sink) { done=0; goto out; }
        }
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

void countingsort() {
  static int newto[MAXE];
  int i,j;
  for(i=0;i<=n;i++) gs[i]=0;
  for(i=0;i<ne;i++) gs[from[i]]++;
  for(i=1;i<n;i++) gs[i]+=gs[i-1];
  gs[n]=ne;
  for(i=0;i<ne;i++) {
    j=--gs[from[i]];
    newto[j]=to[i];
  }
  for(i=0;i<ne;i++) to[i]=newto[i];
  for(i=0;i<n;i++) for(j=gs[i];j<gs[i+1];j++) from[j]=i;
}

/*  add an edge */
#define addedge(a,b,c) from[ne]=a,to[ne++]=b,from[ne]=b,to[ne++]=a,f[a][b]=c,f[b][a]=0
