/* insert element to heap such that t[i]<=t[parent[i]]. NB: t[] is 1-based */
void heapinsert(int *n,int *t,int val)
{
   int i,j,v;

   i=++(*n);
   while(i>1) {
      j=i>>1;
      if(val<=t[j]) break;
      else t[i]=t[j];
      i=j;
   }
   t[i]=val;
}

/* make node i a heap */
void heapify(int *n,int *t,int i)
{
   int l,r,largest;

again:
   l=i<<1; r=(i<<1)+1;
   if(l<=*n && t[l]>t[i]) largest=l;
   else largest=i;
   if(r<=*n && t[r]>t[largest]) largest=r;
   if(largest!=i) {
      l=t[i]; t[i]=t[largest]; t[largest]=l;
      i=largest;
      goto again;
   }
}

/* remove and return largest element */
int heapextractmax(int *n,int *t)
{
   int max;

   if(*n==0) return 1/0;
   max=t[1];
   t[1]=t[*n];
   (*n)--;
   heapify(n,t,1);
   return max;
}

