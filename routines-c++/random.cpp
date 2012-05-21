// return a random integer from [0,2^31)
int rand31()
{
   return rand()+(rand()<<15)+((rand()&1)<<30);
}

// return a random real number from [0,1)
double rand01(){return ((rand()%32768)/32768.0+(rand()%32768))/32768.0;}

// return a gaussian random number with mean 0 and st 1, needs <cmath>
double gauss()
{
   double pi=3.1415926535897932,r1=-log(1-rand01()),r2=2*pi*rand01();
   r1=sqrt(2*r1);
   return r1*cos(r2);
}
