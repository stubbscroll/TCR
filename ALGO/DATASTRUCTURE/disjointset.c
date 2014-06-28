#define MAXSET 30000

/* disjoint set! */
/* OK UVa 10147 2012-06-08 */
/* OK UVa 10301 2012-06-08 */
/* OK UVa 10397 2012-05-26 */
/* OK IPSC 2005 C "crane operator" n<=1000000 2014-05-16 */
/* OK IPSC 2008 I "inventing test data" n<=15000 2014-06-08 */
/* OK NCPC 2009 H "beacons" (UVa 11696) 2012-10-05 */
/* OK Topcoder SRM 572 div 1 250 "NewArenaPassword" n<=50, worst 3 ms, 2013-03-07 */

int p[MAXSET],rank[MAXSET];

int findset(int x) {
	if(x!=p[x]) p[x]=findset(p[x]);
	return p[x];
}

void unionset(int x,int y) {
	x=findset(x);
	y=findset(y);
	if(x==y) return;
	if(rank[x]>rank[y]) p[y]=x;
	else {
		p[x]=y;
		if(rank[x]==rank[y]) rank[y]++;
	}
}

void initset(int n) {
	int i;
	for(i=0;i<n;i++) { p[i]=i; rank[i]=0; }
}
