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

/* Warning, old hash code below. */
/* hash! very simple hash table with linear probing to handle
   collisions.
   TODO i have yet to analyse the runtime of gethashpot, as well
   as suggest sensible hash functions.
   regarding hash table size and hash function:
   - the size should be large enough to accomodate every entry, as well
     as having some extra room so that the linear probe won't happen
     too often.
   - the hash function should be chosen so that the hash value is
     distributed nicely across the hash table, and the number of
     collisions should be low.
*/

/* OK UVa 744 n<=100 0.112 seconds 27.06.2012 */
/* OK UVa 851 17.08.2011 */
/* OK UVa 10181 17.08.2011 */
/* OK UVa 10226 n<=10000 0.496 seconds 05.06.2012 */
/* OK TJU 2486 (NCPC 2006 F "traveling salesman", n<=120000, 0.06 seconds, 01.10.2012 */
/* OK GCJ 2012-R2-D-small 26.05.2012 */
typedef struct {
  ull key;      /*  id of this state. mandatory field */
  ull parent;   /*  key to parent */
  int dist;     /*  distance from start */
  char closed;  /*  1: node is in the closed list */
  char dir;     /*  direction taken in previous state to reach this */
} hashdata_t;

#define MAXHASH 100007
int maxhash=MAXHASH;

uchar hash[(MAXHASH+7)/8];  /*  bit i set if element i is taken */
hashdata_t hashdata[MAXHASH]; /* this need not be initialized, actually */

/*  hash function */
#define GETHASH(key) key%maxhash

/*  return taken bit: 0: not set, >0: set */
#define HASHBIT(pos) (hash[pos>>3]&(1<<(pos&7)))

/*  given key, find its position in hash table */
/*  it probes past entries occupied by other keys, and stops at
    either the right key or the first empty entry */
int gethashpos(ull key) {
  int pos=GETHASH(key);
  while(1) {
    if(!HASHBIT(pos)) break;
    if(hashdata[pos].key==key) break;
    pos++; if(pos==maxhash) pos=0;
  }
  return pos;
}

int exists(ull key) {
	int pos=gethashpos(key);
	if(!HASHBIT(pos)) return 0;
	return hashdata[pos].key==key);
}


/*  put an element into the hash table */
void puthash(hashdata_t *h) {
  int pos=gethashpos(h->key);
  hash[pos>>3]|=1<<(pos&7);
  hashdata[pos]=*h;
}

/*  clears the hash table */
void inithashdata() {
  memset(hash,0,sizeof(hash));
}
