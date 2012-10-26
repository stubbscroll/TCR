#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc,char**argv) {
  double res=0;
  int threads,i;
  if(argc<2) { printf("usage: openmp num-threads\n"); exit(0); }
  threads=strtol(argv[1],0,10);
  #pragma omp parallel for num_threads(threads) reduction(+:res)
  for(i=0;i<500000000;i++) res+=exp(-i*0.000001);
  printf("%d threads, res %f\n",threads,res);
  return 0;
}
