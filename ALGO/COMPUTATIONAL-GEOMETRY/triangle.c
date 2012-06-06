

/*	calculate triangle area given side lengths. this is the
		slightly more numerically stable version */
/*	OK UVa 10347 0.008 seconds 26.05.2012 */
double trianglearea(double a,double b,double c) {
	return sqrt((a+b-c)*(a-b+c)*(-a+b+c)*(a+b+c))/4;
}
