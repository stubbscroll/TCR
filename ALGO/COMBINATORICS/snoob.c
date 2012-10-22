
typedef unsigned long long ull;

/*	return next k-selection (bitmask) */
/*	OK UVa 649 25.05.2012 */
ull snoob(ull x) {
	ull s=x&-x, r=x+s, o=x^r;
	o=(o>>2)/s;
	return r|o;
}
