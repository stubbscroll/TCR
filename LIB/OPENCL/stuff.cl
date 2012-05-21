__kernel void sieve(__global uchar *a,int prim,int max) {
  const int id=get_global_id(0);
  int ix=id*prim;
  if(id>0 && (ix>>3)<max) a[ix>>3]&=~(1<<(ix&7));
}
