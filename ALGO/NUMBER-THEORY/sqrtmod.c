/* modular square root! */

/* calculate the jacobi symbol, returns 0, 1 or -1 */
/* 1: a is quadratic residue mod m, -1: a is not, 0: a mod m=0 */
/* based on algorithm 2.3.5 in "prime numbers" (crandall, pomerance) */
/* WARNING, m must be an odd positive number */
/* OK project euler 216 (only tested for m prime 1 or 7 mod 8) 19.10.2012 */
int jacobi(ll a,ll m) {
	int t=1;
	ll z;
	a%=m;
	while(a) {
		while(!(a&1)) {
			a>>=1;
			if((m&7)==3 || (m&7)==5) t=-t;
		}
		z=a,a=m,m=z;
		if((a&3)==3 && (m&3)==3) t=-t;
		a%=m;
	}
	if(m==1) return t;
	return 0;
}

/* calculate legendre symbol, returns 0, 1 or -1 */
/* 1: a is quadratic residue mod p, -1: a is not, 0: a mod p=0 */
/* WARNING, p must be an odd prime */
/* WARNING, not tested in competition code */
int legendre(ll a,ll p) {
	return ullpowmod(a%p,(p-1)>>1,p)==1?1:-1;
}

ull rand64() {
	return (rand()&32767LL)+((rand()&32767LL)<<15)
		+((rand()&32767LL)<<30)+((rand()&32767LL)<<45)+((rand()&15LL)<<60);
}

/* find square root of a modulo p (p prime) */
/* runtime O(ln^4 p) */
/* mod 3,5,7: algorithm 2.3.8 from "prime numbers" (crandall, pomerance) */
/* mod 1: from http://www.mast.queensu.ca/~math418/m418oh/m418oh11.pdf */
/* OK project euler 216 (only tested for p=1 or 7 mod 8) 2012-10-19 */
/* OK project euler 437 (tested all cases mod 8) 2013-09-21 */
ull sqrtmod(ull a,ull p) {
	int p8,alpha,i;
	ull x,c,s,n,b,J,r2a,r;
	if(p==2) return a&1;
	a%=p;
	if(legendre(a,p)!=1) return 0; /* no square root */
	p8=p&7;
	if(p8==3 || p8==5 || p8==7) {
		if((p8&3)==3) return ullpowmod(a,(p+1)/4,p);
		x=ullpowmod(a,(p+3)/8,p);
		c=ullmulmod(x,x,p);
		return c==a?x:ullmulmod(x,ullpowmod(2,(p-1)/4,p),p);
	}
	alpha=0;
	s=p-1;
	while(!(s&1)) s>>=1,alpha++;
	r=ullpowmod(a,(s+1)/2,p);
	r2a=ullmulmod(r,ullpowmod(a,(s+1)/2-1,p),p);
	do n=rand64()%(p-2)+2; while(legendre(n,p)!=-1);
	b=ullpowmod(n,s,p);
	J=0;
	for(i=0;i<alpha-1;i++) {
		c=ullpowmod(b,2*J,p);
		c=ullmulmod(r2a,c,p);
		c=ullpowmod(c,1ULL<<(alpha-i-2),p);
		if(c==p-1) J+=1ULL<<i;
	}
	return ullmulmod(r,ullpowmod(b,J,p),p);
}

int rand31() {
	return (rand()&32767)+((rand()&32767)<<15)+((rand()&1)<<30);
}

/* sqrtmod for a,p<2^31 */
/* remember to change legendre to use non-ull powmod */
int sqrtmod(int a,int p) {
	int p8,alpha,i,x,c,s,n,b,J,r2a,r;
	if(p==2) return a&1;
	a%=p;
	if(legendre(a,p)!=1) return 0; /* no square root */
	p8=p&7;
	if(p8==3 || p8==5 || p8==7) {
		if((p8&3)==3) return powmod(a,(p+1)/4,p);
		x=powmod(a,(p+3)/8,p);
		c=(ll)x*x%p;
		return c==a?x:(ll)x*powmod(2,(p-1)/4,p)%p;
	}
	alpha=0;
	s=p-1;
	while(!(s&1)) s>>=1,alpha++;
	r=powmod(a,(s+1)/2,p);
	r2a=(ll)r*powmod(a,(s+1)/2-1,p)%p;
	do n=rand31()%(p-2)+2; while(legendre(n,p)!=-1);
	b=powmod(n,s,p);
	J=0;
	for(i=0;i<alpha-1;i++) {
		c=powmod(b,2*J,p);
		c=(ll)r2a*c%p;
		c=powmod(c,1ULL<<(alpha-i-2),p);
		if(c==p-1) J+=1ULL<<i;
	}
	return (ll)r*powmod(b,J,p)%p;
}
