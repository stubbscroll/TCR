#include <stdio.h>
#include <string.h>

/* binary index (fenwick) tree code, 1-indexed tree[] */
/* store frequencies f[1]..f[n]
   update frequency f[i] in O(log n)
   read cumulative frequency sum i=1 to k f[i] in O(log n)
   find kth smallest element in O(log n)
   read single frequency by doing read(i)-read(i-1)
   set frequency f[i]=a by doing update(a-read(i),i))
   delete element: update(-1,ix) */
/* WARNING: don't use index 0. this case can happen if you want
   to count values in a range. instead, shift the range. */
/* OK IDI Open 2010 F Beads ??.??.2011 */
/* OK Topcoder SRM 234 div 1 850 worst case 133 ms, 22.11.2012 */
/* OK Topcoder SRM 310 div 1 500 worst case 69 ms, 03.10.2011 */
/* OK Topcoder Member SRM 482 div 1 250 worst case 716 ms, 22.11.2012 */
/* OK UVa 501 n<=30000, 0.164 seconds, 14.01.2012 */
/* OK UVa 10015 n<=3501, 0.356 seconds, 08.06.2012 */
/* OK UVa 10057 n<=65537, 0.096 seconds, 08.06.2012 */
/* OK UVa 10909 n<=1100000, 0.168 seconds, 04.10.2011 */
/* OK UVa 11525 n<=50000, 0.152 seconds, 04.10.2011 */
/* OK UVa 12086 n<=200000, 0.288 seconds, 15.06.2012 */
/* OK UVa 12365 n<=100000, 0.488 seconds, 12.11.2011 */
/* OK NCPC 2011 problem B Mega-inversions 01.10.2011 */
/* OK SPOJ 9889 (NWERC 2011 problem C movie collection) n<=200000 0.95 seconds 15.01.2012 */
int tree[100001],n;   /*  warning, array must be of size n+1 */
int leftmostbit;  /*  cache the leftmost set bit in n here! */
/*  initialize structure with n elements */
void deltree() {
  /*  three next lines are only needed for findkth() */
  int i;
  for(leftmostbit=n,i=30;!(leftmostbit&(1<<i));i--);
  leftmostbit&=1<<i;
  /*  only thing that's needed for deltree */
  memset(tree,0,sizeof(int)*(n+1));
}
/*  add a to frequency with index ix */
void update(int a,int ix) {
  while(ix<=n) tree[ix]+=a,ix+=(ix&-ix);
}
/*  read cumulative frequency from 1..ix */
int read(int ix) {
  int r=0;
  while(ix) r+=tree[ix],ix-=(ix&-ix);
  return r;
}
/*  read one frequency, faster (by a constant) than read(ix)-read(ix-1) */
/*  WARNING, not tested */
int readone(int ix) {
  int r=tree[ix],z;
  if(ix) {
    z=ix-(ix&-ix);
    ix--;
    while(ix!=z) r-=tree[ix],ix-=ix&-ix;
  }
  return r;
}
/*  find kth smallest element. that is, find lowest index
    such that cumulative frequency >= k. here we assume that at least
    k elements have been put in the tree */
/*  WARNING, please do not call with f=0 */
/*  alternatively: binary search using read(), O(log^2 n) */
int findkth(int f) {
  int ix=0,mask=leftmostbit;
  while(mask) {
    if(ix+mask<=n && f>tree[ix+mask]) f-=tree[ix+=mask];
    mask>>=1;
  }
  return ix+1;
}
