/*  binary indexed (fenwick) tree in 2D!
    domain: 1..h, 1..w (including h and w)
    DO NOT use index 0.
    store frequencies in 2D.
    update2d(x,i,j) adds x to frequency of (i,j) in O(log^2 n) time
    read2d(i,j) reads cumulative frequency of subrectangle (1..i,1,,j)
      in O(log^2 n) time */
/*  fenwick tree generalizes to d dimensions, but the cost for update,
    read becomes O(log^d n) */
/*  it is slightly silly to use this data structure for static problems
    - for that, use area subrectangle DP instead. */
/*  OK zstu 3423 (NCPC 2010 I) 7337 ms, 04.10.2011 */
#define MAXH 150
#define MAXW 128
int tree2d[MAXH][MAXW];
int h,w;                /*  height, width */
void deltree2d() {
  int i;
  for(i=0;i<=h;i++) memset(tree2d[i],0,(w+1)*sizeof(int));
}
/*  add a to frequency(ih,iw) */
void update2d(int a,int ih,int iw) {
  int tw;
  while(ih<=h) {
    tw=iw;
    while(tw<=w) tree2d[ih][tw]+=a,tw+=(tw&-tw);
    ih+=ih&-ih;
  }
}
/*  read cumulative frequency (1..ih, 1..iw) */
int read2d(int ih,int iw) {
  int r=0,tw;
  while(ih) {
    tw=iw;
    while(tw) r+=tree2d[ih][tw],tw-=(tw&-tw);
    ih-=ih&-ih;
  }
  return r;
}
