#include <stdio.h>
#include <string.h>

#define MAXV 100
#define INF 1000000000

/*  solve the assignment problem with the hungarian algorithm:
    find a maximum edge matching of people -> jobs which also maximizes 
    the edge sums
    num(people) must equal num(jobs)
    returns cost (sum of edge weights in match)
    to make a non-edge: enter a g[i][j] < -n*maxedge,
    nonperfect match if return value is negative (might need to make
    g[][] and return value long long)
    runtime: O(n^3)
*/
int g[MAXV][MAXV];    /*  g[i][j]: cost of person i doing job j */
int mate[2*MAXV];     /*  for each node, its match (or -1 if none) */
int n;

void augment(int *mate,int *p,int j) {
  int i,next;
  do {
    i=p[j]; mate[j+n]=i;
    next=mate[i]-n; mate[i]=j+n;
    if(next>-1) j=next;
  } while(next>-1);
}

/*  OK UVa 10072 0.012 seconds, n<=100, 10.08.2011 */
/*  OK UVa 10383 0.268 seconds, n<=500, 11.08.2001 */
/*  OK UVa 10888 0.012 seconds, n<=15, 10.08.2011 */
/*  OK Topcoder SRM 371 div 1 500, 2 ms worst case, n<=50, 10.08.2011 */
/*  OK Topcoder SRM 372 div 1 1000, 2 ms, n<=50 worst case, 10.08.2011 */
int hungarian() {
  static int z[2*MAXV],delta[MAXV],q[MAXV],u[MAXV],v[MAXV],p[MAXV];
  static char o[MAXV],x[MAXV];
  int i,j,left,d;
  char aug;
  memset(mate,-1,(n+n)*sizeof(int));
  memset(z,0,(n+n)*sizeof(int));
  for(i=0;i<n;i++) for(j=0;j<n;j++) if(z[i]<g[i][j]) z[i]=g[i][j];
  left=n;
  while(left) {
    memset(o,0,n); memset(p,-1,n*sizeof(int)); memset(delta,126,n*sizeof(int));
    aug=0;
    for(i=0;i<n;i++) q[i]=mate[i]<0;
    do {
      for(i=0;!q[i];i++);
      j=q[i]=0; o[i]=1;
      while(!aug && j<n) {
        if(mate[i]!=j && delta[j]>z[i]+z[j+n]-g[i][j]) {
          delta[j]=z[i]+z[j+n]-g[i][j];
          p[j]=i;
          if(!delta[j]) {
            if(mate[j+n]<0) {
              augment(mate,p,j);
              aug=1;
              left--;
            } else q[mate[j+n]]=1;
          }
        }
        j++;
      }
      for(i=0;i<n;i++) if(q[i]) break;
      if(!aug && i==n) {
        d=INF;
        for(i=0;i<n;i++) u[i]=o[i];
        for(i=0;i<n;i++) v[i]=delta[i]==0;
        for(i=0;i<n;i++) if(d>delta[i] && !v[i]) d=delta[i];
        for(i=0;i<n;i++) if(u[i]) z[i]-=d;
        for(i=0;i<n;i++) if(v[i]) z[i+n]+=d; else delta[i]-=d;
        for(i=0;i<n;i++) x[i]=(!v[i] && !delta[i]);
        for(i=0;i<n;i++) if(x[i] && mate[i+n]<0) break;
        if(i==n) {
          for(i=0;i<n;i++) if(x[i]) q[mate[i+n]]=1;
        } else {
          augment(mate,p,i);
          aug=1;
          left--;
        }
      }
    } while(!aug);
  }
  for(i=j=0;i<n;i++) if(mate[i]>-1) j+=g[i][mate[i]-n];
  return j;
}

/*  minimum weight match in bipartite graph */
/*  run on normal g[][], it gets destroyed */
/*  OK UVa 10888 0.012 seconds, n<=15, 10.08.2011 */
/*  OK Topcoder SRM 372 div 1 1000, 2 ms worst case, n<=50, 10.08.2011 */
int minhungarian() {
	int max=0,i,j;
	for(i=0;i<n;i++) for(j=0;j<n;j++) if(max<g[i][j]) max=g[i][j];
	for(i=0;i<n;i++) for(j=0;j<n;j++) if(g[i][j]>-1) g[i][j]=max-g[i][j];
	return max*n-hungarian();
}
