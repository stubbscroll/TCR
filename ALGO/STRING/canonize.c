#define MAXW 400000
#define MAXS 40
char w[MAXW][MAXS];
char canon[MAXW][MAXS];
int nw;

/* generate canonical representation of word, in terms of letter patterns.
   the i-th unique letter in the word is replaced with the value i.
   non-letters are ignored, and simply results in a shorter canonical
   representation. warning, 1-indexed! this allows c-string format with
   0-termination. example. competition => 12345676728.
   this format is handy for checking for word matches in substitution
   ciphers. */
void canonize(char *s,char *t) {
	char taken[26];
	int i,j,ix=1,c;
	memset(taken,-1,26);
	for(i=j=0;s[i];i++) {
		c=-1;
		if(s[i]>='A' && s[i]<='Z') c=s[i]-'A';
		if(s[i]>='a' && s[i]<='z') c=s[i]-'a';
		if(c>-1) {
			if(taken[c]==-1) taken[c]=ix++;
			t[j++]=taken[c];
		}
	}
	t[j]=0;
}

/* read wordlist from file and generate canonical representation */
void gencanon(char *filename) {
	FILE *f=fopen(filename,"r");
	char s[1000];
	if(!f) { puts("error"); return; }
	nw=0;
	while(fscanf(f,"%999s",s)==1) {
		if(nw>=MAXW) { puts("too many words"); return; }
		if(strlen(s)>MAXS-1) { puts("word too long"); return; }
		strcpy(w[nw],s);
		canonize(w[nw],canon[nw]);
		nw++;
	}
	fclose(f);
}
