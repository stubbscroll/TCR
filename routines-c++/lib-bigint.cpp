/* bigint */

#define bi bigint
#define sh show()

int comps(string a,string b) {
   if(a.si<b.si) return -1;
   else if(a.si>b.si) return 1;
   int n=a.si-1;
   fid {
      if(a[i]<b[i]) return -1;
      else if(a[i]>b[i]) return 1;
   }
   return 0;
}

string adds(string a,string b) {
   string t; char d;
   int al=a.si,bl=b.si,n=max(al,bl),c=0;
   fi {
      d=c;
      if(i<al) d+=a[i]-'0';
      if(i<bl) d+=b[i]-'0';
      if((c=(d>9))) d-=10;
      d+='0';
      t+=d;
   }
   if(c) t+='1';
   return t;
}

string subs(string a,string b) {
   string t; char d;
   int al=a.si,bl=b.si,n=max(al,bl),c=0;
   fi {
      d=-c;
      if(i<al) d+=a[i]-'0';
      if(i<bl) d-=b[i]-'0';
      if((c=(d<0))) d+=10;
      d+='0';
      t+=d;
   }
   while(n>1 && t[n-1]=='0') n--;
   if(n<t.si) t=t.substr(0,n);
   return t;
}

string muls(string a,string b) {
   string r; int al=a.si,bl=b.si,k,n; vi t;
   t.resize(al+bl+2,0);
   fir(al) fjr(bl) t[i+j]+=(a[i]-'0')*(b[j]-'0');
   n=al+bl+1;
   fi { k=t[i]/10; t[i]=t[i]%10+'0'; t[i+1]+=k; }
   while(n>1 && t[n-1]=='0') n--;
   r.resize(n);
   fi r[i]=t[i];
   return r;
}

string divs(string a,string b) {
   if(b=="0") return "0";
   string r,c=b,d=a; int al=a.si,n,el=-1;
   n=comps(a,b);
   if(n==0) return "1";
   else if(n==-1) return "0";
   while(comps(a,c)>=0) { c='0'+c; el++; }
   c=c.substr(1);
   r.resize(al,'0');
   while(1) {
      while(comps(d,c)>=0) {
         r[el]++;
         d=subs(d,c);
      }
      el--;
      if(c.si==1) break;
      c=c.substr(1);
   }
   n=al;
   while(n>1 && r[n-1]=='0') n--;
   if(n<r.si) r=r.substr(0,n);
   return r;
}

class bigint {
private:
   string num; // backwards
   int sign; // -1 or 1
public:
   void make(string s) {
      int o=0,sl;
      if(s[0]=='-') { sign=-1; s=s.substr(1); } else sign=1;
      sl=s.si;
      while(s[o]=='0' && o<sl-1) o++;
      num.resize(sl-o,' ');
      fr(i,o,sl)
         num[i-o]=s[sl-i+o-1];
      if(num=="0") sign=1;
   }
   bigint() { sign=1; num="0"; }
   bigint(string s) { make(s); }
   bigint(int i) { make(itos(i)); }
   void set(string s) { make(s); }
   void set(int i) { make(itos(i)); }

   string show() {
      string t;
      if(sign<0) t+="-";
      fird(num.si-1) t+=num[i];
      return t;
   }
   /* 0: equal -1:b is larger, 1: b is less */
   int comp(bigint b) {
      if(sign<b.sign) return -1;
      else if(sign>b.sign) return 1;
      return comps(num,b.num);
   }

   void add(bigint b) {
      if(sign==b.sign) num=adds(num,b.num);
      else {
         int c=comps(num,b.num);
         if(c<0 && sign<b.sign) { num=subs(b.num,num); sign=1; }
         else if(c>=0 && sign<b.sign) { num=subs(num,b.num); sign=-1; }
         else if(c>=0 && sign>b.sign) { num=subs(num,b.num); sign=1; }
         else if(c<0 && sign>b.sign) { num=subs(b.num,num); sign=-1; }
      }
      if(num=="0") sign=1;
   }

   void sub(bigint b) {
      if(sign==b.sign) {
         int c=comps(num,b.num);
         if(c>=0) { num=subs(num,b.num); sign=1; }
         else { num=subs(b.num,num); sign=-1; }
      } else if(sign<b.sign) num=adds(num,b.num);
      else if(sign>b.sign) num=adds(num,b.num);
      if(num=="0") sign=1;
   }

   void mul(bigint b) {
      num=muls(num,b.num);
      sign=sign*b.sign;
      if(num=="0") sign=1;
   }

   void div(bigint b) {
      num=divs(num,b.num);
      sign=sign*b.sign;
      if(num=="0") sign=1;
   }
};
