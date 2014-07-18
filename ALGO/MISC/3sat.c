/* 3-SAT solver! naive implementation of schöning's probabilistic algorithm */

/* usage:
   only 3-clauses are supported now, but the algorithm can be easily changed
   to k-sat or var-sat.
   each clause i (x_a OR x_b OR x_c) is entered into the array clause,
   where variables are 1-indexed and negated variables are entered with
   negative index.
   the solution is returned in var[] (0-indexed).
   example (0-indexed variables)
     (x_0 OR ~x_6 OR x_10)
   becomes
     {1,-7,11}
   in the 1-indexed encoding in clause[][].
*/

#define MAXCL 500
#define MAXVAR 100

int clause[MAXCL][3]; /* 1-indexed, negative means negation */
int var[MAXVAR];      /* 0-indexed */
int n;                /* number of clauses */
int m;                /* number of variables */

/* OK IPSC 2013 L "labyrinth", n<=100, m<=400, 2014-07-18 */
int sat3() {
	int i,j,N=3*n,tries;
	for(tries=1;;tries++) {
		/* init: random assignment */
		for(i=0;i<n;i++) var[i]=(rand()>>14)&1;
		while(N--) {
			/* scan until we find an unsolved clause */
			for(i=0;i<m;i++) {
				for(j=0;j<3;j++) {
					if(clause[i][j]>0 && var[clause[i][j]-1]) goto ok;
					if(clause[i][j]<0 && !var[-clause[i][j]-1]) goto ok;
				}
				goto notok;
			ok:;
			}
			/* couldn't find any unsolved clauses, so we succeeded! */
			return 1;
		notok:
			/* flip random variable in clause */
			j=rand()%3;
			if(clause[i][j]>0) var[clause[i][j]-1]^=1;
			else var[-clause[i][j]-1]^=1;
		}
		/* no solution, restart. if we are trying to solve unsolvable instances, we
		   might want to cap the number of tries */
	}
	return 0;
}
