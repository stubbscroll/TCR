/* max-clique!
   to find maximal independent sets, run this algorithm on the
   complement graph */

/* bron-kerbosch algorithm with pivoting:
   find all maximal cliques using backtracking
   source: http://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
   NB, version with vertex ordering not implemented yet
   special version with bitmasks:
   - graph is in array of bitmasks g
   - graph size n<=64
*/
/* OK topcoder SRM 571 div 1-500 n<=50 0.059 s 2013-08-31 */
/* OK topcoder SRM 589 div 1-450 n<=50 0.011 s 2013-08-31 */

typedef unsigned long long ull;

ull g[64]; /* g[i] bit j is set if there is an edge between nodes i and j */
int n;

void bronkerboschbitpivot(ull R,ull P,ull X) {
	ull Q;
	int v,u;
	if(!(P|X)) {
		/* visit maximal clique in R */
	} else {
		for(u=0;u<n;u++) if((P|X)&(1ULL<<u)) break;
		for(Q=P&~g[u],v=0;v<n;v++) if(Q&(1ULL<<v)) {
			bronkerboschbitpivot(R|(1ULL<<v),P&g[v],X&g[v]);
			P-=1ULL<<v;
			X|=1ULL<<v;
		}
	}
}
