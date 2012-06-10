#include <stdio.h>
#include <string.h>

/* max flow with edge cost 1, but arbitrary capacities. efficient memory
   usage: O(E) */
/* remember, f[] (initial capacities) must be initialized. the easiest way is
   to define the graph from left to right, and set cap only on forward edges */

/* OK UVa 4.204 seconds n<=122 ne<=3620 08.06.2012 */
/* OK Topcoder SRM 422 div-1 1000, 45 ms, n<=1802, ne<=405000, 21.05.2012 */

#define MAXE 2000000
#define MAXV 333500
#define INF 1000000000

int from[MAXE],to[MAXE];  /*  graph */
int gs[MAXV+1];           /*  pointer to first node */
int f[MAXE];              /*  flow */
int inv[MAXE];            /*  index to reverse edge */
int n,ne;

int maxflow(int source,int sink) {
  int i,j,done,flow=0,done2,a,r,qe=0,k,l;
  static char t[MAXV];
  static int parent[MAXV],min[MAXV],q[MAXV];
  memset(t,0,n);
  memset(parent,-1,n*sizeof(int));
  memset(min,126,n*sizeof(int));
  /*  TODO change from bfs to dfs */
  do {
    done=1;
    t[source]=1;
    q[qe++]=source;
    do {
      done2=1;
      for(k=0;k<qe;k++) if(t[i=q[k]]) for(l=gs[i];l<gs[i+1];l++) {
        j=to[l];
        if(!t[j] && f[l]>0) {
          a=f[l];
          t[j]=1; parent[j]=l; done2=0;
          q[qe++]=j;
          if(min[i]<a) min[j]=min[i]; else min[j]=a;
          if(j==sink) { done=0; goto out; }
        }
      }
    } while(!done2);
    break;
  out:
    l=sink; r=min[sink];
    while(parent[l]>-1) {
      f[parent[l]]-=r;
      f[inv[parent[l]]]+=r;
      l=from[parent[l]];
    }
    flow+=r;
    while(qe) {
      j=q[--qe];
      t[j]=0; parent[j]=-1; min[j]=INF;
    }
  } while(!done);
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

/* for each edge a->b, find index to b->a */
void inverseedges() {
  static int starts[MAXV+1];
  int i;
  memcpy(starts,gs,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) inv[i]=starts[to[i]]++;
}

/* add both directions of an edge */
void addedge(int a,int b) {
	from[ne]=a; to[ne++]=b;
	from[ne]=b; to[ne++]=a;
}

