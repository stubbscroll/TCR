#include <stdio.h>
#include <string.h>

/*  specialized max flow routine, only works when all capacities are 1,
    and is only extremely efficient if the only node with a high outdegree
    is the source. recommended for tripartite matching where each center node
    has rather few possibilities on both sides.
    
    how to use: try to arrange the graph as a DAG going from left to right
    such that an edge (a,b) satisfies a<b. then, add all edges (both ways!)
    to from[],to[], and set n and ne to number of nodes/edges. then
    call radixsort() and inverseedges(). then, set f[i] to 1 if i represents
    a forward edge (going right), 0 otherwise. then call maxflow().
    oh, and remember to split the center node when doing tripartite matching.
    
    (this routine _might_ also work if all capacities are just set to 1 both
    ways)
*/

/*  OK SPOJ 9890 (NWERC 2011 problem D), n<=333335, 2.32 seconds 30.11.2011 */
/*  TODO test more. test on all bipartite matching problems */
#define MAXE 2000000
#define MAXV 333500
#define INF 1000000000

/*  TODO consider interleaving all these in a struct or something
    for cache-friendliness */
int from[MAXE],to[MAXE];  /*  graph */
int gs[MAXV+1];           /*  pointer to first node */
int f[MAXE];              /*  flow/capacity/residual graph */
int inv[MAXE];            /*  index to reverse edge */
int n,ne;                 /*  number of nodes, edges */

/*  NB! force starting move heuristic assumes that all capacities==1 */
int maxflow(int source,int sink) {
  int i,j,flow=0,qe=0,k,l,last=gs[source];
  static char t[MAXV];
  static int parent[MAXV],q[MAXV];
  memset(t,0,n);
  memset(parent,-1,n*sizeof(int));
  for(last=gs[source];last<gs[source+1];last++) {
    t[source]=t[to[last]]=1;
    q[qe++]=source;
    q[qe++]=to[last];
    parent[to[last]]=last;
    for(k=1;k<qe;k++) if(t[i=q[k]]) for(l=gs[i];l<gs[i+1];l++) {
      j=to[l];
      if(!t[j] && f[l]>0) {
        t[j]=1; parent[j]=l;
        q[qe++]=j;
        if(j==sink) goto out;
      }
    }
    /*  if all left edges must be matched, have a break here.
        if not, put a continue here */
    break;
/*    continue; */
  out:
    l=parent[sink];
    while(l>-1) {
      f[l]--;
      f[inv[l]]++;
      l=parent[from[l]];
    }
    flow++;
    while(qe) {
      j=q[--qe];
      t[j]=0; parent[j]=-1;
    }
  }
  return flow;
}

void radixsort() {
  static int newfrom[MAXE],newto[MAXE];
  int i,j;
  /*  sort on to */
  memset(gs,0,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) gs[to[i]]++;
  for(i=1;i<n;i++) gs[i]+=gs[i-1];
  for(i=ne-1;i>=0;i--) {
    j=--gs[to[i]];
    newto[j]=to[i];
    newfrom[j]=from[i];
  }
  /*  (stable) sort on from */
  memset(gs,0,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) gs[newfrom[i]]++;
  for(i=1;i<n;i++) gs[i]+=gs[i-1];
  gs[n]=ne;
  for(i=ne-1;i>=0;i--) {
    j=--gs[newfrom[i]];
    to[j]=newto[i];
    from[j]=newfrom[i];
  }
}

/*  for each edge a->b, find index to b->a */
void inverseedges() {
  static int starts[MAXV+1];
  int i;
  memcpy(starts,gs,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) inv[i]=starts[to[i]]++;
}
