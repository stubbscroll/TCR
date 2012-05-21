/* algorithms for adjacency lists */
/* on undirected graphs: make a double copy of all edges (a,b) => (b,a) */

#define MAXN 1000
#define MAXE 100000

typedef struct {
   int a,b,c;
} node;
int n,ne;        /* number of nodes, number of edges */
node g[MAXE];    /* edges */
int gp[MAXN+1];  /* pointers to g */

/* sort nodes on a,b,c */
int compn(node *a,node *b)
{
   if(a->a!=b->a) return(a->a>b->a)-(a->a<b->a);
   if(a->b!=b->b) return(a->b>b->b)-(a->b<b->b);
   return(a->c>b->c)-(a->c<b->c); /* lowest cost first */
   return(a->c<b->c)-(a->c>b->c); /* highest cost first */
}

/* sort graph and generate start and end points for each node */
int process()
{
   int i;

   qsort(g,ne,sizeof(node),compn);
   for(i=1;i<n;i++) gp[i]=-1;
   gp[0]=0;
   for(i=0;i<ne;i++) if(gp[g[i].a]==-1) gp[g[i].a]=i;
   gp[n]=ne;
   for(i=0;i<n;i++) if(gp[i]==-1) gp[i]=gp[i-1];
}
