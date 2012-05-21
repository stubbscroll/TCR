// breadth-first search
// assumes global x,y - size of map

void bfs(int sx,int sy,vs &g)
{
   deque<pii> q;
   vvi t=makevvi(x,y,INF); t[sx][sy]=0;
   q.pb(mp(sx,sy));
   while(!q.empty()) {
      int x1=q.front().first,y1=q.front().second; q.pop_front();
      fir(4) {
         int x2=x1+dx[i],y2=y1+dy[i],m=t[x1][y1]+1;
         if(x2<0 || x2>=x || y2<0 || y2>=y
           || g[x2][y2]=='X' // insert block tile here
           || t[x2][y2]<=m) continue;
         q.pb(mp(x2,y2));
         t[x2][y2]=m;
      }
   }
}
