/* maximum number of control points */
#define DEG 5

/* lagrange interpolation: given n pairs (x_i,f(x_i)), tries to calculate
   f(z). NB, f(z) must be an integer. */
/* OK UVA 12076 (NWERC 2005 problem A "unequalled consumption") 0.376 seconds 11.09.2012 */
/* OK UVa-live 3408 (NWERC 2005 problem A "unequalled consumption") 0.468 seconds 11.09.2012 */
ll lagrange(ll *x,ll *y,ll z,int n) {
	ll l[DEG],teller,nevner,g,r=0;
	int i,j;
	for(i=0;i<n;i++) {
		teller=nevner=1;
		for(j=0;j<n;j++) if(i!=j) {
			teller*=(z-x[j]);
			nevner*=(x[i]-x[j]);
			g=gcd(teller<0?-teller:teller,nevner<0?-nevner:nevner);
			teller/=g; nevner/=g;
		}
		l[i]=teller/nevner;
	}
	for(i=0;i<n;i++) r+=l[i]*y[i];
	return r;
}
