#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*  return card rank: 0:nothing, 1:pair, 2:two pair, 3:three of a kind,
    4:straight, 5:flush, 6:full house, 7:four of a kind, 8:straight flush,
    9:royal flush. no tiebreakers! */
/*  OK UVa-live 4295 (NWERC 2008 problem K) 2.268 seconds, 09.08.2011 */
int rank(int card[5]) {
  int flush=1,straight=0,freq[13],count[5],i;
  for(i=0;i<13;i++) freq[i]=0;
  for(i=0;i<6;i++) count[i]=0;
  for(i=0;i<5;i++) freq[card[i]%13]++,flush&=(card[0]/13==card[i]/13);
  for(i=0;i<13;i++) count[freq[i]]++;
  for(i=-1;i<9;i++) if(freq[i<0?12:i] && freq[i+1] && freq[i+2] &&
      freq[i+3] && freq[i+4]) { straight=1; break; }
  if(flush && straight) return 8+(freq[8] && freq[12]);
  if(count[4]) return 7;
  if(count[2] && count[3]) return 6;
  if(flush) return 5;
  if(straight) return 4;
  if(count[3]) return 3;
  if(count[2]) return count[2];
  return 0;
}

/*  convert a card string [vs] to number such that value (v) is 0-12 and 
    suit (s) is 0,13,26,39 */
/*  value: 2 3 4 5 6 7 8 9 T J Q K A, suit: c,d,h,s */
/*  OK UVa-live 4295 (NWERC 2008 problem K). 09.08.2011 */
int convert(char *s) {
  int val=0;
  switch(s[1]) {
  case 'c': val=0; break;
  case 'd': val=13; break;
  case 'h': val=26; break;
  case 's': val=39; break;
  }
  switch(s[0]) {
  case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
    val+=s[0]-'2'; break;
  case 'T': val+=8; break;
  case 'J': val+=9; break;
  case 'Q': val+=10; break;
  case 'K': val+=11; break;
  case 'A': val+=12; break;
  }
  return val;
}
