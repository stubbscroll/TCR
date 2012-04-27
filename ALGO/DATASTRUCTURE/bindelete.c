/*  binary deletion tree!
    supports:
    - build balanced binary tree from array in O(n)
    - get n-th element in O(log n)
    - get and delete n-th element in O(log n)
    - convert tree to array in O(n)
    - find index, given value of item in O(n)

    since no insertions occur, no rebalancing need
    to be done.
*/

#include <stdio.h>
#include <string.h>

#define MAX 1000000
int pleft[MAX],pright[MAX]; /*  pointer to left and right children */
int cleft[MAX],cright[MAX]; /*  size of left and right subtrees */
int val[MAX];               /*  value of element */
char has[MAX];              /*  1 if element exists, 0 if deleted */
int root;                   /*  the root node of the tree */
int n;                      /*  size of tree including deleted elements */

/*  subroutine for build() */
int buildbtr(int lo,int hi) {
  int mid=(lo+hi)>>1;
  if(mid>lo) pleft[mid]=buildbtr(lo,mid-1), cleft[mid]=mid-lo;
  if(mid<hi) pright[mid]=buildbtr(mid+1,hi), cright[mid]=hi-mid;
  return mid;
}

/*  build tree from array, it will be sorted on INDEX, not content. */
/*  OK UVa 10909 0.220 s, n=335000, 21.09.2011 */
/*  OK UVa 11525 0.168 s, n=50000, 04.10.2011 */
void build(int *a,int num) {
  memset(pleft,-1,sizeof(int)*num);
  memset(pright,-1,sizeof(int)*num);
  memset(cleft,0,sizeof(int)*num);
  memset(cright,0,sizeof(int)*num);
  memcpy(val,a,sizeof(int)*num);
  memset(has,1,num);
  root=buildbtr(0,num-1);
  n=num;
}

int extractnthbtr(int node,int ix) {
  if(ix==cleft[node] && has[node]) {
    has[node]=0;
    return val[node];
  } else if(ix<cleft[node]) {
    cleft[node]--;
    return extractnthbtr(pleft[node],ix);
  } else {
    cright[node]--;
    return extractnthbtr(pright[node],ix-cleft[node]-has[node]);
  }
}

/*  get n-th element and mark it as removed (0-indexed) */
/*  OK UVa 10909 220 ms, n=335000, 21.09.2011 */
/*  OK UVa 11525 0.168 s, n=50000, 04.10.2011 */
int extractnth(int ix) {
  return extractnthbtr(root,ix);
}

int getnthbtr(int node,int ix) {
  if(ix==cleft[node] && has[node]) return val[node];
  else if(ix<cleft[node]) return getnthbtr(pleft[node],ix);
  else return getnthbtr(pright[node],ix-cleft[node]-has[node]);
}

/*  get n-th element (0-indexed) without removing it */
/*  OK UVa 10909 220 ms, n=335000, 21.09.2011 */
int getnth(int ix) {
  return getnthbtr(root,ix);
}

/*  convert tree to array */
/*  TODO test this routine */
void toarray(int *a,int *num) {
  int i;
  for(i=*num=0;i<n;i++) if(has[i]) a[(*num)++]=val[i];
}

/*  return number of elements in tree, O(1) */
/*  OK  UVa 10909 220 ms, n=335000, 21.09.2011 */
int getsize() {
  return cleft[root]+cright[root]+has[root];
}

int getindexbtr(int node,int v) {
  if(v<val[node] && pleft[node]<0) return -1;
  if(v>val[node] && pright[node]<0) return -1;
  if(v==val[node]) return has[node]?cleft[node]:-1;
  else if(v<val[node]) return getindexbtr(pleft[node],v);
  else return cleft[node]+has[node]+getindexbtr(pright[node],v);
}

/*  given that the original array was sorted, and given a value:
    return its current index or -1 if element doesn't exist */
/*  TODO test this routine */
int getindex(int v) {
  return getindexbtr(root,v);
}

