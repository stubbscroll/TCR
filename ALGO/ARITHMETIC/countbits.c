typedef long long ll;
/*	count the number of bits set in bit position i for all number from
		0 to n, inclusive. should work with b<=61, or b<=62 for ull */
/*	OK Topcoder SRM 543 div 1 250 21.05.2012, NB only bit 0 in answer tested */
ll countbits(ll n,int b) {
	ll x=(n+1)>>(b+1)<<b;
	ll y=(n+1)&((1LL<<(b+1))-1);
	if(y>=(1LL<<b)) x+=y-(1LL<<b);
	return x;
}
