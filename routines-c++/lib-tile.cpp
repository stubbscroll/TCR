vector<vs> tiles;
vector<char> t;

void floodcut(int i,int j,vs &g,vs &cur)
{
   if(i<0 || j<0 || i>=g.si || j>=g[0].si) return;
   if(!t[g[i][j]]) return;
   cur[i][j]=g[i][j];
   g[i][j]=' ';
   floodcut(i-1,j,g,cur);
   floodcut(i+1,j,g,cur);
   floodcut(i,j-1,g,cur);
   floodcut(i,j+1,g,cur);
}

void normalise(vs &a,vs &b)
{
   vs ny;
   int x1=999,x2=0,y1=999,y2=0;
   fir(a.si) fjr(a[0].si) if(t[a[i][j]]) {
      x1<?=i; y1<?=j; x2>?=i; y2>?=j;
   }
   fr(i,x1,x2+1) ny.pb(a[i].substr(y1,y2-y1+1));
   b=ny;
}

vs rotatetile(vs &a)
{
   vs ny(a[0].si,string(a.si,' '));
   fir(a.si) fjr(a[0].si) ny[a[0].si-j-1][i]=a[i][j];
   return ny;
}

int equaltiles(vs &a,vs &b)
{
   if(a==b) return 1;
   vs c=rotatetile(b);
   if(a==c) return 1;
   c=rotatetile(c);
   if(a==c) return 1;
   c=rotatetile(c);
   return a==c;
}

void cutout(vs &g,string acc)
{
   int x=g.si,y=g[0].si;
   t.resize(128,0);
   fir(acc.si) t[acc[i]]=1;
   fir(x) fjr(y) if(t[g[i][j]]) {
      vs cur(x,string(y,' '));
      floodcut(i,j,g,cur);
      normalise(cur,cur);
      fkr(tiles.si) if(equaltiles(tiles[k],cur)) goto tileexists;
      tiles.pb(cur);
   tileexists:;
   }
}

void printboard(vs &a)
{
   fir(a.si) puts(a[i].c_str());
   puts("-------");
}

