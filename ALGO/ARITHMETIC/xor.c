typedef long long ll;

/*	xor of all numbers from 0 to n (inclusive)!
		given n such that n%4==0, it's easy to see that
		n^(n+1)^(n+2)^(n+3) is always 0:
		xxxxxxx00
		xxxxxxx01
		xxxxxxx10
		xxxxxxx11
		the bit pattern for the x's is equal for all, so the xor is 0. bit 0 and 1
		are set an even number of times.
*/
/*	return xor of all numbers from 0 to n, inclusive */
/*	OK Topcoder SRM 543 div 1 250, 21.05.2012 */
ll sumxor(ll n) {
	switch(n&3) {
	case 0: return n;
	case 1: return 1;
	case 2: return n+1;
	}
	return 0;
}
