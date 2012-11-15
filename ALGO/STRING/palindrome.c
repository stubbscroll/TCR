#include <stdio.h>
#include <string.h>

/* given a string, find longest substring that is a palindrome! */

/* O(n^2) algorithm
   return using variables at,len such that palindrome starts at s[at]
   and has length len */
/* WARNING, not tested in competitions */
int at,len;
void palindrome(char *s) {
	int i,l,r;
	at=len=-1;
	for(i=0;s[i];i++) {
		/* odd length */
		for(l=r=i;l>=0 && s[l]==s[r];l--,r++);
		if(len<r-l-1) len=r-l-1,at=l+1;
		/* even length */
		if(s[i]==s[i+1]) {
			for(l=i,r=i+1;l>=0 && s[l]==s[r];l--,r++);
			if(len<r-l-1) len=r-l-1,at=l+1;
		}
	}
}

#define MAXSTR 1000000
/* O(n), manacher's algorithm. for each center position, find largest
   palindrome!
   source: http://www.leetcode.com/2011/11/longest-palindromic-substring-part-ii.html
   string cannot contain ASCII-codes 1-2, sentinels are used with these values
   maximal length of input string: MAXSTR (change #define if needed) */
/* OK SPOJ 2426 PLD n<=30000 0.01s 11.11.2012 */
void manacher(char *s) {
	static char t[2*MAXSTR+5];
	static int p[2*MAXSTR+5];
	int n=strlen(s),i,j,n2=2*n+3,c=0,r=0;
	t[0]=2; t[2*n+4]=0; t[1]=1;
	for(i=0;i<n;i++) t[i+i+2]=s[i],t[i+i+3]=1;
	for(i=1;i<n2-1;i++) {
		j=c+c-i;
		p[i]=r>i?(r-i<p[j]?r-i:p[j]):0;
		while(t[i+1+p[i]]==t[i-1-p[i]]) p[i]++;
		if(i+p[i]>r) c=i,r=i+p[i];
	}
	/* now we have an array p[] where p[2*i+1] holds the
	   length of the longest palindrome centered at s[i] and p[2*i+2] holds
	   the length of the longest palindrome centered between s[i] and s[i+1]. */
	/* this code finds the longest palindrone and puts info about it in at,len */
	at=len=-1;
	for(i=1;i<n2-1;i++) if(len<p[i]) len=p[i],at=(i-1-len)>>1;
}

void solve(char *s) {
	int i;
	palindrome(s);
	printf("%d %d\n",at,len);
	for(i=0;i<len;i++) printf("%c",s[i+at]); printf("\n");
	manacher(s);
	printf("%d %d\n",at,len);
	for(i=0;i<len;i++) printf("%c",s[i+at]); printf("\n");
}

int main() {
	solve("abdfgfuireghregueoewabcbaewrfewi");
	solve("madammim");
	solve("regningertralala");
	solve("abc");
	solve("abcc");
	return 0;
}
