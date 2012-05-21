/*  generate assembly code on the fly and execute it */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

int a[1000];
void *p;
uchar *r;
void (*q)(void);

void makeprogram() {
  int p=0;
  r[p++]=0xB8;  /*  MOV $12345, %EAX */
  r[p++]=0x39;
  r[p++]=0x30;
  r[p++]=0x00;
  r[p++]=0x00;
  r[p++]=0xa3;  /*  MOV %EAX, a[0] */
  r[p++]=(int)a&255;
  r[p++]=((int)a>>8)&255;
  r[p++]=((int)a>>16)&255;
  r[p++]=(int)a>>24;
  r[p++]=0xC3;  /*  RET (near return to calling procedure) */
}

int main() {
  if(!(p=malloc(1000000))) return 0;
  q=p;
  r=p;
  makeprogram();
  a[0]=0;
  printf("before call: a[0]=%d\n",a[0]);
  (*q)();
  printf("after call: a[0]=%d\n",a[0]);
  free(p);
  return 0;
}
