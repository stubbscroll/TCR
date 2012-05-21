double eval();

/* ternary search for parabolic and similar functions, find minimum value */
double ternarysearch(double m,double s)
{
   double t1,t2,t3,d;
   double r1,r2,r3,r15,r25;

   t1=EPS; t3=m;
   while(1) {
      t2=(t1+t3)/2;
      r1=eval(m,s,t1); r2=eval(m,s,t2); r3=eval(m,s,t3);
      if(equal(r1,r2) && equal(r2,r3)) break;
      if(r1>r2 && r2<r3) {
         d=(t3-t1)/2;
         r15=eval(m,s,t2-d); r25=eval(m,s,t2+d);
         if(r15<r25 && r15<r3) t3=t2;
         else if(r3<r15 && r3<r25) {
            t1+=d; t2-=d;
         } else t1=t2;
         continue;
      } else if(r1>r2 && r1>r3) {
         t1=t2; continue;
      } else if(r3>r2 && r3>r1) {
         t3=t2; continue;
      } else return 1/0;
   }
   return r2;
}

