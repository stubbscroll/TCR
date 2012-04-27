#include <stdio.h>
#include <string.h>

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned char uchar;

/*  hash! very simple hash table with linear probing to handle
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
/*  OK UVa 851 17.08.2011 */
/*  OK UVa 10181 17.08.2011 */

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
hashdata_t hashdata[MAXHASH];

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
