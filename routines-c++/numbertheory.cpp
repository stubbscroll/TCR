int gcd(int a,int b) {
   while(b) { a%=b; swap(a,b); }
   return abs(a);
}

void genprimes(vector<bool> &sieve,vector<int> &primes,int MAKS) {
   sieve=vector<bool>(MAKS,1);
   primes=vector<int>();
   for(int i=2;i*i<MAKS;i++) if(sieve[i]) for(int j=i*i;j<MAKS;j+=i) sieve[j]=0;
   for(int i=2;i<MAKS;i++) if(sieve[i]) primes.push_back(i);
}

bool isprime(long long n,vector<int> &primes) {
   if(n<2) return false;
   for(int i=0;(long long)primes[i]*primes[i]<=n;i++) if(n%primes[i]==0) return false;
   return true;
}

bool isprobableprime(long long n) {
   
}

