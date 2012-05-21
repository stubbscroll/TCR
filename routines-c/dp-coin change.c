/* NOT TESTED */

#define MAX 1000000
#define INF 1000000000

int val[]={5,10,25,50,100,200};
int nv=6;
int dp[MAX];

/* find number of ways to make the amount n with the available coins */
int numways(int n)
{
   int i,j;

   memset(dp,0,sizeof(int)*(n+1));
   dp[0]=1;
   for(j=0;j<nv;j++) {
      for(i=v[j];i<=n;i++) dp[i]+=dp[i-v[j]];
   return dp[n];
}

/* find the least number of coins to make the amount n */
int leastchange(int n)
{
   int i,j;

   dp[0]=0;
   for(i=1;i<n;i++) {
      dp[i]=INF;
      for(j=0;j<nv && i-j>=0;j++) if(dp[i-j]>0) {
         if(dp[i-j]+1<dp[i]) dp[i]=dp[i-j]+1;
      }
   }
   return dp[n];
}
