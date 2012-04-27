/*  NB, need to copy in heap routines */

/*  template for A*!
    - need macro HEUR(cur,goal) which returns an underestimate of the
      distance from cur to goal (can be equal to the real distance)
    - heap requirement is unknown, just make it large enough
    - it's preferable to use instead of dijkstra or bfs when:
      - there's always a path to from start to goal
      - H(cur) isn't 0 everywhere (ie. we aren't doing dijkstra)
*/
/*  OK UVa 851 n is unknown, 0.028 seconds 17.08.2011 */
/*  OK UVa 10181 n is unknown, 1.460 seconds 17.08.2011 */
/*  OK UVa-live 4374 (NEERC 2008 problem D) n<=4000000, e<=16000000 3.276 seconds 16.08.2011 */
/*  OK SPOJ 3465 (NEERC 2008 problem D) n<=4000000, e<=16000000 14.00 seconds, 16.08.2011 */
/*  OK TJU 3132 (NEERC 2008 problem D) n<=4000000, e<=16000000 3.28 seconds, 16.08.2011 */
int astar(int start,int goal) {
  static int dist[];            /*  distance array */
  static int parent[];          /*  add this if we need to restore path */
  static char vis[];            /*  1: node is closed */
  int i,cur,cx,cy,x2,y2,d,val;
  ll w,h;
  /*  set vis to 0 */
  /*  set dist to INF, or another upper bound for the answer */
  dist[start]=0;
  parent[start]=-1; /*  add this if we need to restore path */
  heapn=1;
  heapinsert(HEUR(start),start);
  while(heapn>1) {
    heapremove(&val,&cur);
    if(vis[cur]) continue;
    /*  if graph has different edge costs, put check here */
    if(cur==goal) return val;
    val=dist[cur];
    vis[cur]=1;
    for(neighbours to cur) {
      x2=potential neighbour
      if(x2 is out of bounds || vis[x2]) continue;
      /*  if graph has equal edge costs, put check here */
      /*  however, only keep one of them */
      if(cur==goal) return val;
      w=val+calcdist(x2,goal);
      h=HEUR(x2,goal);
      if(dist[x2]>w && w+h<INF) {
        dist[x2]=w;
        heapinsert(w+h,x2);
        parent[x2]=cur;   /*  add this if we need to restore path */
      }
    }
  }
  return -1;
}
