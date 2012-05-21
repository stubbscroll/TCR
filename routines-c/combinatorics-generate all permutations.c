int compchar (char *a,char *b)
{
  return (*a>*b)-(*a<*b);
}

void visit(char *s)
{
   printf("%s\n",s);
}

void permute(char *s,void (*visit)(char *s))
{
   int n,l,j,k;
   char a[100]={0},t;

   strcpy(a+1,s);
   n=strlen(a+1);
   qsort(a+1,n,1,compchar);
   do {
      visit(a+1);
      j=n-1;
      while(a[j]>=a[j+1]) j--;
      if(j==0) break;
      l=n;
      while(a[j]>=a[l]) l--;
      t=a[j]; a[j]=a[l]; a[l]=t;
      k=j+1; l=n;
      while(k<l) {
         t=a[k]; a[k]=a[l]; a[l]=t;
         k++; l--;
      }
   } while(1);
}

