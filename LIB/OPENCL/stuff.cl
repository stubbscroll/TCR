__kernel void sieve(__global uchar *a,int prim,int max) {
  const int id=get_global_id(0)+get_global_id(1)*1024+get_global_id(2)*1048576;
  int ix=(id+2)*prim;
	if(ix<max) a[ix]=0;
}
