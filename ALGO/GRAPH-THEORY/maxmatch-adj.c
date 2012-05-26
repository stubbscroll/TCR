#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*  maximum matching in non-bipartite graph! (adjacency matrix)
    g[i][j]=1 if edge, i,j<n. remember back edge g[j][i]=1!
    returns number of edges in match
    runtime: O(n^3)
*/

/*  OK Topcoder TCO2006 round 1 350, worst case 3 ms, N=18 */
/*  OK UVa 11439 0.024 seconds, N=64 */
/*  OK UVa-live 3820 (Pacific 2007 problem I) 0.016 seconds, N=200 */
/*  OK UVa-live 4130 (Pacific 2008 problem H) 0.032 seconds, N=300 */
/*  OK GCJ 2008 AMER semifinal problem D, N=225 */
/*	OK Topcoder SRM 477 div 1-500, 7 ms, N<=200, 24.05.2012 */

/*  graph */
#define MAXV 64
char g[MAXV][MAXV];
int n;

/*  variables for maximum matching */
int mu[MAXV];
int phi[MAXV]; 
int rho[MAXV];
char scanned[MAXV];
int un[MAXV];
int distx[MAXV],disty[MAXV];
#define ISOUTER(i) (mu[(i)]==(i) || phi[mu[(i)]]!=mu[(i)])
#define OUTOFFOREST(i) (mu[(i)]!=(i) && phi[(i)]==(i) && phi[mu[(i)]]==mu[(i)])
void follow(int *dist,int v,int max) {
  int d=0;
  un[v]++;
  dist[v]=d++;
  if(v==max) return;
  while(1) {
    v=mu[v];
    if(dist[v]>-1) break;
    un[v]++;
    dist[v]=d++;
    if(v==max) break;
    v=phi[v];
    if(dist[v]>-1) break;
    un[v]++;
    dist[v]=d++;
    if(v==max) break;
  }
}

void pathunion(int x,int y,int max) {
  int i;
  for(i=0;i<n;i++) distx[i]=disty[i]=-2,un[i]=0;
  follow(distx,x,max);
  follow(disty,y,max);
}

int maximummatch() {
  int x,y,i,r;
  for(x=0;x<n;x++) mu[x]=phi[x]=rho[x]=x,scanned[x]=0;
  while(1) {
    for(x=0;x<n;x++) if(ISOUTER(x) && !scanned[x]) break;
    if(x==n) break;
  three:
    for(y=0;y<n;y++) if(g[x][y] && (OUTOFFOREST(y) || 
      (ISOUTER(y) && rho[x]!=rho[y]))) break;
    if(y==n) { scanned[x]=1; continue; }
    if(OUTOFFOREST(y)) {
      phi[y]=x;
      goto three;
    }
    pathunion(x,y,-1);
    for(i=0;i<n;i++) if(un[i]>1) break;
    if(i==n) {
      for(i=0;i<n;i++) if((distx[i]|disty[i])&1) {
        mu[phi[i]]=i;
        mu[i]=phi[i];
      }
      mu[x]=y;
      mu[y]=x;
      for(i=0;i<n;i++) phi[i]=i,rho[i]=i,scanned[i]=0;
      continue;
    }
    for(r=-1,i=0;i<n;i++) if(un[i]>1 && rho[i]==i)
      if(r==-1 || distx[i]<distx[r]) r=i;
    if(r<0) printf("error");
    pathunion(x,y,r);
    for(i=0;i<n;i++) if(((distx[i]|disty[i])&1) && rho[phi[i]]!=r)
      phi[phi[i]]=i;
    if(rho[x]!=r) phi[x]=y;
    if(rho[y]!=r) phi[y]=x;
    for(i=0;i<n;i++) if(un[rho[i]]) rho[i]=r;
    goto three;
  }
  for(x=y=0;x<n;x++) if(mu[x]>x) y++;
  return y;
}
