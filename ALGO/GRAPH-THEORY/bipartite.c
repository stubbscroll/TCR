#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* TODO
   see if i can implement a variant where there are capacities between
   nodes. this is useful on problems like uva 10092, idi open 2012 A and
   maybe even on USACO NOV07 gold problem 3 "tanning".
*/

/* maximum matching in bipartite graph!
   n nodes on left side, m nodes on right side
   g[i][j]=1 if edge, 0<=i<n, 0<=j<m
   returns number of edges
   runtime: O(nm)
*/
#define MAXV 150
char g[MAXV][MAXV];         /* [i][j]: edge between i on the left and j on the right */
int n,m;                    /* left, right */
int mate[MAXV];             /* mate for right node (or -1 if unmatched) */
int parent[2*MAXV];

/* OK Topcoder SRM 200 div 1 1000, 4 ms worst case, n=m=176, 2011-08-07 */
/* OK Topcoder SRM 236 div 1 1000, 12 ms worst case, n,m<=100, 2011-08-07 (+bfs, binary search) */
/* OK Topcoder SRM 303 div 1 500, 5 ms worst case, n,m<=338, 2011-08-07 */
/* OK Topcoder SRM 477 div 1 500, 8 ms worst case, n+m<=200, 2011-08-07 */
/* OK Topcoder SRM 549 div 1 250, 4 ms worst case, n+m<=100, 2012-07-09 */
/* OK Topcoder TCO 2003 semifinals 4 1050, 7 ms worst case, n,m<=300, 2011-08-07 */
/* OK UVa 259 0.020 seconds, n,m<=10, 2011-08-13 (small constraints, backtracking is faster */
/* OK UVa 663 0.004 seconds, n=m<=26, 2011-08-07 */
/* OK UVa 670 0.016 seconds, n,m<=100, 2011-08-07 */
/* OK UVa 753 0.108 seconds, n,m<=100, 2011-08-13 (+floyd-warshall) */
/* OK UVa 10080 0.016 seconds, n,m<=100, 2011-08-11 */
/* OK UVa 10092 0.376 seconds, n<=1000,m<=200, 2011-08-11 */
/* OK UVa 10122 0.980 seconds, n,m<=100, 2011-08-11 (+binary search, preprocessing) */
/* OK UVa 10349 0.008 seconds, n,m<=200, 2011-08-11 */
/* OK UVa 10804 0.076 seconds, n,m<=50, 2011-08-07 (+binary search) */
/* OK UVa 11418 0.008 seconds, n=m<=26, 2011-08-07 */
/* OK UVa 11419 0.396 seconds, n,m<=1000, 2011-08-07 */
/* OK UVa 12083 0.044 seconds, n+m<=500, 2012-08-29 */
/* OK UVa-live 3415 (NWERC 2005 problem H) 0.040 seconds, n+m<=500, 2012-08-29 */
/* OK UVa-live 4288 (NWERC 2008 problem C) 0.032 seconds, n+m<=500, 2012-08-08 */
/* OK hnu 11025 (BAPC 2006 problem F) 15 ms, n<5120, m<100, 2011-08-12 */
/* OK IDI Open 2012 problem A (Boss Rush), n<=100, m<=300, 2012-05-21 */
/* OK IPSC 2003 problem A (arranging flowers), n,m<=220, 2014-05-31 */
/* OK IPSC 2004-practice problem T, n,m<=10945, 2014-05-31 */
int bipartite() {
	int i,j,r=0,cur,qs,qe=0;
	static int q[2*MAXV];
	memset(mate,-1,m*sizeof(int));
	memset(parent,-1,(n+m)*sizeof(int));
	for(i=0;i<n;i++) {
		qs=0;
		q[qe++]=i;
		while(qs<qe) {
			cur=q[qs++];
			if(cur<n) {
				for(j=n;j<m+n;j++) if(g[cur][j-n] && parent[j]<0) {
					parent[j]=cur;
					q[qe++]=j;
					if(mate[j-n]<0) {
						while(j>-1) {
							mate[j-n]=parent[j];
							j=parent[parent[j]];
						}
						r++;
						goto ok;
					}
				}
			} else {
				q[qe++]=mate[cur-n];
				parent[mate[cur-n]]=cur;
			}
		}
		/* insert termination here if we really want to match the left side in
		   increasing order */
	ok:
		while(qe) parent[q[--qe]]=-1;
	}
	return r;
}

/* find minimum vertex cover in bipartite graph
   uses könig's theorem: let T be all unmatched left-side vertices, and all
   vertices that can be reached from them in the residual network. then
   the vertex cover is (L\T) union (R intersect T)
   runtime: O(n+m)
   run bipartite() first */
/*  OK UVa 11419 0.396 seconds, n,m<=1000, 07.08.2011 */
char cover[2*MAXV];
void minimumvertexcover() {
	int i,j,qs=0,qe=0,cur;
	static int q[2*MAXV];
	memset(cover,1,n); memset(cover+n,0,m);
	/* for i<n, cover[i]: 0=visited, 1=unvisited. inverted for i>=n */
	for(i=0;i<m;i++) if(mate[i]>-1) cover[mate[i]]=0;
	for(i=0;i<n;i++) if(cover[i]) q[qe++]=i,cover[i]=0; else cover[i]=1;
	while(qs<qe) {
		cur=q[qs++];
		if(cur<n) {
			for(j=n;j<n+m;j++) if(g[cur][j-n] && !cover[j]) {
				cover[j]=1;
				q[qe++]=j;
			}
		} else if(mate[cur-n]>-1) {
			cover[mate[cur-n]]=0;
			q[qe++]=mate[cur-n];
		}
	}
}

int main() {
	int r;
	n=m=2;
	g[0][0]=1;
	g[0][1]=1;
	g[1][0]=1;
	r=bipartite();
	printf("%d\n",r);
	return 0;
}
