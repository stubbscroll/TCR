/* hamilton path in undirected graph!
   based on "a search procedure for hamilton paths and circuits"
	 by frank rubin (1973)

   usage:
	 - set n to the number of nodes and set ne=0
   - add edges (add both ways manually!)
	 - run radixsort
	 - run hamcycle with desired start node, returns 0 if no cycle found,
     otherwise 1
   - path is in path[]
*/

/* OK UVa 775 2013-06-06 n<=256 0.356 s */
/* OK hacker.org "oneofus" n<= */

#define MAXV 256
#define MAXE 65536

#define UNKNOWN 0
#define REQUIRED 1
#define DELETED 2

int from[MAXE],to[MAXE];
int gs[MAXV+1];
int n,ne;

char taken[MAXV];
char edge[MAXE];    /* edge status */
int eptr[MAXE];     /* pointer to canonical edge, from<to */
int q[MAXE*10],qe;  /* queue of edge status changes */
int deg[MAXV];      /* degree of non-deleted */
int req[MAXV];      /* number of required edges incident to node */

int path[MAXV];     /* node visited at step i */
int pathedge[MAXV]; /* edge taken when going from node path[i], it's eptr'd */
int maxiter=-1,iter;

/* return 1 if we changed. e must be eptr'd */
int change(int e,int v) {
	if(edge[e]!=v) {
		if(v==DELETED) deg[from[e]]--,deg[to[e]]--;
		else if(v==REQUIRED) req[from[e]]++,req[to[e]]++;
		q[qe++]=e;
		q[qe++]=edge[e];
		edge[e]=v;
		return 1;
	}
	return 0;
}

/* follow required edges and return the other loose end */
int follow(char *t2,int u,int *steps) {
	int v=-1,i,c=0;
	t2[u]=1;
	for(i=gs[u];i<gs[u+1];i++) if(edge[eptr[i]]==REQUIRED) {
		v=to[i]; t2[v]=1; c++;
		break;
	}
	if(v<0) { *steps=0; return -1; }
again:
	for(i=gs[v];i<gs[v+1];i++) if(edge[eptr[i]]==REQUIRED && to[i]!=u) {
		u=v; v=to[i]; t2[v]=1; c++;
		goto again;
	}
	*steps=c;
	return v;
}

int num[MAXV],lav[MAXV],teller;

int dfs3(int u,int p) {
	int ch=0,i,v;
	lav[u]=num[u]=teller++;
	for(i=gs[u];i<gs[u+1];i++) if(edge[eptr[i]]!=DELETED) {
		v=to[i];
		if(num[v]<0) {
			ch++;
			if(dfs3(v,u)) return 1;
			if(lav[u]>lav[v]) lav[u]=lav[v];
			if(p>-1 && lav[v]>=num[u]) return 1;
		} else if(v!=p && lav[u]>num[v]) lav[u]=num[v];
	}
	return !num[u] && ch>1;
}

/* check if graph has articulation points */
int articulation() {
	memset(num,-1,n*sizeof(int));
	memset(lav,-1,n*sizeof(int));
	teller=0;
	return dfs3(0,-1);
}

int reach() {
	static int q[MAXV+5];
	static char vis[MAXV];
	int start=0,cur,i,qs=0,qe=0;
	memset(vis,0,n);
	q[qe++]=start;
	vis[start]=1;
	while(qs<qe) {
		cur=q[qs++];
		for(i=gs[cur];i<gs[cur+1];i++) if(!vis[to[i]] && edge[eptr[i]]!=DELETED) {
			vis[to[i]]=1;
			q[qe++]=to[i];
		}
	}
	return qe==n;
}

/* this routine could be much faster */
int admissible(int d) {
	int i,j,changed,steps,v;
	static char t2[MAXV];
	/* we added edge from path[d-1] to path[d] via edge id pathedge[d-1] */
	change(pathedge[d-1],REQUIRED);
	do {
		changed=0;
		/* D1: if a node has two required edges, delete the rest */
		for(i=0;i<n;i++) if(req[i]==2 && deg[i]>2) for(j=gs[i];j<gs[i+1];j++)
			if(edge[eptr[j]]==UNKNOWN) changed|=change(eptr[j],DELETED);
		/* R2: if a node has two edges they are required */
		for(i=0;i<n;i++) if(deg[i]==2 && req[i]<2) for(j=gs[i];j<gs[i+1];j++)
			if(edge[eptr[j]]==UNKNOWN) changed|=change(eptr[j],REQUIRED);
		/* F1/2: fail if a node has degree <2 */
		/* F5: fail if a node has >2 required edges */
		for(i=0;i<n;i++) if(deg[i]<2 || req[i]>2) return 0;
		/* F6: fail is graph is disconnected (more reliable than the check for
		   closed cycles in the original paper */
		if(!reach()) return 0;
		/* D3: delete edges that would lead to a closed non-ham cycle */
		memset(t2,0,n);
		for(i=0;i<n;i++) if(!t2[i] && req[i]==1) {
			v=follow(t2,i,&steps);
			if(steps<2 || steps==n-1) continue;
			/* find edge u->v and forbid it */
			for(j=gs[i];j<gs[i+1];j++) if(to[j]==v) changed|=change(eptr[j],DELETED);
		}
	} while(changed);
	/* F9: fail if an articulation point exist */
/*	if(articulation()) return 0;*/
	return 1;
}

void undochanges(int qs) {
	int e,v;
	while(qe>qs) {
		v=q[--qe];
		e=q[--qe];
		if(edge[e]==DELETED) deg[from[e]]++,deg[to[e]]++;
		else if(edge[e]==REQUIRED) req[from[e]]--,req[to[e]]--;
		edge[e]=v;
	}
}

int btr(int at,int d,int qf) {
	int i,r=0,s;
	if(iter++==maxiter) return 2;
	if(d==n) {
		for(i=gs[at];i<gs[at+1];i++) if(to[i]==path[0]) return 1;
		return 0;
	}
	/* check for forced edge */
	for(i=gs[at];i<gs[at+1];i++) if(!taken[to[i]] && edge[eptr[i]]==REQUIRED) {
		r=1;
		goto process;
	}
	for(i=gs[at];i<gs[at+1];i++) if(!taken[to[i]] && edge[eptr[i]]!=DELETED) {
	process:
		taken[to[i]]=1;
		path[d]=to[i];
		pathedge[d-1]=eptr[i];
		if(admissible(d)) {
			s=btr(to[i],d+1,qe);
			if(s) return s;
		}
		undochanges(qf);
		taken[to[i]]=0;
		if(r) break;
	}
	return 0;
}

void inverseedges() {
	static int starts[MAXV+1];
	int i;
	memcpy(starts,gs,sizeof(int)*(n+1));
	for(i=0;i<ne;i++) eptr[i]=starts[to[i]]++;
	for(i=0;i<ne;i++) if(from[i]<to[i]) eptr[i]=i;
}

void radixsort() {
	static int newfrom[MAXE],newto[MAXE];
	int i,j;
	/* sort on to */
	memset(gs,0,sizeof(int)*(n+1));
	for(i=0;i<ne;i++) gs[to[i]]++;
	for(i=1;i<n;i++) gs[i]+=gs[i-1];
	for(i=ne-1;i>=0;i--) {
		j=--gs[to[i]];
		newto[j]=to[i];
		newfrom[j]=from[i];
	}
	/* (stable) sort on from */
	memset(gs,0,sizeof(int)*(n+1));
	for(i=0;i<ne;i++) gs[newfrom[i]]++;
	for(i=1;i<n;i++) gs[i]+=gs[i-1];
	gs[n]=ne;
	for(i=ne-1;i>=0;i--) {
		j=--gs[newfrom[i]];
		to[j]=newto[i];
		from[j]=newfrom[i];
	}
	inverseedges();
}

void addedge(int a,int b) {
	from[ne]=a; to[ne++]=b;
}

/* return 1 if hamcycle exists, 0 otherwise */
int hamcycle(int start) {
	int i;
	memset(taken,0,n);
	taken[start]=1;
	path[0]=start;
	qe=iter=0;
	for(i=0;i<n;i++) deg[i]=gs[i+1]-gs[i];
	for(i=0;i<n;i++) req[i]=0;
	for(i=0;i<ne;i++) edge[i]=UNKNOWN;
	return btr(start,1,0);
}
#undef UNKNOWN
#undef REQUIRED
#undef DELETED
