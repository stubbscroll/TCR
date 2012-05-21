#define MAXS 100
#define MAXL 100

char t[MAXS][MAXL];
int tn,nt;

void split(char *s)
{
   int i,sl;

   tn=nt=0; sl=strlen(s);
   for(i=0;i<=sl;i++)
      if(s[i] && s[i]!=' ') {
         t[tn][nt++]=s[i];
      } else if(nt) {
         t[tn++][nt]=0; nt=0;
   }
}

