#include <stdio.h>

#define FANCYCALL(thecall,var) var+=123; thecall; var+=20;

int fib(int n,int m) {
	printf("tralala %d ",m);
	if(n<2) return n;
	else return fib(n-1,m+2)+fib(n-2,m+1);
}

int main() {
	double tid=0;
	int tall;
	FANCYCALL(tall=fib(10,5),tid);
	printf("%f %d\n",tid,tall);
	return 0;
}
