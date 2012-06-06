#include <stdio.h>
#include <string.h>

#define MAXP 1024

/* OK UVa 10679 |s|<=1000000, |w|<=1000, 1.348 seconds, 06.05.2012 */

/* knuth-morris-pratt! */
/* find pattern *w in text *s. return index of first match, or -1 if none */
int kmp(char *s,char *w) {
	int m,i,sn=strlen(s),wn=strlen(w),p,c=0;
	static int t[MAXP];
	/* build table */
	t[0]=-1; t[1]=0;
	for(p=2;p<wn;p++) {
		if(w[p-1]==w[c]) c++,t[p]=c,p++;
		else if(c>0) c=t[c];
		else t[p]=0,p++;
	}
	/* search */
	for(i=m=0;m+i<sn;) {
		if(w[i]==s[m+i]) {
			if(i==wn-1) return m;
			i++;
		} else {
			m+=i-t[i];
			i=t[i]>-1?t[i]:0;
		}
	}
	return -1;
}
