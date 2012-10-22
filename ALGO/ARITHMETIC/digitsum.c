#include <stdio.h>
typedef long long ll;

/* digit sum! these routines are not defined for n<0 */

/* calculate recursive digit sum, call until sum is <10 */
/* NB, untested */
int recdigitsum(ll n) {
  return !n?0:(n-1)%9+1;
}

/* calculate digit sum */
/* OK UVa 12517 n<=10^9 20.10.2012 */
int digitsum(ll n) {
  int r=0;
  while(n) r+=n%10,n/=10;
  return r;
}

/* calculate sum of digit sums for all numbers from 1 to a (inclusive) */
/* NB, uses digitsum() above */
/* NB, need to call init first */
/* OK UVa 12517 n<=10^9 20.10.2012 */

ll sum[21]; /* sum[i]: sum of all numbers up to and including i digits */

void init() {
	ll ten=1;
	int i;
	sum[0]=0;
	for(i=1;i<20;i++,ten*=10) sum[i]=sum[i-1]*10+45*ten;
}

ll res(ll a) {
	ll r=0,z;
	ll ten=10,occ=1;
	int at=0;
	while(a) {
		while(a%ten) {
			r+=dsum(a/occ-1)*occ+sum[at];
			a-=occ;
		}
		occ=ten; ten*=10; at++;
	}
	return r;
}

int main() {
  printf("num %d: %d\n",123765765,digitsum(123765765));
  printf("recnum %d: %d\n",123765765,recdigitsum(123765765));
  return 0;
}
