#include <stdio.h>
#include <string.h>

/* specialized max flow routine, only works when all capacities from source
   to its direct neighbours are 1, and is only extremely efficient if the
   only node with a high outdegree is the source. this algorithm is actually
   expected to take less memory than regular bipartite matching on sparse
   graphs.

   recommended domains:
   - tripartite matching where each center node has rather few possibilities
     on both sides
   - "arbitrary cap bipartite matching" where the left side can be chosen
     once

   usage:
   - derermine if edges should be sort reversed, and if we want to force
     all nodes adjacent to the sink to be matched. change the code
     accordingly
   - set MAXV,MAXE to guaranteed maximum values for the problem (MAXE
     must include back edges!)
   - set n to the desided number of nodes, set ne=0
   - define edges either by putting them into from[],to[] manually
     (remember back edges) or by using addedges()
   - radixsort()
   - inverseedges()
   - clear f[] to zero
   - do a separate pass where caps are set in f[]. only set forward caps,
     back edges have cap 0
   - finally, maxflow()

   if you want to reverse sort to[] in order to let the augmenting path
   search try forward edges before back edges, reverse the order of certain
   for loops in radixsort() and inverseedges() as indicated.
*/

/* warning, there shouldn't be more than one edge between any two given
   nodes */

/* OK UVa 563 0.012 seconds, n<=5002, 30.11.2012 */
/* OK SPOJ 9890 (NWERC 2011 problem D), n<=333335, 2.32 seconds 30.11.2011 */
/* OK SPOJ 9890 (NWERC 2011 problem D), n<=333335, 1.75 seconds with reversed edges! 30.12.2011 */
/* OK Topcoder SRM 422 div-1-1000, 45 ms, n<=1802, ne<=405000 21.05.2012 */
#define MAXE 2000000
#define MAXV 333500
#define INF 1000000000

/* TODO consider interleaving all these in a struct or something
   for cache-friendliness */
int from[MAXE],to[MAXE];  /* graph */
int gs[MAXV+1];           /* pointer to first node */
int f[MAXE];              /* flow/capacity/residual graph */
int inv[MAXE];            /* index to reverse edge */
int n,ne;                 /* number of nodes, edges */

/* NB! force starting move heuristic assumes that all capacities==1 */
int maxflow(int source,int sink) {
	int i,j,flow=0,qe=0,k,l,last=gs[source];
	static char t[MAXV];
	static int parent[MAXV],q[MAXV];
	memset(t,0,n);
	memset(parent,-1,n*sizeof(int));
	for(last=gs[source];last<gs[source+1];last++) {
		t[to[last]]=1;
		q[qe++]=to[last];
		for(k=0;k<qe;k++) if(t[i=q[k]]) for(l=gs[i];l<gs[i+1];l++) {
			j=to[l];
			if(!t[j] && f[l]>0) {
				t[j]=1; parent[j]=l;
				q[qe++]=j;
				if(j==sink) goto out;
			}
		}
		/* if all nodes adjacent to the sink must be matched: use break
		   if not: use continue */
		break;
/*		continue; */
	out:
		l=parent[sink];
		while(l>-1) {
			f[l]--;
			f[inv[l]]++;
			l=parent[from[l]];
		}
		flow++;
		while(qe) {
			j=q[--qe];
			t[j]=0; parent[j]=-1;
		}
	}
	return flow;
}

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
	/* use the next line instead of the previous for reversing to[] */
/*	for(i=0;i<ne;i++) {*/
		j=--gs[newfrom[i]];
		to[j]=newto[i];
		from[j]=newfrom[i];
	}
}

/*  for each edge a->b, find index to b->a */
void inverseedges() {
	static int starts[MAXV+1];
	int i;
	memcpy(starts,gs,sizeof(int)*(n+1));
	for(i=0;i<ne;i++) inv[i]=starts[to[i]]++;
	/* use the next line instead of the previous for reversing to[] */
/*	for(i=ne-1;i>=0;i--) inv[i]=starts[to[i]]++;*/
}
