/* red-black tree (with order statistics)
   NB, nodes start at index 1, index 0 is a sentinel node */

/* OK SPOJ 9889 (NWERC 2011 problem C "movie collection") n<=100000 1.61 seconds 14.01.2012 */
/* UVa 501 n<=30000 0.076 seconds 14.01.2012 */
/* UVa 10909 n<=350000 0.232 seconds 14.01.2012 */
/* UVa 11525 n<=50000 0.272 seconds 14.01.2012 */

#define MAX 131072

int value[MAX];
int p[MAX];
int left[MAX];
int right[MAX];
int count[MAX];
char col[MAX];
int root;
int n;
int rbq[MAX],rbqn;  /*  queue for reusing deleted nodes */

int rbindex[MAX];   /*  given value, find index in tree */

void rbinit() {
  n=1;
  rbqn=root=p[0]=left[0]=right[0]=col[0]=count[0]=0;
}

int rblev[32];

int rbbtr(int lo,int hi,int prev,int depth) {
  int mid=(hi+lo)>>1;
  count[mid]=hi-lo+1;
  col[mid]=rblev[depth];
  p[mid]=prev;
  left[mid]=mid>lo?rbbtr(lo,mid-1,mid,depth+1):0;
  right[mid]=mid<hi?rbbtr(mid+1,hi,mid,depth+1):0;
  return mid;
}

/*  build a perfectly balanced tree with N nodes. assume that rbinit() is
    called, and that value[] is filled in. */
/*  also initialize rbindex[] yourself whenever needed */
void rbbuildtree(int N) {
  int sum=0,lev;
  for(lev=0;sum<N;lev++) {
    sum+=1<<lev;
    rblev[lev]=lev&1;
    if(sum>N) rblev[lev]=1,rblev[lev-1]=0;
  }
  root=rbbtr(1,N,0,0);
  n=N+1;
}

void rbleftrotate(int x) {
  int y=right[x];
  right[x]=left[y];
  if(left[y]) p[left[y]]=x;
  p[y]=p[x];
  if(!p[x]) root=y;
  else if(x==left[p[x]]) left[p[x]]=y;
  else right[p[x]]=y;
  left[y]=x;
  p[x]=y;
  count[y]=count[x];
  count[x]=1+count[left[x]]+count[right[x]];
}

void rbrightrotate(int x) {
  int y=left[x];
  left[x]=right[y];
  if(right[y]) p[right[y]]=x;
  p[y]=p[x];
  if(!p[x]) root=y;
  else if(x==right[p[x]]) right[p[x]]=y;
  else left[p[x]]=y;
  right[y]=x;
  p[x]=y;
  count[y]=count[x];
  count[x]=1+count[left[x]]+count[right[x]];
}

int rbnewnode(int val) {
  int ix;
  if(rbqn) ix=rbq[--rbqn];
  else ix=n++;
  value[ix]=val;
  count[ix]=1;
  rbindex[val]=ix;    /*  added for by-value lookup */
  return ix;
}

void rbinsertfixup(int z) {
  int y;
  while(col[p[z]]) {
    if(p[z]==left[p[p[z]]]) {
      y=right[p[p[z]]];
      if(col[y]) col[p[z]]=col[y]=0,col[p[p[z]]]=1,z=p[p[z]];
      else {
        if(z==right[p[z]]) z=p[z],rbleftrotate(z);
        col[p[z]]=0;
        col[p[p[z]]]=1;
        rbrightrotate(p[p[z]]);
      }
    } else {
      y=left[p[p[z]]];
      if(col[y]) col[p[z]]=col[y]=0,col[p[p[z]]]=1,z=p[p[z]];
      else {
        if(z==left[p[z]]) z=p[z],rbrightrotate(z);
        col[p[z]]=0;
        col[p[p[z]]]=1;
        rbleftrotate(p[p[z]]);
      }
    }
  }
  col[root]=0;
}

void rbinsert(int val) {
  int z=rbnewnode(val),y=0,x=root;
  while(x) y=x,x=value[z]<value[x]?left[x]:right[x];
  p[z]=y;
  if(!y) root=z;
  else if(value[z]<value[y]) left[y]=z;
  else right[y]=z;
  left[z]=right[z]=0;
  col[z]=1;
  x=p[z];
  while(x) count[x]++,x=p[x];
  rbinsertfixup(z);
}

int rbtreeminimum(int x) {
  while(left[x]) x=left[x];
  return x;
}

void rbdeletefixup(int x) {
  int w;
  while(x!=root && !col[x]) {
    if(x==left[p[x]]) {
      w=right[p[x]];
      if(col[w]) {
        col[w]=0;
        col[p[x]]=1;
        rbleftrotate(p[x]);
        w=right[p[x]];
      }
      if(!col[left[w]] && !col[right[w]]) col[w]=1,x=p[x];
      else {
        if(!col[right[w]]) col[left[w]]=0,col[w]=1,rbrightrotate(w),w=right[p[x]];
        col[w]=col[p[x]];
        col[p[x]]=col[right[w]]=0;
        rbleftrotate(p[x]);
        x=root;
      }
    } else {
      w=left[p[x]];
      if(col[w]) {
        col[w]=0;
        col[p[x]]=1;
        rbrightrotate(p[x]);
        w=left[p[x]];
      }
      if(!col[right[w]] && !col[left[w]]) col[w]=1,x=p[x];
      else {
        if(!col[left[w]]) col[right[w]]=0,col[w]=1,rbleftrotate(w),w=left[p[x]];
        col[w]=col[p[x]];
        col[p[x]]=col[left[w]]=0;
        rbrightrotate(p[x]);
        x=root;
      }
    }
  }
  col[x]=0;
}

int rbtreesuccessor(int x) {
  int y;
  if(right[x]) return rbtreeminimum(right[x]);
  y=p[x];
  while(y && x==right[y]) x=y,y=p[y];
  return y;
}

/*  delete node with index z */
/*  the space taken by node z might be reoccupied by another node,
    so that another index actually gets freed */
void rbdelete(int z) {
  int y=!left[z]||!right[z]?z:rbtreesuccessor(z);
  int x=left[y]?left[y]:right[y];
  p[x]=p[y];
  if(!p[y]) root=x;
  else if(y==left[p[y]]) left[p[y]]=x;
  else right[p[y]]=x;
  if(y!=z) {
    value[z]=value[y];
    rbindex[value[z]]=z;  /*  added for by-value lookup */
  }
  rbq[rbqn++]=y;
  count[y]=0;
  z=p[y];
  while(z) count[z]--,z=p[z];
  if(!col[y]) rbdeletefixup(x);
}

/*  find index of k-th element in subtree rooted at at (k is 0-indexed) */
/*  WARNING, untested */
int rbfindrank(int k,int at) {
  while(k!=count[left[at]]) {
    if(k>count[left[at]]) k-=count[left[at]]+1,at=right[at];
    else at=left[at];
  }
  return at;
}

/*  given index to node, find its rank in tree */
int rbfindrankbyindex(int y) {
  int r=count[left[y]];
  while(y!=root) {
    if(y==right[p[y]]) r+=count[left[p[y]]]+1;
    y=p[y];
  }
  return r;
}

/*  insert element such that it gets rank k (0-indexed) */
/*  WARNING, only tested with k=0 */
void rbinsertrank(int val,int k) {
  int z=rbnewnode(val),y=0,x=root;
  while(x) {
    y=x;
    if(k<=count[left[x]]) x=left[x];
    else x=right[x],k-=1+count[left[x]];
  }
  p[z]=y;
  if(!y) root=z;
  else if(k) right[y]=z;
  else left[y]=z;
  left[z]=right[z]=0;
  col[z]=1;
  x=p[z];
  while(x) count[x]++,x=p[x];
  rbinsertfixup(z);
}
