#include <stdio.h>

int condition(int mid) {
  return mid>=100;
}

/*  given a function such that f(v)=false for v<x and
    f(v)=true for v>=x for a value x, return x */
/*  OK UVa 10804 07.08.2011 */
/*  OK Topcoder SRM 236 div 1 1000 07.08.2011 */
int binarysearch() {
  int n=5000;
  int lo=0,hi=n,mid;  /*  n is exclusive */
  while(lo!=hi) {
    mid=lo+(hi-lo)/2;
    if(!condition(mid)) lo=mid+1;
    else hi=mid;
  }
  return lo;
}

int main() {
  printf("%d\n",binarysearch());
  return 0;
}
