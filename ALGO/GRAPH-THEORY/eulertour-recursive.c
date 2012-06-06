#include <stdio.h>
#include <string.h>

/* find euler tour, recursive version */
/* OK UVa 10054 n<=50 ne<=2000 0.288 seconds 05.06.2012 */

#define MAXV 51
#define MAXE 2048
int n,ne;               /* number of nodes, number of edges */
int from[MAXE],to[MAXE];/* edge i: from[i] to[i] */
int gs[MAXV+1];         /* gs[i]: start of edges from i, gs[i+1]: end */
int inv[MAXE];          /* index to reverse edge */

int tour[MAXE];					/* actual tour */
int len;								/* length of tour */
char taken[MAXE];

void radixsort() {
  static int newfrom[MAXE],newto[MAXE];
  int i,j;
  /* sort on to */
  memset(gs,0,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) gs[to[i]]++;
  for(i=1;i<n;i++) gs[i]+=gs[i-1];
  for(i=ne-1;i>=0;i--) {
    j=--gs[to[i]];
    newto[j]=to[i];
    newfrom[j]=from[i];
  }
  /* (stable) sort on from */
  memset(gs,0,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) gs[newfrom[i]]++;
  for(i=1;i<n;i++) gs[i]+=gs[i-1];
  gs[n]=ne;
  for(i=ne-1;i>=0;i--) {
    j=--gs[newfrom[i]];
    to[j]=newto[i];
    from[j]=newfrom[i];
  }
}

/* for each edge a->b, find index to b->a */
/* call after radixsort! */
void inverseedges() {
  static int starts[MAXV+1];
  int i;
  memcpy(starts,gs,sizeof(int)*(n+1));
  for(i=0;i<ne;i++) inv[i]=starts[to[i]]++;
	for(i=0;i<ne-1;i++) if(from[i]==to[i] && from[i+1]==from[i] && to[i+1]==to[i] && inv[i]==i && inv[i+1]==i+1)
		inv[i]=i+1,inv[i+1]=i;
}

/* check if all nodes have even degrees (necessary for euler tour) */
int evendegree() {
	int i;
	for(i=0;i<n;i++) if((gs[i+1]-gs[i])&1) return 0;
	return 1;
}

void findeulertourbtr(int u) {
	int i;
	for(i=gs[u];i<gs[u+1];i++) if(!taken[i]) {
		taken[i]=taken[inv[i]]=1;
		findeulertourbtr(to[i]);
	}
	tour[len++]=u;
}

/* how to find euler path: connect the two odd-degree nodes with dummy
   edge and call this algorithm */
int findeulertour() {
	int i;
	radixsort();
	inverseedges();
	if(!evendegree()) return 0;
	len=0;
	memset(taken,0,ne);
	for(i=0;i<n;i++) if(gs[i+1]>gs[i]) {
		findeulertourbtr(i);
		return len+len-2==ne;
	}
	return 0;
}
