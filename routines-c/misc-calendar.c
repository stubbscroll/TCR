#define ISLEAP(y) (!(y&3) && (y%100 || y%400==0))
#define FEB(y,m) (m==1 && ISLEAP(y))
#define DAYS(y,m) (days[m]+FEB(y,m))

const int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
const char mn[12][4]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP",
   "OCT","NOV","DEC"};
const char dn[7][3]={"MON","TUE","WED","THU","FRI","SAT","SUN"};

void corrup()
{
   min+=sec/60; sec%=60;
   if(min>=0 && min<60) return;
   hour+=min/60; min%=60;
   if(hour>=0 && hour<24) return;
   while(hour>23) { hour-=24; date++; day=(1+day)%7; }
   if(date>0 && date<=DAYS(year,mon)) return;
   while(date>DAYS(year,mon)) {
      date-=DAYS(year,mon); mon++;
      if(mon>11) {
         mon=0; year++;
      }
   }
}

void corrdown()
{
   while(sec<0) { sec+=60; min--; }
   if(min>=0 && min<60) return;
   while(min<0) { min+=60; hour--; }
   if(hour>=0 && hour<24) return;
   while(hour<0) { hour+=24; date--; day=(6+day)%7; }
   if(date>0 && date<=DAYS(year,mon)) return;
   while(date<1) {
      mon--;
      if(mon<0) {
         mon=11; year--;
      }
      date+=DAYS(year,mon);
   }
}

void changetime(int c)
{
   sec+=c;
   if(sec>59) corrup();
   if(sec<0) corrdown();
}

long long countsec(int starty,int year,int mon,int date,int hour,int min,int sec)
{
   int i;
   long long v=0;

   for(i=starty;i<year;i++) v+=(365+ISLEAP(i))*24*3600;
   for(i=0;i<mon-1;i++) v+=DAYS(year,i)*24*3600;
   return v+(date-1)*24*3600+hour*3600+min*60+sec;
}

