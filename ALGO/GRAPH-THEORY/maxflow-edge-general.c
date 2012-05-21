#include <stdio.h>
#include <string.h>

/*  max flow with edge cost 1, but arbitrary capacities. efficient memory
    usage: O(E) */

/*	OK Topcoder SRM 422 div-1 1000, 45 ms, n<=1802, ne<=405000, 21.05.2012 */

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

/*  for each edge a->b, find index to b->a */
void inverseedges() {
  static int starts[MAXV+1];
  int i;
  memcpy(starts,gs,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) inv[i]=starts[to[i]]++;
}

char map[512][512];
int nodeno[512][512];
int x,y;

int dx[]={1,0,-1,0},dy[]={0,1,0,-1};

int main() {
  int cases,i,j,na,nb,nc,d,x2,y2,no,bb,np;
  int source=0,sink,starta=1,startb,startc;
  scanf("%d",&cases);
  while(cases--) {
    scanf("%d %d",&x,&y);
    for(i=0;i<x;i++) scanf("%s",map[i]);
    /*  na: number of white on even-numbered columns
        nb: number of white on odd-numbered columns */
    na=nb=nc=0;
    for(i=0;i<x;i++) for(j=0;j<y;j++) {
      if(map[i][j]=='B') nc++;
      else if(map[i][j]=='W') {
        if(j&1) nb++;
        else na++;
      }
    }
    /*  set up graph for "tripartite" matching */
    if(na!=nb || na!=nc) { puts("NO"); continue; }
    np=na;
    startb=starta+na;
    startc=startb+nb+nb;
    sink=startc+nc;
    n=sink+1;
    na=1;
    nb=startc;
    nc=startb;
    for(i=0;i<x;i++) for(j=0;j<y;j++) {
      if(map[i][j]=='B') nodeno[i][j]=nc++;
      else if(map[i][j]=='W') {
        if(j&1) nodeno[i][j]=nb++;
        else nodeno[i][j]=na++;
      }
    }
    ne=0;
    for(i=0;i<np;i++) {
      /*  edges from source to all even white */
      from[ne]=source; to[ne++]=i+starta;
      from[ne]=i+starta; to[ne++]=source;
      /*  edges from odd whites to sink */
      from[ne]=startc+i; to[ne++]=sink;
      from[ne]=sink; to[ne++]=startc+i;
      /*  split black node */
      from[ne]=i+startb; to[ne++]=i+startb+np;
      from[ne]=i+startb+np; to[ne++]=i+startb;
    }
    for(i=0;i<x;i++) for(j=0;j<y;j++) if(map[i][j]=='B') {
      bb=nodeno[i][j];
      for(d=0;d<4;d++) {
        x2=i+dx[d],y2=j+dy[d];
        if(x2<0 || y2<0 || x2>=x || y2>=y || map[x2][y2]!='W') continue;
        no=nodeno[x2][y2];
        if(no<bb) {
          /*  node to the left */
          from[ne]=no; to[ne++]=bb;
          from[ne]=bb; to[ne++]=no;
        } else {
          /*  node to the right */
          from[ne]=no; to[ne++]=bb+np;
          from[ne]=bb+np; to[ne++]=no;
        }
      }
    }
    radixsort();
    inverseedges();
/*    for(i=0;i<ne;i++) printf("edge %d: %d -> %d (inv %d)\n",i,from[i],to[i],inv[i]);*/
    /*  hackish flow init: only forward edges have flow */
    for(i=0;i<ne;i++) f[i]=from[i]<to[i];
    if(maxflow(source,sink)==np) puts("YES");
    else puts("NO");
  }
  return 0;
}
