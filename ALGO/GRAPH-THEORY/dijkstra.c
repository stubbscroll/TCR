#include <stdio.h>
#include <string.h>

/*  regular dijkstra! (with binary heap)
    tips:
    - to eke out the last inch of speed, unroll the neighbours loop and
      hardcode each case (can be done in a 2d grid graph, for instance)
*/
/*  OK UVa 10986 n<=20000,e<=100000 0.236 seconds, 16.08.2011 */
/*  OK UVa-live 4374 (NEERC 2008 problem D) n<=4008004,e<=~16000000 7.724 seconds 16.08.2011 */
/*  OK SPOJ 3465 DRIVE (NEERC 2008 problem D) n<=4008004,e<=~16000000 30.59 seconds 16.08.2011 */
/*  OK TJU 3132 (NEERC 2008 problem D) n<=4008004,e<=~16000000 9.11 seconds 16.08.2011 */
/*  OK UVa 11367 (NCPC 2007 problem F) n<=100000 0.108 seconds 29.09.2011 */
/*	OK IDI Open 2012 problem F (Longest Common Path) n<=2000 21.05.2012 */
/*	OK IDI Open 2012 problem H (Holey Road) n<=40000 21.05.2012 */

/*  dijkstra with costs on nodes! (see below for code) */
/*  OK UVa 929 0.588 seconds 16.08.2011 */

#define MAXV 20000
#define MAXE 100000
/*  TODO i have no idea of a lower bound for MAXH given n,ne */
#define MAXH 1000000

int heap[MAXH];   /*  heap value (on which it is sorted)  */
int heapid[MAXH]; /*  heap id */
int heapn;        /*  (number of elements in heap)+1 */

/*  insert a new value to the heap */
/*  warning, function will choke if heap is full */
void heapinsert(int val,int id) {
  int pos=heapn++;
  while(pos>1 && val<heap[pos>>1]) {
    heap[pos]=heap[pos>>1];
    heapid[pos]=heapid[pos>>1];
    pos>>=1;
  }
  heap[pos]=val; heapid[pos]=id;
}

/*  remove a value from the heap and put it in *val,*id */
void heapremove(int *val,int *id) {
  int pos=1,next=1,newval,newid;
  *val=heap[1]; *id=heapid[1];
  newval=heap[--heapn];
  newid=heapid[heapn];
  while((pos<<1)<=heapn) {
    /*  take lowest of left and right children */
    if((pos<<1)==heapn) next=pos<<1;
    else next=(pos<<1)+(heap[pos<<1]>heap[(pos<<1)+1]);
    if(heap[next]>=newval) break;
    heap[pos]=heap[next];
    heapid[pos]=heapid[next];
    pos=next;
  }
  heap[pos]=newval;
  heapid[pos]=newid;
}

int n,ne;               /*  number of nodes, number of edges */
int from[MAXE],to[MAXE];/*  edge i: from[i] to[i] */
int cost[MAXE];         /*  cost of edge from[i] -> to[i] */
int gs[MAXV+1];         /*  gs[i]: start of edges from i, gs[i+1]: end */

/*  return -1 on no path */
int dijkstra(int start,int goal) {
  static int dist[MAXV];    /*  array of distances */
  static char vis[MAXV];    /*  array indicating visited (processed) nodes */
  int val,w,cur,i,x2;
  if(start==goal) return 0;
  memset(vis,0,n);
  memset(dist,126,n*sizeof(int));
  dist[start]=0;              /*  set distance to start to 0 */
  heapn=1;
  heapinsert(0,start);        /*  insert start node to heap */
  while(heapn>1) {
    heapremove(&val,&cur);
    if(vis[cur]) continue;    /*  if current node is already done: continue */
    if(cur==goal) return val; /*  done! */
    vis[cur]=1;
    for(i=gs[cur];i<gs[cur+1];i++) {
      x2=to[i];
      if(vis[x2]) continue;
      w=val+cost[i];
      if(dist[x2]>w) {
        dist[x2]=w;
        heapinsert(w,x2);
      }
    }
  }
  return -1;
}

/*  version of countingsort supporting edge costs */
void countingsort() {
  static int newto[MAXE];
  static int newcost[MAXE];
  int i,j;
  for(i=0;i<=n;i++) gs[i]=0;
  for(i=0;i<ne;i++) gs[from[i]]++;
  for(i=1;i<n;i++) gs[i]+=gs[i-1];
  gs[n]=ne;
  for(i=0;i<ne;i++) {
    j=--gs[from[i]];
    newto[j]=to[i];
    newcost[j]=cost[i];
  }
  for(i=0;i<ne;i++) to[i]=newto[i],cost[i]=newcost[i];
  for(i=0;i<n;i++) for(j=gs[i];j<gs[i+1];j++) from[j]=i;
}

/*  dijkstra template */
int dijkstra_template(int start,int goal) {
  static int dist[];    /*  array of distances */
  static char vis[];    /*  array indicating visited (processed) nodes */
  int val,w,cur;
  if(start==goal) return 0;
  /*  set visited to 0 */
  /*  set distances to INF */
  dist[start]=0;              /*  set distance to start to 0 */
  heapn=1;
  heapinsert(0,start);        /*  insert start node to heap */
  while(heapn>1) {
    heapremove(&val,&cur);
    if(vis[cur]) continue;    /*  if current node is already done: continue */
    if(cur==goal) return val; /*  done! */
    vis[cur]=1;
    for(neighbours to cur) {
      x2=potential neighbour;
      if(x2 is out of bounds) continue;
      if(vis[x2]) continue;
      w=val+calcdist(cur,x2);
      if(dist[x2]>w) {
        dist[x2]=w;
        heapinsert(w,x2);
      }
    }
  }
  return INF;
}

int nodecost[MAXV];  /*  node costs for graph */

/*  dijkstra template (for costs on nodes) */
/*  special case of dijkstra: edge cost is associated with node.
    - when a node is scanned, we know the optimal path
      - hence, whenever we scan the goal node, exit
    - a node is pushed only once
    (warning, nodecost[] is destroyed)
*/
int dijkstranodecost_template(int start,int goal) {
  int val,cur,x2;
  if(start==goal) return nodecost[start];
  heapn=1;
  heapinsert(nodecost[start],start);
  g[start]=-1;
  while(heapn>1) {
    heapremove(&val,&cur);
    for(neighbours to cur) {
      x2=neighbour
      if(x2 is out of range, or nodecost[x2]<0) continue;
      if(x2==goal) return val+nodecost[x2];
      heapinsert(val+nodecost[x2],x2);
      nodecost[x2]=-1;
    }
  }
  return -1;
}
