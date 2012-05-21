//TODO: dijkstra
//TODO: bellman-ford
//TODO: johnson

vvi floydwarshall(vvi &g)
{
   vvi f=g; int n=f.si;
   fkr(n) fir(n) fjr(n) f[i][j]=min(f[i][j],f[i][k]+f[k][j]);
   return f;
}
