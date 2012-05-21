#define INF 1000000000
#define M 131072
#define E 131072
#define V 20480

/* graph: adjacency lists */
typedef struct { int a,b,w; } sg;
sg g[E];
int gs[V],ge[V],n,d[V];

int compg(sg *a,sg *b)
{
   if(a->a!=b->a) return a->a-b->a;
   return a->b-b->b;
}

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

int dijkstra(int start,int end)
{
   int i,j,k,u,v,t;

   for(i=0;i<n;i++) d[i]=INF;
   for(i=0;i<n;i++) { A[i+1]=i; b[i]=i+1; }
   qn=n;
   d[start]=0;
   SWAP(b[A[start+1]],b[A[1]]);
   SWAP(A[start+1],A[1]);
   while(1) {
      u=extractmin();
      if(u==end) return d[u];
      for(i=gs[u];i<ge[u];i++) {
         v=g[i].b;
         if(d[v]>d[u]+g[i].w)
            d[v]=d[u]+g[i].w;
            decreasekey(b[v]);
      }
   }
   return INF;
}

