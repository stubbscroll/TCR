/* how many n-tuples exist of length k with n different values */
long long ntup(int n,int k)
{
   long long v=1; fir(k) v*=n; return v;
}

/* how many sequences of length k with distinct items, out of n different values */
long long ntupd(int n,int k)
{
   long long v=1;
   fir(k) { v*=n; n--; } return v;
}

/* choose k different things out of n things */
long long nCk(int n,int k)
{
   long long b[101][101];
   fr(i,1,101) b[i][0]=1;
   fr(i,1,101) b[i][i]=1;
   fr(i,1,101) fr(j,1,i) b[i][j]=b[i-1][j-1]+b[i-1][j];
   return b[n][k];
}

/* how many non-decreasing sequences of length k with n different values */
long long dp[101][9];

long long ntups(int n,int k)
{
   long long ret=0;
   if(k==0) return 1;
   if(dp[n][k]>-1) return dp[n][k];
   fr(i,1,n+1) ret+=ntups(i,k-1);
   return dp[n][k]=ret;
}

void init()
{
   fir(101) fjr(9) dp[i][j]=-1;
}

/* dp[n][m]: number of partitions of n with m as highest number
   p[n]: number of partitions of n */
long long dp[512][512],p[512];

void partition()
{
   int i,j,n,m;
   for(i=0;i<512;i++) for(j=0;j<512;j++) dp[i][j]=0;
   dp[0][0]=1; p[0]=1;
   for(n=1;n<512;n++) {
      p[n]=1; if(n>1) p[n]++;
      dp[n][1]=1; dp[n][n]=1;
      for(m=2;m<n;m++) {
         dp[n][m]=0;
         for(i=1;i<=m;i++) dp[n][m]+=dp[n-m][i];
         p[n]+=dp[n][m];
      }
   }
}
