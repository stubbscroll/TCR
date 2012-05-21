/* two players, array step[], n piles, step[x] is the number of stones in each pile */
   rules: a player can take any number of stones from the leftmost pile
   the player who takes the last stone wins
   player 1 begins */

int n,step[1000];

/* 0: player 1 wins, 1: player 2 wins */
int solve()
{
   int i,t;

   /* if there are no piles, the last player already won */   
   t=1;
   for(i=n-1;i>=0;i--)
      /* if there are more stones in this pile, the player can take either all or all-1
         stones such that he will force a losing position upon his opponent the next turn */
      if(step[i]>1) t=0;
      /* one stone, forced */
      else t=1-t;
   return t;
}
