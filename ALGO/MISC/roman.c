/* convert between int and roman numerals */

int v[128];

void initroman() {
  v['I']=1; v['V']=5; v['X']=10; v['L']=50;
  v['C']=100; v['D']=500; v['M']=1000; v[0]=0;
}

/* OK UVa 12522 20.10.2012 */
int romantoint(char *s) {
	int i,j=0;
	for(i=0;s[i];i++) if(v[s[i]]<v[s[i+1]]) j-=v[s[i]]; else j+=v[s[i]];
	return j;
}

/* OK UVa 12522 20.10.2012 */
void inttoroman(int n,char *s) {
	int i=0;
	while(n>=1000) { s[i++]='M'; n-=1000; }
	while(n>=900) { s[i++]='C'; s[i++]='M'; n-=900; }
	while(n>=500) { s[i++]='D'; n-=500; }
	while(n>=400) { s[i++]='C'; s[i++]='D'; n-=400; }
	while(n>=100) { s[i++]='C'; n-=100; }
	while(n>=90) { s[i++]='X'; s[i++]='C'; n-=90; }
	while(n>=50) { s[i++]='L'; n-=50; }
	while(n>=40) { s[i++]='X'; s[i++]='L'; n-=40; }
	while(n>=10) { s[i++]='X'; n-=10; }
	while(n>=9) { s[i++]='I'; s[i++]='X'; n-=9; }
	while(n>=5) { s[i++]='V'; n-=5; }
	while(n>=4) { s[i++]='I'; s[i++]='V'; n-=4; }
	while(n>=1) { s[i++]='I'; n-=1; }
	s[i]=0;
}
