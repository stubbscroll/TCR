int rand31()
{
   return rand()+(rand()<<15)+((rand()&1)<<30);
}

