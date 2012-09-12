#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* digits: 9.5 * MAXBIG */
#define MAXBIG 20
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
/* assume that n is always tight, a[n-1]==0 is never allowed for n>0 */
/* the number 0 is represented by n=0 */
typedef struct {
	uint a[MAXBIG];
	int n;
} big_t;

big_t TT; /* temp */

/* if you need to convert to/from string and bigint, just copy all routines
   from here down to bigtostr(), inclusive */

/* assign small constant to a bigint */
void bigzero(big_t *a) { a->n=0; }
void bigone(big_t *a) { a->n=a->a[0]=1; }
void bigint(uint a,big_t *b) { if(!a) bigzero(b); else b->n=1,b->a[0]=a; }
void bigull(ull a,big_t *b) {
	if(a<(1ULL<<32)) bigint((uint)a,b);
	else b->n=2,b->a[0]=a&0xFFFFFFFFu,b->a[1]=a>>32;
}

/* 1 if a is zero, 0 otherwise */
int bigcmpzero(big_t *a) { return !a->n; }

/* -1 if a<b, 0 if a==b, 1 if a>b */
int bigcmp(big_t *a,big_t *b) {
	int i;
	if(a->n<b->n) return -1;
	if(a->n>b->n) return 1;
	for(i=a->n-1;i>=0;i--) {
		if(a->a[i]<b->a[i]) return -1;
		if(a->a[i]>b->a[i]) return 1;
	}
	return 0;
}

int bigcmpint(big_t *a,uint b) {
	if(!a->n) return -(b>0);
	if(a->n>1 || a->a[0]>b) return 1;
	if(a->a[0]<b) return -1;
	return 0;
}

void bigcpy(big_t *a,big_t *b) {
	int i;
	b->n=a->n;
	for(i=0;i<b->n;i++) b->a[i]=a->a[i];
}

/* return a mod b (where b is int) */
/* OK UVa 288 0.044 seconds 12.09.2012 */
/* OK UVa 10157 0.048 seconds 11.09.2012 */
/* OK UVa 10069 0.060 seconds 11.09.2012 */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
uint bigmodint(big_t *a,uint b) {
	uint r=0;
	int i;
	for(i=a->n-1;i>=0;i--) r=(((ull)r<<32)+a->a[i])%b;
	return r;
}

/* calculate a/b where b is int */
/* OK UVa 288 0.044 seconds 12.09.2012 */
/* OK UVa 10069 0.060 seconds 11.09.2012 */
/* OK UVa 10157 0.048 seconds 11.09.2012 */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
void bigdivint(big_t *a,uint b,big_t *c) {
	ull carry=0,x;
	int i;
	for(i=a->n-1;i>=0;i--) {
		x=a->a[i];
		TT.a[i]=(x+carry)/b;
		carry=((x+carry)%b)<<32;
	}
	TT.n=a->n;
	while(TT.n && !TT.a[TT.n-1]) TT.n--;
	bigcpy(&TT,c);
}

/* used by bigstr! */
/* OK UVa 288 0.044 seconds 12.09.2012 */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
void bigaddint(big_t *a,uint b,big_t *c) {
	int i;
	c->n=a->n;
	for(i=0;i<a->n;i++) c->a[i]=a->a[i];
	c->a[i]=0;
	if(c->n) c->a[0]+=b; else c->a[0]=b,c->n=b>0;
	if(c->a[0]<b) {
		i=1;
		c->a[i]++;
		while(!c->a[i]) i++,c->a[i]++;
		if(i>=c->n) c->n++;
	}
}

/* helper function for mul */
void bigaddat(uint *a,uint b) {
	if(*a+b<b) {
		*a=*a+b; a++; (*a)++;
		while(!(*a)) a++,(*a)++;
	} else *a=*a+b;
}

/* used by bigstr! */
/* OK UVa 288 0.044 seconds 12.09.2012 */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
void bigmulint(big_t *a,uint b,big_t *c) {
	int i;
	ull r;
	for(TT.n=i=0;i<=a->n;i++) TT.a[i]=0;
	for(i=0;i<a->n;i++) {
		r=(ull)a->a[i]*b;
		bigaddat(&TT.a[i],r&0xFFFFFFFFu);
		if(r>=(1ULL<<32)) bigaddat(&TT.a[i+1],r>>32);
	}
	for(i=a->n;i>=0 && !TT.a[i];i--);
	c->n=i+1;
	for(i=0;i<c->n;i++) c->a[i]=TT.a[i];
}

/* convert string to bigint */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
void bigstr(char *s,big_t *a) {
	int i=0;
	bigzero(a);
	for(i=0;s[i];i++) bigmulint(a,10,a),bigaddint(a,s[i]-48,a);
}

/* convert bigint to string */
/* NB, uses bigcmpzero, bigcpy, bigmodint, bigdivint */
/* OK UVa 288 0.044 seconds 12.09.2012 */
/* OK UVa 10069 0.060 seconds 11.09.2012 */
/* OK UVa 10157 0.048 seconds 11.09.2012 */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
void bigtostr(big_t *a,char *s) {
	int n=0,i;
	char t;
	bigcpy(a,&TT);
	while(!bigcmpzero(&TT)) s[n++]=bigmodint(&TT,10)+48,bigdivint(&TT,10,&TT);
	for(i=0;i+i<n;i++) t=s[i],s[i]=s[n-i-1],s[n-i-1]=t;
	if(!n) s[n++]=48;
	s[n]=0;
}

/* shift a left by one bit */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
void bigshiftleft(big_t *a,big_t *b) {
	int i;
	if(!a->n) { b->n=0; return; }
	for(i=a->n-1;i;i--) TT.a[i]=(a->a[i]<<1)|(a->a[i-1]>>31);
	TT.a[0]=a->a[0]<<1;
	TT.n=a->n;
	if(a->n && (a->a[a->n-1]&0x80000000u)) TT.a[TT.n++]=1;
	bigcpy(&TT,b);
}

/* shift a right by one bit */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
void bigshiftright(big_t *a,big_t *b) {
	int i;
	if(!a->n) { b->n=0; return; }
	for(i=0;i<a->n-1;i++) TT.a[i]=(a->a[i]>>1)|((a->a[i+1]&1)<<31);
	TT.a[i]=a->a[i]>>1;
	TT.n=a->n-(TT.a[i]==0);
	bigcpy(&TT,b);
}

/* OK UVa 288 0.044 seconds 12.09.2012 */
/* OK UVa 10157 0.048 seconds 11.09.2012 */
/* OK UVa 10069 0.060 seconds 11.09.2012 */
void bigadd(big_t *a,big_t *b,big_t *c) {
	int i,n;
	uint u,v;
	if(a->n>=b->n) n=a->n;
	else n=b->n;
	for(i=0;i<=n;i++) TT.a[i]=0;
	for(i=0;i<n;i++) {
		if(i<a->n) u=a->a[i]; else u=0;
		if(i<b->n) v=b->a[i]; else v=0;
		if((ull)TT.a[i]+u+v>=(1ULL<<32)) TT.a[i+1]=1;
		TT.a[i]=TT.a[i]+u+v;
	}
	c->n=n+(TT.a[n]>0);
	for(i=0;i<c->n;i++) c->a[i]=TT.a[i];
}

/* calculate a-b. warning! requires a>=b */
/* OK UVa 288 0.044 seconds 12.09.2012 */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
void bigsub(big_t *a,big_t *b,big_t *c) {
	int i,n;
	uint u,v;
	ll B=0;
	if(bigcmp(a,b)<0) { bigzero(c); return; }
	if(a->n>=b->n) n=a->n;
	else n=b->n;
	for(i=0;i<n;i++) {
		if(i<a->n) u=a->a[i]; else u=0;
		if(i<b->n) v=b->a[i]; else v=0;
		if((ll)u-(ll)v-B<0) TT.a[i]=(uint)((ll)u-(ll)v-B),B=1;
		else TT.a[i]=(ll)u-(ll)v-B,B=0;
	}
	TT.n=n;
	bigcpy(&TT,c);
	while(c->n && !c->a[c->n-1]) c->n--;
}

/* OK UVa 288 0.044 seconds 12.09.2012 */
void bigmul(big_t *a,big_t *b,big_t *c) {
	int i,j;
	ull r;
	for(TT.n=i=0;i<=a->n+b->n;i++) TT.a[i]=0;
	for(i=0;i<a->n;i++) for(j=0;j<b->n;j++) {
		r=(ull)a->a[i]*b->a[j];
		bigaddat(&TT.a[i+j],r&0xFFFFFFFFu);
		if(r>=(1ULL<<32)) bigaddat(&TT.a[i+j+1],r>>32);
	}
	for(i=a->n+b->n;i>=0 && !TT.a[i];i--);
	c->n=i+1;
	for(i=0;i<c->n;i++) c->a[i]=TT.a[i];
}

/* calculate a/b and a%b. if only one is desired, let the other be NULL */
/* algorithm 9.1.3 in "prime numbers: a computational perspective",
   uses only bitshift/addition/subtraction */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
void bigdiv(big_t *a,big_t *b,big_t *c,big_t *d) {
	int B=1;
	static big_t m,x,t;
	if(c) bigint(0,&t);
	bigcpy(b,&m);
	while(1) {
		bigshiftleft(&m,&x);
		if(bigcmp(&x,a)>0) break;
		bigcpy(&x,&m);
		B++;
	}
	bigcpy(a,&x);
	while(B--) {
		if(c) bigshiftleft(&t,&t);
		if(bigcmp(&x,&m)>-1) {
			if(c) bigaddint(&t,1,&t);
			bigsub(&x,&m,&x);
		}
		bigshiftright(&m,&m);
	}
	if(c) bigcpy(&t,c);
	if(d) bigcpy(&x,d);
}

/* calculate gcd(a,b) and put result in c */
/* OK UVa 10814 0.004 seconds 11.09.2012 */
void biggcd(big_t *a,big_t *b,big_t *c) {
	static big_t x,y,z;
	bigcpy(a,&x);
	bigcpy(b,&y);
	while(!bigcmpzero(&y)) {
		bigcpy(&x,&z);
		bigcpy(&y,&x);
		bigdiv(&z,&x,0,&y);
	}
	bigcpy(&x,c);
}

/* calculate lcm(a,b) and put result in c */
void biglcm(big_t *a,big_t *b,big_t *c) {
	static big_t x;
	biggcd(a,b,&x);
	bigmul(a,&x,&x);
	bigmul(&x,b,c);
}

/* calculate a^b and put result in c */
/* OK UVa 288 0.044 seconds 12.09.2012 */
void bigpowint(big_t *a,uint b,big_t *c) {
	uint m=1;
	static big_t t,u;
	if(!b || !bigcmpint(a,1)) { bigone(c); return; }
	if(bigcmpzero(a)) { bigzero(c); return; }
	bigone(&u);
	bigcpy(a,&t);
	while(1) {
		if(b&m) bigmul(&u,&t,&u);
		if((ull)m+m>b) break;
		m<<=1;
		bigmul(&t,&t,&t);
	}
	bigcpy(&u,c);
}

/* same as above, but inputs are uint */ 
void bigintpowint(uint a,uint b,big_t *c) {
	static big_t z;
	bigint(a,&z);
	bigpowint(&z,b,c);
}

/* TODO mulmod, pow, powmod, powintmod, powintmodint */

void bigdebug(big_t *a) {
	int i;
	printf("[");
	for(i=0;i<a->n;i++) printf("%8x%s",a->a[i],i==a->n-1?"":",");
	puts("]");
}
