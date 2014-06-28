/* linear programming with naive implementation of simplex algorithm!

   usage: enter equation in standard form into eq and eqrhs. standard form
   implies that the equation is an inequality with <= and that all variables
   are non-negative. also, enter maximization expression into lpmax. finally,
   call simplex().

   if you need to enter equality expressions like
     a_0 x_0 + a_1 x_1 + ... == b,
   split them into two equations:
     a_0 x_0 + a_1 x_1 + ... <= b
    -a_0 x_0 - a_1 x_1 + ... <=-b
   if the objective is to minimize, negate the coefficients.
   if you have variables without nonnegativity constraints, split them:
     a_0 x_0 <= b
   becomes
     a_0 x_1 - a_0 x_2 <= b,
   where x_1 - x_2 = x_0.

   warning, number of variables in system is increased by MAXEQ, please ensure
   that data structures are large enough to hold them.

   warning, this implementation assumes that the basic solution (all slack
   variables set to 0) is feasible.
*/

#define MAXEQ 5500
#define MAXVAR 8000
#define EPS 1e-9
#define INF 1e50

double lpmax[MAXVAR];     /* objective function to maximize */
double lpconst;           /* constant in expression */

double eq[MAXEQ][MAXVAR]; /* coefficients */
double eqrhs[MAXEQ];      /* right-hand side */
int eqn;                  /* number of equations */
int vn;                   /* number of variables */

void initsimplex() {
	int i,j,k,need=eqn;
	/* add need extra variables, append identity matrix to the right */
	if(vn+need>MAXVAR) printf("increase MAXVAR to %d and recompile\n",vn+need),exit(1);
	for(i=0;i<eqn;i++) for(j=0;j<need;j++) eq[i][j+vn]=i==j;
	for(i=0;i<eqn;i++) lpmax[i+vn]=0;
	for(i=0;i<vn;i++) lpmax[i]=-lpmax[i];
	vn+=need;
	/* set k to index of minimum eqrhs */
	for(k=0,i=1;i<vn;i++) if(eqrhs[k]>eqrhs[i]) k=i;
	if(eqrhs[k]>=0) return; /* basic solution is feasible, ok */
	puts("error, haven't yet implemented case where basic solution is bad");
	exit(1);
}

void pivot(int lx,int e) {
	double m;
	int i,j;
	/* normalize row lx */
	m=eq[lx][e];
	for(i=0;i<vn;i++) eq[lx][i]/=m;
	eqrhs[lx]/=m;
	/* from each row i, subtract multiple of eq[i][e] of row lx */
	for(i=0;i<eqn;i++) if(i!=lx && eq[i][e]) {
		m=eq[i][e];
		for(j=0;j<vn;j++) eq[i][j]-=m*eq[lx][j];
		eqrhs[i]-=m*eqrhs[lx];
	}
	/* subtract multiple from objective function */
	m=lpmax[e];
	if(m) for(j=0;j<vn;j++) lpmax[j]-=m*eq[lx][j];
	lpconst-=m*eqrhs[lx];
}

/* OK IPSC 2014 I "inventing test data" eqn<=2018, vn<=6588 2014-06-29 */
double simplex() {
	double best,r;
	int i,e,ix;
	initsimplex();
	while(1) {
		/* select variable whose coefficient in the objective function
		   is negative, and pick the maximal among these */
		ix=-1; best=INF;
		for(e=0;e<vn;e++) if(best>lpmax[e]) best=lpmax[e],ix=e;
		if(best>=-EPS) break;
		e=ix;
		/* find variable that maximizes ratio rhs/e */
		ix=-1; best=INF;
		for(i=0;i<eqn;i++) if(eq[i][e]>EPS) {
			r=eqrhs[i]/eq[i][e];
			if(best>r) best=r,ix=i;
		}
 		if(best>=INF) return INF; /* unbounded */
		pivot(ix,e);
	}
	return lpconst;
}
