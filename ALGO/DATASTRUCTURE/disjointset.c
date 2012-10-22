#define MAXSET 30000

/* disjoint set! */
/* OK UVa 10147 08.06.2012 */
/* OK UVa 10301 08.06.2012 */
/* OK UVa 10397 26.05.2012 */
/* OK NCPC 2009 H "beacons" (UVa 11696) 05.10.2012 */

int p[MAXSET],rank[MAXSET];

int findset(int x) {
  if(x!=p[x]) p[x]=findset(p[x]);
  return p[x];
}

void unionset(int x,int y) {
  x=findset(x);
  y=findset(y);
  if(x==y) return;
  if(rank[x]>rank[y]) p[y]=x;
  else {
    p[x]=y;
    if(rank[x]==rank[y]) rank[y]++;
  }
}

void initset(int n) {
  int i;
  for(i=0;i<n;i++) { p[i]=i; rank[i]=0; }
}
