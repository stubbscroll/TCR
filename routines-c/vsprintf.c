#include <stdlib.h>

/*  vprintf: print directly
    vsprintf: print to string buffer
    vfprintf: print to file or stdin/out/err  */

char buffer[1024];
void printtobuffer(char *s,...) {
  va_list argptr;
  va_start(argptr,s);
  vsprintf(buffer,s,argptr);
  va_end(argptr);
}
