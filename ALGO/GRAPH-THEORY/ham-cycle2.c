/* randomized hamilton cycle algorithm based on
   angluin/valiant, http://www.cs.berkeley.edu/~sinclair/cs271/n14.pdf

   this implementation assumes that the input graph has a hamiltonian
   cycle. in this case the algorithm finds a cycle in polynomial time
   (and very fast at that).

   usage:
   - create graph with addedge() (remember to add back edges)
   - call countingsort()
   - call hamcycle(start)

   caveat:
   - the algorithm can get stuck. in this case, let it run for a finite
     number of iterations before termination, then run it again from
     a different start node
   - if several runs fail (with 4(n-1)*ln(n-1) iterations) we can
     conclude (but not be fully sure) that the graph has no ham-cycle
   - the above estimate might be optimistic because of a dubious shortcut
     taken during implementation
*/

/* OK UVa 775 0.052 seconds, n<=256, 2013-06-07 */
/* OK hacker.org oneofus, n<=10609, 2013-06-07 (not timed exactly, but 998 out of 1000 instances were solved in under 1 s */

#define MAXV 11000
#define MAXE 2000000

int from[MAXE],to[MAXE];
int gs[MAXV+1];
int n,ne;

void countingsort() {
  static int newto[MAXE];
  int i,j;
  for(i=0;i<=n;i++) gs[i]=0;
  for(i=0;i<ne;i++) gs[from[i]]++;
  for(i=1;i<n;i++) gs[i]+=gs[i-1];
  gs[n]=ne;
  for(i=0;i<ne;i++) {
    j=--gs[from[i]];
    newto[j]=to[i];
  }
  for(i=0;i<ne;i++) to[i]=newto[i];
  for(i=0;i<n;i++) for(j=gs[i];j<gs[i+1];j++) from[j]=i;
}

void addedge(int a,int b) {
	from[ne]=a; to[ne++]=b;
}

char taken[MAXV];
int path[MAXV];

int randham(int s) {
	static int temp[MAXV];
	int ndp=s,pn=1,i,y,j,k;
	memset(taken,0,n);
	path[0]=s;
	taken[s]=1;
	while(1) {
		if(pn==n) for(i=gs[s];i<gs[s+1];i++) if(to[i]==path[n-1]) return 1;
		i=gs[path[pn-1]]+rand()%(gs[path[pn-1]+1]-gs[path[pn-1]]);
		y=to[i];
		if(!taken[y]) path[pn++]=y,taken[y]=1;
		else {
			for(i=0;i<pn;i++) if(path[i]==y) break;
			for(j=i;j<pn;j++) temp[j]=path[j];
			for(k=pn-1,j=i+1;j<pn;j++,k--) path[k]=temp[j];
		}
	}
	return 0;
}
