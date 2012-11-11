/* find lexicographically smallest rotation of cyclic string! */
/* algorithm by zhou yuan, called minimum expression */
/* warning, input string s is destroyed and has another copy of
   itself concatenated, the buffer needs to be large enough. */
/* source: couldn't find any first-hand source, only this
   http://online-judge.uva.es/board/viewtopic.php?t=42601&f=22 */
/* OK UVa 719 |s|<=10000 0.020 seconds 23.06.2012 */
int minexp(char *s) {
	int n=strlen(s),i,j,k;
	for(i=0;i<n;i++) s[i+n]=s[i];
	for(n+=n,s[n]=i=k=0,j=1;i+k<n && j+k<n;) {
		if(s[i+k]==s[j+k]) k++;
		else if(s[i+k]>s[j+k]) { i=i+k+1; k=0; if(i<=j) i=j+1; }
		else { j=j+k+1; k=0; if(j<=i) j=i+1; }
	}
	return i<j?i:j;
}
