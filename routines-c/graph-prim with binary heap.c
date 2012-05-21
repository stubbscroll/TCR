// NB! this is a version with an implicit graph: points in plane
// no decrease-key in loop

#define INF 1000000000
#define M 131072
#define E 131072
#define V 20480

int n,pi[V];
double x[V],y[V],d[V];
char taken[V];

/* heap routines */

#define PARENT(i) (i>>1)
#define LEFT(i) (i<<1)
#define RIGHT(i) ((i<<1)+1)
#define SWAP(a,b) { t=a; a=b; b=t; }

/* 1-indexed heap, bleh */
int qn;   /* heap size */
int A[M]; /* heap */
int b[M]; /* reverse-pointer into heap */

void minheapify(int i)
{
   int l=LEFT(i),r=RIGHT(i),largest,t;

   if(l<=qn && d[A[l]]<d[A[i]]) largest=l;
   else largest=i;
   if(r<=qn && d[A[r]]<d[A[largest]]) largest=r;
   if(largest!=i) {
      SWAP(b[A[i]],b[A[largest]]);
      SWAP(A[i],A[largest]);
      minheapify(largest);
   }
}

int extractmin()
{
   int min=A[1];
   A[1]=A[qn];
   b[A[1]]=1;
   qn--;
   minheapify(1);
   return min;
}

void decreasekey(int i)
{
   int t;

   while(i>1 && d[A[PARENT(i)]]>d[A[i]]) {
      SWAP(b[A[i]],b[A[PARENT(i)]]);
      SWAP(A[i],A[PARENT(i)]);
      i=PARENT(i);
   }
}

void prim(int start)
{
   int i,j,k,u,v,t;
   double dx,dy,dist,best;

   for(i=0;i<n;i++) d[i]=INF;
   for(i=0;i<n;i++) { A[i+1]=i; b[i]=i+1; }
   qn=n;
   d[start]=0;
   SWAP(b[A[start+1]],b[A[1]]);
   SWAP(A[start+1],A[1]);
   memset(taken,0,n);
   for(i=1;i<n;i++) {
      dx=x[0]-x[i];
      dy=y[0]-y[i];
      d[i]=dx*dx+dy*dy;
      decreasekey(i);
   }
   for(i=0;i<n;i++) {
      u=extractmin();
      best=1e100;
      for(v=0;v<n;v++) if(taken[v]) {
         dx=x[u]-x[v];
         dy=y[u]-y[v];
         if(best>dx*dx+dy*dy) {
            best=dx*dx+dy*dy;
            j=v;
         }
      }
      taken[u]=1;
      if(i) { // add edge from u to j
      }
   }
}
