/* gray code (more specifically: reflected binary code)
   http://en.wikipedia.org/wiki/Gray_code
*/

/* OK IPSC 2003-practice T "strange numbers" 2014-05-30 */

typedef unsigned long long ull;

ull binarytogray(ull x) {
	return x^(x>>1);
}

ull graytobinary(ull x) {
	ull mask;
	for(mask=x>>1;mask;mask>>=1) x^=mask;
	return x;
}
