typedef unsigned int uint;
typedef unsigned long long ull;

/* evaluate 1+a+a^2+...+a^n mod m in O(log(n)) time */
/* warning, need n<2^32-1 */
/* OK IPSC 2005 problem B2 (bottom coder) 2014-05-15 */
uint powsum(uint a,uint n,uint m) {
	uint z=1,b=a,c=1,r=0,i;
	n++;
	for(i=0;i<32 && (1U<<i)<=n;i++) {
		if((1U<<i)&n) {
			r=(r+(ull)c*z)%m;
			c=((ull)c*b)%m;
		}
		z=(z+(ull)z*b)%m;
		b=((ull)b*b)%m;
	}
	return r;
}
