#include <stdio.h>
typedef long long ll;

/*  digit sum! not defined for n<0 */

/*  calculate recursive digit sum, call until sum is <10 */
/*  NB, untested */
int recdigitsum(ll n) {
  return !n?0:(n-1)%9+1;
}

/*  calculate digit sum */
/*  NB, untested */
int digitsum(ll n) {
  int r=0;
  while(n) r+=n%10,n/=10;
  return r;
}

int main() {
  printf("num %d: %d\n",123765765,digitsum(123765765));
  printf("recnum %d: %d\n",123765765,recdigitsum(123765765));
  return 0;
}
