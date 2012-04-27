#define MAXSET 30000

int p[MAXRSET],rank[MAXSET];

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
