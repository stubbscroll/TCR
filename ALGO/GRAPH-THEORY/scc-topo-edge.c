#include <stdio.h>
#include <string.h>

/*  find strongly connected components with edge lists! */

#define MAXV 20000
#define MAXE 50000
int n,ne;               /*  number of nodes, number of edges */
int from[MAXE],to[MAXE];/*  edge i: from[i] to[i] */
int gs[MAXV+1];         /*  gs[i]: start of edges from i, gs[i+1]: end */

char vis[MAXV];         /*  during dfs: 0:unvisited 1:under progress 2:done */
int res[MAXV];          /*  order of finalising in dfs */
int resn;               /*  counter */
char cycle;             /*  1: graph has cycle and no toposort exists */

int con[MAXV];          /*  id of connected component for vertex i */

/*  subroutine: sort from[],to[] on increasing from[] */
/*  (see maxflow-edge-general for a full radix sort which also sorts to[]) */
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

/*  start dfs from node u */
/*  before calling, set vis[] to 0, res[] to 0, resn to 0 */
/*  when calling from scc, c is component id, otherwise it's negative */
void dfsvisit(int u,int c) {
  static int stack[2*MAXV];
  int sp=0,at,v;
  stack[sp++]=u; stack[sp++]=gs[u];
  vis[u]=1;
  if(c>=0) con[u]=c;      /*  set component id */
  while(sp) {
    at=stack[--sp]; u=stack[--sp];
    while(1) {
      if(at==gs[u+1]) {
        vis[u]=2;
        res[resn++]=u;
        break;
      } else {
        v=to[at];
        if(vis[v]==1) { cycle=1; at++; } /*  may want to return if cycle */
        else if(!vis[v]) {
          vis[v]=1;
          if(c>=0) con[v]=c;  /*  set component id */
          stack[sp++]=u;
          stack[sp++]=at+1;
          at=gs[v]; u=v;
        } else at++;
      }
    }
  }
}

/* topological sort! res[] contains the nodes in backwards order */
/* OK UVa 103, 0.008 seconds, n<=30,ne<=420 25.05.2011 */
/* OK UVa 11686, 0.452 seconds, n,ne<=1000000 26.05.2012 */
/* OK UVa 12466, 0.692 seconds, n<=100000,ne<=999999 07.06.2012 */
void toposort() {
  int i;
  memset(vis,0,n);
  /*  may want to quit early if cycle */
  for(resn=cycle=i=0;i<n;i++) if(!vis[i]) dfsvisit(i,-1); 
}

/*  return the number of connected components */
/*  for vertex i, id of component is in con[i] */
/*  OK UVa 11709 0.860 seconds, n<=1000, ne<=999000 09.08.2011 */
/*  OK UVa-live 4287 (NWERC 2008 problem B), n<=20000, ne<=50000, 0.088 seconds, 09.08.2011 */
int scc() {
  static int dfsres[MAXV];
  int i,t,c=0;
  memset(vis,0,n);
  for(resn=i=0;i<n;i++) if(!vis[i]) dfsvisit(i,-1);
  /*  transpose graph */
  memcpy(dfsres,res,n*sizeof(int));
  for(i=0;i<ne;i++) t=from[i],from[i]=to[i],to[i]=t;
  countingsort();
  memset(vis,0,n);
  for(resn=0,i=n-1;i>=0;i--) if(!vis[dfsres[i]]) dfsvisit(dfsres[i],c++);
  return c;
}

/*  how to use:
    read n,ne (graph size)    }
    read graph in from[],to[] } <- init stage
    run counting sort         }

    for toposort: run toposort()
    for scc: run scc()

    the graph over the components have r nodes (return value from scc())
    and there is an edge from con[from[i]] to con[to[i]] whenever these
    values are different.
*/
    
int main() {
}
