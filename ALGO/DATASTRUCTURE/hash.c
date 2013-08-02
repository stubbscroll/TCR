#include <stdio.h>
#include <string.h>

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned char uchar;

/* hash table! to change type of key, change lines marked with (*) */
/* OK UVa 1152 n<=16000000, 1.616 seconds, 14.12.2012 */

#define MAXHASH 40000007

typedef unsigned char uchar;
typedef struct {
	int key;    /* (*) */
	int count;  /* value */
} hashdata_t;
int maxhash=MAXHASH;
uchar hash[(MAXHASH+7)/8];
hashdata_t hashdata[MAXHASH];

#define HASHBIT(pos) (hash[(pos)>>3]&(1<<((pos)&7)))
#define HASHCOMPARE(key1,key2) key1==key2

/* this function might be ok if key is int, change otherwise */
int GETHASH(int key) {
	int ret=key%maxhash;
	if(ret<0) ret+=maxhash;
	return ret;
}

/* get position of key in hash table regardless of existence */
int gethashpos(int key) { /* (*) */
	int pos=GETHASH(key);
	while(1) {
		if(!HASHBIT(pos)) break;
		if(HASHCOMPARE(key,hashdata[pos].key)) break;
		pos++; if(pos==maxhash) pos=0;
	}
	return pos;
}

/* look up key */
hashdata_t *lookuphash(int key) { /* (*) */
	int pos=gethashpos(key);
	return (HASHBIT(pos))?hashdata+pos:NULL;
}

/* put element in hash table, will overwrite if key exists */
void puthash(hashdata_t *h) {
	int pos=gethashpos(h->key);
	hashdata[pos]=*h;
	hash[pos>>3]|=1<<(pos&7);
}

/* clear hash table */
void inithashdata() {
	memset(hash,0,sizeof(hash));
}

/* old hash code removed, see earlier commit */
