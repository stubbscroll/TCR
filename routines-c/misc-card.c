/* return integer value of card (2-9TJQKA), A=14 */
int cardval(char c)
{
   return isdigit(c)?c-'0':(c=='T'?10:(c=='J'?11:(c=='A'?14:(c=='K'?13:(c=='Q'?12:-1)))));
}

int suitval(char c)
{
   return c=='H'?3:(c=='S'?2:(c=='D'?1:(c=='C'?0:-1)));
}

