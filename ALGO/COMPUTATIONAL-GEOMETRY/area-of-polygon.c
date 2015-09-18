/* signed area*2 of polygon */
/* can overflow when |x|,|y| approaches sqrt(2^31), change to ll if so */
/* OK UVa 109 2012-05-25 */
/* OK UVa 12300 2012-06-11 */
/* OK Project Euler 507 2015-02-24 */
int polyarea2(int *x,int *y,int n) {
	int a=0,i,j;
	for(j=n-1,i=0;i<n;j=i++) a+=x[j]*y[i]-x[i]*y[j];
	return a;
}
