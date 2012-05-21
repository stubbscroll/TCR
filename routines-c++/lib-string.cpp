vs splits(string s,string d=" ") {
   vs r; string t;
   fir(s.si) if(string::npos!=d.find(s[i])) { if(t!="") r.pb(t); t=""; } else t+=s[i];
   if(t!="") r.pb(t); return r;
}

string itos(int n) {
   char s[15];
   sprintf(s,"%d",n);
   return s;
}

int stoi(string s) {
   int v=0,f=1;
   if(s=="-2147483648") return -2147483647-1;
   if(s[0]=='-') { f=-1; s=s.substr(1); }
   fir(s.si) v=v*10+s[i]-'0';
   return v*f;
}

vi spliti(string s,string d=" ") {
   vi r; string t;
   fir(s.si) if(string::npos!=d.find(s[i])) { if(t!="") r.pb(stoi(t)); t=""; } else t+=s[i];
   if(t!="") r.pb(stoi(t)); return r;
}

string sucase(string s) {
   fir(s.si) s[i]=toupper(s[i]);
   return s;
}

string slcase(string s) {
   fir(s.si) s[i]=tolower(s[i]);
   return s;
}

