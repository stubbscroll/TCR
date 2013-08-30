/* hash table for large keys/values encoded as char */

ll TOTHASH;           /* number of bytes set aside for hash table */
ll MAXHASH;           /* capacity of hash table (number of elements) */
ll MAGICMUL=177LL;    /* magic multiplier */
ll numhash;           /* number of elements currently in hash table */
int hsize;            /* size of key in bytes */
int vsize;            /* size of value in bytes */

uchar *hash;          /* bitfield showing taken elements */
uchar *hashdata;      /* main storage area for keys+values */

/* check if pos in hashtable is taken */
#define HASHBIT(pos) (hash[(pos)>>3]&(1<<((pos)&7)))

/* compare entry in hashtable with key pair given in q (value is not compared)
   1 if equal, 0 if different */
int HASHCOMPARE(ll ix,uchar *q) {
	int i;
	ll p=ix*(hsize+dsize);
	/* TODO investigate memcmp */
	for(i=0;i<hsize;i++) if(hashdata[p+i]!=q[i]) return 0;
	return 1;
}

/* return hashvalue of key */
ll GETHASH(uchar *p) {
	int i;
	ll h=0;
	for(i=0;i<hsize;i++) h=(h*MAGICMUL+p[i])%MAXHASH;
	return h;
}

/* get position of key in hash table. if key doesn't exist, it will end
   up at the returned position */
ll gethashpos(uchar *p) {
	ll pos=GETHASH(p);
	while(1) {
		if(!HASHBIT(pos)) break;
		if(HASHCOMPARE(pos,p)) break;
		pos++; if(pos==MAXHASH) pos=0;
	}
	return pos;
}

/* check if key exists */
int lookuphash(uchar *p) {
	ll pos=gethashpos(p);
	return HASHBIT(pos)?1:0;
}

/* put key/value in hash table, will overwrite if key exists */
void puthash(uchar *p) {
	ll pos=gethashpos(p);
	int i;
	for(i=0;i<hsize+dsize;i++) hashdata[i+pos*(hsize+dsize)]=p[i];
	hash[pos>>3]|=1<<(pos&7);
	numhash++;
}

/* helper function as we want MAXHASH prime. called once in init,
   so we don't care about efficiency */
int isprime(ll z) {
	int i;
	if(z<4) return z>1;
	if(z%2==0) return 0;
	for(i=3;(ll)i*i<=z;i+=2) if(z%i==0) return 0;
	return 1;
}

/* initialize hash table - hsize and vsize must be set before */
/* remember to free hash and hashdata after use! */
void inithash(ll mem) {
	TOTHASH=mem;
	MAXHASH=(int)(TOTHASH/(hsize+vsize+0.125));
	numhash=0;
	while(!isprime(MAXHASH)) MAXHASH--;
	if(!(hash=malloc((MAXHASH+7)>>3))) puts("error");
	if(!(hashdata=malloc(MAXHASH*(hsize+vsize)))) puts("error");
	memset(hash,0,(MAXHASH+7)>>3);
}
