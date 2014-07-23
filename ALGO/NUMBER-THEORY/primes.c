/* TODO! this file is a mess. split stuff into separate files, such as
   factor, numdiv, sumdiv, phi etc and indicate where common code should
   be inserted (or just duplicate it) */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* MAXP      65536  1000000  10000000  30000000
   n-primes   6542    78498    664579   1857859
*/

#define MAXP 10000000
typedef long long ll;
typedef unsigned long long ull;
typedef unsigned int uint;

char sieve[MAXP];
int prime[664579];
int primes;

/* prime number generation! */

/* OK project euler 214 2011-08-03 */
/* OK spoj (classical) 4942 2011-08-04 */
/* OK spoj (tutorial) 1392 2011-08-04 */
/* OK UVa 11073 2011-08-04 */
/* OK UVa 11466 n<=10000000 2013-06-12 */
/* OK UVa 12101 (NWERC 2006 problem G) 0.004 ms n<10000 2012-08-26 */
/* OK UVa 12493 n<=46500 2012-08-23 */
/* OK UVa-live 3639 (NWERC 2006 problem G) 0.004 ms n<10000 2012-08-26 */
void createsieve() {
	int i,j,q;
	memset(sieve,1,sizeof(sieve));
	q=sqrt(MAXP);
	for(sieve[0]=sieve[1]=0,i=2;i<=q;i++) if(sieve[i])
		for(j=i*i;j<MAXP;j+=i) sieve[j]=0;
}

/* OK project euler 214 2011-08-03 */
/* OK spoj (classical) 4942 2011-08-04 */
/* OK spoj (tutorial) 2011-08-04 */
/* OK UVa 11073 2011-08-04 */
/* OK UVa 11466 n<=10000000 2013-06-12 */
/* OK UVa 12493 n<=46500 2012-08-23 */
void genprimes() {
	int i;
	for(primes=i=0;i<MAXP;i++) if(sieve[i]) prime[primes++]=i;
}

/* factorization sieve: sieve[i] is 0 if i is prime, otherwise sieve[i] is
   smallest divisor. the datatype for sieve[] must be able to hold
   sqrt(MAXP). same asymptotic runtime as normal sieve, but higher constant
   because of the branch in the inner loop, and wider datatype */
/* OK Project Euler 386 27.08.2011 */
/* OK Topcoder Inv 2002 finals (run 1) 500 04.06.2012 */
/* OK UVa 10680 0.096 seconds n<=1000000 08.06.2012 */
int sieve2[MAXP];
void fsieve() {
	int q=sqrt(MAXP),i,j;
	memset(sieve2,0,sizeof(sieve2));
	sieve2[0]=sieve2[1]=0;
	for(i=2;i<=q;i++) if(sieve2[i]<1) for(j=i*i;j<MAXP;j+=i) if(!sieve2[j]) sieve2[j]=i;
}
/* use factorization sieve to factor a number (requires n<MAXP), runs in time
   linear to the number of prime factors (including duplicates). */
/* OK Project Euler 386 2011-08-27 */
/* OK Topcoder Inv 2002 finals (run 1) 500 2012-06-04 */
/* OK UVa 10680 0.096 seconds n<=1000000 2012-06-08 */
void ffactor(int n,int *f,int *fc,int *fn) {
	*fn=0;
	while(sieve2[n]) {
		if(*fn && f[*fn-1]==sieve2[n]) fc[*fn-1]++,n/=sieve2[n];
		else f[*fn]=sieve2[n],fc[(*fn)++]=1,n/=sieve2[n];
	}
	if(n>1) {
		if(*fn && f[*fn-1]==n) fc[*fn-1]++;
		else f[*fn]=n,fc[(*fn)++]=1;
	}
}

/* bit-packed sieve storing only odd numbers using 32-bit ints */
uint psieve[(MAXP+63)/64];
#define CLEARBIT(ix) psieve[(ix)>>6]&=~(1<<(((ix)>>1)&31))
#define CHECKBIT(ix) (psieve[(ix)>>6]&(1<<(((ix)>>1)&31)))
/* WARNING, not tested in competitions! */
void pcreatesieve() {
	ll i,j,q;
	memset(psieve,255,(MAXP+63)/16);
	q=sqrt(MAXP);
	for(i=3;i<=q;i+=2) if(CHECKBIT(i)) for(j=i*i;j<MAXP;j+=i+i) CLEARBIT(j);
}

void pgenprimes() {
	int i;
	prime[0]=2;
	for(primes=1,i=3;i<MAXP;i+=2) if(CHECKBIT(i)) prime[primes++]=i;
}

/* gcd! */
ull gcd(ull a,ull b) {
	ull t;
	while(b) t=a,a=b,b=t%b;
	return a;
}

/* slightly faster gcd */
/* OK project euler 005 2011-08-04 */
/* OK spoj (classical) 4942 2011-08-04 */
ull binarygcd(ull a,ull b) {
	int e=0;
	ull d;
	if(!a||!b) return a|b;
	while(!((a|b)&1)) a>>=1,b>>=1,e++;
	while(!(a&1)) a>>=1;
	do {
		while(!(b&1)) b>>=1;
		if(a<b) b-=a;
		else d=a-b,a=b,b=d;
		b>>=1;
	} while(b);
	return a<<e;
}

/* OK project euler 005 2011-08-04 */
ull lcm(ull a,ull b) {
	return a/binarygcd(a,b)*b;
}

/* find x,y such that ax + by = gcd(a,b) */
/* UVa 10090 2012-06-08 */
ll extendedeuclid(ll a,ll b,ll *lastx,ll *lasty) {
	ll x=0,y=1,t,q;
	*lastx=1; *lasty=0;
	while(b) {
		q=a/b;
		t=a,a=b,b=t%b;
		t=x,x=*lastx-q*x,*lastx=t;
		t=y,y=*lasty-q*y,*lasty=t;
	}
	return a;
}

/* nice short recursive extended euclid */
/* warning, doesn't calculate gcd(a,b), only finds x,y */
/* WARNING, not tested */
void exeuclid(ll a,ll b,ll *x,ll *y) {
	if(!b) *x=1,*y=0;
	else exeuclid(b,a%b,y,x),*y-=*x*(a/b);
}

/* find the multiplicative inverse of a modulo mod */
/* requires gcd(a,mod)==1, returns 0 otherwise */
/* OK UVa 12365 2011-11-12 */
/* OK IPSC 2005 G "gears in action" 2014-05-17 */
/* OK Project Euler 381 2012-04.27 */
/* OK Project Euler 274 2013-03-08 */
/* OK Codeforces 138 div 1 problem C 2012-09-16 */
ll inverse(ll a,ll mod) {
	ll b=mod,x=0,y=1,t,q,lastx=1,lasty=0;
	while(b) {
		q=a/b;
		t=a,a=b,b=t%b;
		t=x,x=lastx-q*x,lastx=t;
		t=y,y=lasty-q*y,lasty=t;
	}
	return a==1?(lastx%mod+mod)%mod:0;
}

/* faster/better inverse from "prime numbers - a computational perspective,
   algorithm 9.4.4 page 465. beware of overflow! */
/* OK Project Euler 274 2013-03-08 */
ll inverse2(ll x,ll p) {
	ll a=1,q;
	if(x>=p) x%=p;
	while(x!=1) {
		q=-(p/x);
		x=p+q*x;
		a=(q*a)%p; /* warning ullmulmod if necessary */
		if(a<0) a+=p;
	}
	return a;
}

/* solve a set of modular equations using chinese remainder theorem */
/* n: number of equations of the form
   x = a[i] mod b[i] */
/* OK IPSC 2005 G "gears in action" (java bigint version) 2014-05-17 */
/* OK project euler 365 2012-02-20 */
ull chinese(int n,int *a,int *b) {
	int i;
	ull N,x=0;
	for(N=1,i=0;i<n;i++) N*=b[i];
	for(i=0;i<n;i++) x=(x+a[i]*N/b[i]*inverse(N/b[i]%b[i],b[i]))%N;
	return x;
}

/* primality testing! */

/* trial division, can check primality for numbers up to prime[primes-1]^2 */
/* can actually remove the i<primes check if n is at most
   prime[primes-2]^2 */
/* change ll to int and remove (ll) if n<2^31 always holds */
/* NB, not tested */
int isprime(ll n) {
	int i;
	if(n<MAXP) return sieve[n];
	for(i=0;i<primes && (ll)prime[i]*prime[i]<=n;i++) if(n%prime[i]==0) return 0;
	return 1;
}

/* table used by routine that finds log base 2 a.k.a. number of highest
   set bit */
/* NB, needed for powmod, and hence also for witness, millerrabin,
   ullmillerrabin,factormain */
/* NB, initlogtable must be called! */
/* TODO get rid of this drivel */
char logtable256[256];
/* init routine which must be called */
void initlogtable() {
	int i;
	logtable256[0]=-1;
	logtable256[1]=0;
	for(i=2;i<256;i++) logtable256[i]=1+logtable256[i>>1];
}
/* find highest set bit of unsigned 32-bit number n and put result in r.
   t,tt are temp variables in 32, t in 64 */
#define LOG2_32(n,r,t,tt) if((tt=n>>16)) r=(t=tt>>8)?24+logtable256[t]:\
	16+logtable256[tt]; else r=(t=n>>8)?8+logtable256[t]:logtable256[n];
#define LOG2_64(n,r,t) if((t=n>>56)) r=56+logtable256[t];\
	else if((t=n>>48)) r=48+logtable256[t];\
	else if((t=n>>40)) r=40+logtable256[t];\
	else if((t=n>>32)) r=32+logtable256[t];\
	else if((t=n>>24)) r=24+logtable256[t];\
	else if((t=n>>16)) r=16+logtable256[t];\
	else if((t=n>>8)) r=8+logtable256[t];\
	else r=logtable256[n];

/* need powmod for witness */
/* OK UVa 10956 2011-08-31 */
/* NOT OK YET spoj (classical) 4942 2011-08-04 */
uint powmod(uint n,uint k,uint mod) {
	int i,j;
	uint v=n,ans=1,t,tt;
	if(!k) return 1;
	/* TODO rewrite this drivel:
	goto start;
	while(k>0) {
		v=(ull)v*v%mod;
	start:
		if(k&1) ans=(ull)ans*v%mod;
		k>>=1;
	}
	*/
	/* find topmost set bit */
	LOG2_32(k,i,t,tt)
/* alternatively:
	i=31;
	while(i>=0 && !(k&(1<<i))) --i;
*/
	for(j=0;j<=i;j++) {
		if(k&(1U<<j)) ans=(ull)ans*v%mod;
		v=(ull)v*v%mod;
	}
	return ans;
}

/* subroutine for miller-rabin, only works for odd n */
/* OK UVa 10956 2011-08-04 */
/* OK spoj (classical) 4942 2011-08-04 */
int witness(uint n,uint a) {
	int s=1,r;
	uint d=(n-1)>>1,x;
	while(!(d&1)) s++,d>>=1;
	x=powmod(a,d,n);
	if(x==1 || x==n-1) return 1;
	for(r=1;r<s;r++) {
		x=(ull)x*x%n;
		if(x==1) return 0;
		if(x==n-1) return 1;
	}
	return 0;
}

/* deterministic miller-rabin, return 1 if n is prime */
/* OK UVa 10956 2011-08-04 */
/* OK spoj (classical) 4942 2011-08-04 */
int millerrabin(uint n) {
	if(n<4) return n>1;
	if(!(n&1)) return 0;
	if(n<1373653) return witness(n,2) && witness(n,3);
	if(n<9080191) return witness(n,31) && witness(n,73);
	return witness(n,2) && witness(n,7) && witness(n,61);
}

/* needed for ullwitness */
/* OK spoj (classical) 4942 2011-08-04 */
/* (however, not tested for values very close to maxull) */
ull ullmulmod(ull a,ull b,ull mod) {
	ull r=0,t;
	if(a>=mod) a%=mod;
	if(b>=mod) b%=mod;
	if(a>b) t=a,a=b,b=t;
	while(b) {
		if(b&1) {
			r+=a;
			if(r>=mod || r<a) r-=mod;
		}
		a=(a<<1)%mod;
		b>>=1;
	}
	return r;
}

/* 128-bits version! faster than ullmulmod */
/* warning, it apparently only works in 64-bits gcc */
/* http://apps.topcoder.com/forums/?module=Thread&threadID=765421&start=0 */
/* TODO find out which online judges this works on */
typedef __uint128_t ulll;
ull ullmulmod2(ull a,ull b,ull mod) { return (ulll)a*b%mod; }

/* 64-bits assembly version */
/* slightly faster than ullmulmod2 (not by much) */
ull ullmulmod3(ull x,ull y,ull mod) {
	ull r;
	/* only use the two next lines if x>=mod or y>=mod can happen! */
	if(x>=mod) x%=mod;
	if(y>=mod) y%=mod;
	__asm__("mulq %2\n\t"
	    "divq %3":"=&d"(r),"+%a"(x):"rm"(y),"rm"(mod):"cc");
	return r;
}

/* OK spoj (classical) 4942 2011-08-04 */
ull ullpowmod(ull n,ull k,ull mod) {
	int i,j;
	ull v=n,ans=1;
	uint t;
	if(!k) return 1;
	/*  find topmost set bit */
	LOG2_64(k,i,t)
	for(j=0;j<=i;j++) {
		if(k&(1ULL<<j)) ans=ullmulmod(ans,v,mod);
		v=ullmulmod(v,v,mod);
	}
	return ans;
}

/* subroutine for miller-rabin, only works for odd n */
/* OK spoj (classical) 4942 2011-08-04 */
int ullwitness(ull n,ull a) {
	int s=1,r;
	ull d=(n-1)>>1,x;
	while(!(d&1)) s++,d>>=1;
	x=ullpowmod(a,d,n);
	if(x==1 || x==n-1) return 1;
	for(r=1;r<s;r++) {
		x=ullmulmod(x,x,n);
		if(x==1) return 0;
		if(x==n-1) return 1;
	}
	return 0;
}

/* deterministic miller-rabin for 64-bit numbers */
/* OK spoj (classical) 4942 2011-08-04 */
/* OK Project Euler 387 2012-07-04 */
int ullmillerrabin(ull n) {
	if(n<4294967295LU) return millerrabin(n);
	if(!(n&1)) return 0;
	if(n<4759123141LL)
		return ullwitness(n,2) && ullwitness(n,7) && ullwitness(n,61);
	if(n<2152302898747LL)
		return ullwitness(n,2) && ullwitness(n,3) && ullwitness(n,5)
			&& ullwitness(n,7) && ullwitness(n,11);
	if(n<3474749660383LL)
		return ullwitness(n,2) && ullwitness(n,3) && ullwitness(n,5)
			&& ullwitness(n,7) && ullwitness(n,11) && ullwitness(n,13);
	if(n<341550071728321LL)
		return ullwitness(n,2) && ullwitness(n,3) && ullwitness(n,5)
			&& ullwitness(n,7) && ullwitness(n,11) && ullwitness(n,13)
			&& ullwitness(n,17);
	return ullwitness(n,2) && ullwitness(n,325) && ullwitness(n,9375)
		&& ullwitness(n,28178) && ullwitness(n,450775) && ullwitness(n,9780504)
		&& ullwitness(n,1795265022);
}

/* TODO solovay-strassen */
/* TODO lucas-lehmer */
/* TODO use algorithm from PRIMES in P */

/* factorization */

/* uses prime[] */
/* write out the factors to f, the count of each f to fc, the
   number of factors to fn */
/* OK project euler 003 2011-06-09 */
/* OK spoj (tutorial) 1392 2011-08-04 */
/* OK UVa 11073 2011-08-04 */
/* OK UVa 11466 n<=10000000 2013-06-12 */
/* OK UVa 12465 n<=32000 2012-06-06 */
void factor(ull n,ull *f,int *fc,int *fn) {
	int i;
	for(*fn=i=0;i<primes && (ull)prime[i]*prime[i]<=n;i++) if(n%prime[i]<1) {
		f[*fn]=prime[i];
		fc[*fn]=1;
		n/=prime[i];
		while(n%prime[i]<1) n/=prime[i],fc[*fn]++;
		(*fn)++;
	}
	if(n>1) f[*fn]=n,fc[(*fn)++]=1;
}

/* warning, only call on composite numbers */
/* OK spoj (classical) 4942 2011-08-04 */
/* OK UVa 11476 2011-08-04 */
ull pollardrho(ull n,ull c) {
	/* number of iterations: 1.177*sqrt(sqrt(n)) is good
	   source http://users.telenet.be/janneli/jan/factorization/pollard_rho.html
	   TODO proof? */
	int max=1.179*sqrt(sqrt(n)),range=1,j,k;
	ull x1=2,x2=4+c,product=1,temp,g;
	for(j=0;j<max;) {
		for(k=0;k<range;k++) {
			x2=ullmulmod(x2,x2,n);
			x2+=c;
			if(x2>=n) x2-=n;
			temp=x1>x2?x1-x2:x2-x1;
			product=ullmulmod(product,temp,n);
			j++;
			if(!(j&7)) {
				g=binarygcd(product,n);
				if(g==n) return 0;
				else if(g>1) return g;
				product=1;
			}
		}
		x1=x2;
		range<<=1;
		for(k=0;k<range;k++) {
			x2=ullmulmod(x2,x2,n);
			x2+=c;
			if(x2>=n) x2-=n;
		}
	}
	return 0;
}

/* requires primes up to MAXPRIME */
/* OK spoj (classical) 4942 0.09 seconds 2011-08-04 */
/* OK UVa 11476 9.600 seconds 2011-08-04 */
#define MAXPRIME 1000
void factormain(ull n,ull *f,int *fc,int *fn,int small) {
	int i,fc1[2][64],fn1[2],j,k,done;
	ull v,f1[2][64];
	*fn=0;
	if(small) {
		/* factor out small factors */
		for(i=0;prime[i]<MAXPRIME && (ull)prime[i]*prime[i]<=n;i++) if(n%prime[i]==0) {
			n/=prime[i];
			f[*fn]=prime[i];
			fc[*fn]=1;
			while(n%prime[i]==0) fc[*fn]++,n/=prime[i];
			(*fn)++;
		}
		if(n<2) return;
		if(n<(ull)MAXPRIME*MAXPRIME) { f[*fn]=n; fc[(*fn)++]=1; return; }
	}
	if(ullmillerrabin(n)) { fc[*fn]=1; f[(*fn)++]=n; return; }
	for(i=1;!(v=pollardrho(n,i));i++);
	factormain(v,f1[0],fc1[0],&fn1[0],0);
	factormain(n/v,f1[1],fc1[1],&fn1[1],0);
	for(i=0;i<2;i++) for(j=0;j<fn1[i];j++) {
		for(k=0;k<*fn;k++) if(f[k]==f1[i][j]) {
			fc[k]+=fc1[i][j];
			goto done;
		}
		f[*fn]=f1[i][j];
		fc[(*fn)++]=fc1[i][j];
	done:;
	}
	do {
		done=1;
		for(i=0;i<*fn-1;i++) if(f[i]>f[i+1]) {
			v=f[i],f[i]=f[i+1],f[i+1]=v;
			k=fc[i],fc[i]=fc[i+1],fc[i+1]=k;
			done=0;
		}
	} while(!done);
}

/* euler's totient (phi) function! */
/* calculate phi(n), needs one prime[] > sqrt(n) */
/* OK, compared against phigen 2011-08-03 */
/* OK UVa 12493 n<=46500 2012-08-23 */
ll phi(ll n) {
	int i=0,q=sqrt(n);
	ll res=1;
	if(n<MAXP && sieve[n]) return n-1;
	while(prime[i]<=q) {
		if(n%prime[i]==0) {
			res*=prime[i]-1;
			n/=prime[i];
			while(n%prime[i]==0) {
				res*=prime[i];
				n/=prime[i];
			}
		}
		i++;
	}
	if(n>1) res*=n-1;
	return res;
}

/* generate all phi up to MAXPHI, needs all prime[]<MAXPHI.
   this routine is faster than calculating each phi(i) individually */
/* OK project euler 214 2011-08-03 */
#define MAXPHI 40000001
int phivalues[MAXPHI];

void phigenbtr(int n,int t,int p) {
	int n2,t2;
	phivalues[n]=t;
	while(p<primes && (ll)n*prime[p]<MAXPHI) {
		n2=n*prime[p];
		t2=t*(prime[p]-1);
		phigenbtr(n2,t2,p+1);
		while((ll)n2*prime[p]<MAXPHI) {
			n2*=prime[p];
			t2*=prime[p];
			phigenbtr(n2,t2,p+1);
		}
		p++;
	}
}

void phigen() {
	phigenbtr(1,1,0);
}

/* inverse phi! */

/* OK UVa 11073 2011-08-04 0.076 seconds */
/* OK Project Euler 248 2011-08-04 */
void inversephibtr(ull left,ull build,int p,ull *val,int *n) {
	ull temp,b;
	if(left==1) { val[(*n)++]=build; return; }
	if(left&1) return;
	if(left+1>=prime[p] && isprime(left+1)) val[(*n)++]=build*(left+1);
	for(;p<primes && (ull)(prime[p]-1)*prime[p]<=left;p++) if(left%(prime[p]-1)==0) {
		inversephibtr(temp=left/(prime[p]-1),b=build*prime[p],p+1,val,n);
		while(temp%prime[p]==0) {
			temp/=prime[p];
			b*=prime[p];
			inversephibtr(temp,b,p+1,val,n);
		}
	}
}

void inversephi(ull left,ull *val,int *n) {
	*n=0;
	if(left==1) { val[0]=1; *n=val[1]=2; }
	if(left<2) return;
	inversephibtr(left,1,0,val,n);
}

/* number of divisors, requires prime[]<=sqrt(n) */
/* OK-ish, n<100001 matches OEIS A000005 2011-08-03 */
/* OK UVa 12465 n<=1000000000 2012-06-06 */
ll numdiv(ll n) {
	ll ans=1,f[64];
	int i,fc[64],fn;
	factor(n,f,fc,&fn);
	for(i=0;i<fn;i++) ans*=fc[i]+1;
	return ans;
}

/* number of divisors, worst-case runtime O(sqrt(n)) which happens when
   n is prime or a power, but is standalone */
/* OK-ish, matches numdiv() for n<10000000 2014-07-23 */
ll numdiv2(ll n) {
	ll r=1;
	int i,j;
	for(i=2;(ll)i*i<=n;i++) if(n%i==0) {
		for(j=2,n/=i;n%i==0;n/=i,j++);
		r*=j;
	}
	if(n>1) r*=2;
	return r;
}

/* sum of divisors, requires prime[]<=sqrt(n) */
/* OK-ish, n<100001 matches OEIS A000203 2011-08-03 */
/* OK spoj (tutorial) 1392 2011-08-04 */
ll sumdiv(ll n) {
	ll ans=1,f[64],q;
	int i,j,fc[64],fn;
	factor(n,f,fc,&fn);
	for(i=0;i<fn;i++) {
		for(j=fc[i]+1,q=1;j;j--) q*=f[i];
		ans*=(q-1)/(f[i]-1);
	}
	return ans;
}

int compull(const void *A,const void *B) {
	ull *a=(ull *)A,*b=(ull *)B;
	if(*a<*b) return -1;
	if(*a>*b) return 1;
	return 0;
}

int main() {
	ull f[64],sum,n,x,y;
	int fc[64],fn,i;
	static ull a[1000000];
	createsieve(MAXP);
	genprimes();
	factor(600851475143LL,f,fc,&fn);
	printf("Project Euler 003: %lld\n",f[fn-1]);     /*  6857 */
	printf("Project Euler 007: %d\n",prime[10000]);  /*  104743 */
	for(sum=i=0;prime[i]<2000000;i++) sum+=prime[i];
	printf("Project Euler 010: %lld\n",sum);         /*  142913828922 */
	x=1;
	for(i=1;i<21;i++) x=lcm(x,i);
	printf("Project Euler 005: %lld\n",x);
	inversephi(1LL*2*3*4*5*6*7*8*9*10*11*12*13,a,&fn);
	qsort(a,fn,sizeof(ull),compull);
	printf("Project Euler 248: %lld\n",a[149999]);
	return 0;
}
