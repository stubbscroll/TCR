
typedef long long ull;

/*	return next k-selection (bitmask) */

ull snoob(ull x) {
	ull s=x&-x, r=x+s, o=x^r;
	o=(o>>2)/s;
	return r|o;
}
