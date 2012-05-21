int mod[32];

int calc(int b,int p,int m)
{
   long long r;
   long long i;

   if(b==0) return 0;
   if(p==0) return 1;
   r=0;
   mod[0]=b%m;
   i=1;
   do {
      r++;
      i*=2;
      mod[r]=(long long)mod[r-1]%m*mod[r-1]%m;
   } while(i<p);
   r=1;
   for(i=0;i<31;i++)
      if((p >> i)&1)
         r=r*mod[i]%m;
   return r;
}

