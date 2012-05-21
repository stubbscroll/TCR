/*  generate assembly code on the fly and execute it */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

int a[1000];
void *p;
uchar *r;
void (*q)(void);

/*  place whatever assembly code here that you want to hex dump */
void test() {
  __asm__ volatile(
    "ret\n\t"
    "movl $12345,%%eax\n\t"
    "movl %%eax,%0\n\t"
    "ret"
    :"=m"(a[0])
    :
    :"%eax"
  );
}

/*  do the actual hex dump */
void print() {
  int i;
  uchar *z=(uchar *)test;
  for(i=0;i<50;i++) printf("%02x ",z[i]);
  printf("\n");
}

void makeprogram() {
  int p=0;
  r[p++]=0xB8;  /*  MOV $12345, %EAX */
  r[p++]=0x39;
  r[p++]=0x30;
  r[p++]=0x00;
  r[p++]=0x00;
  r[p++]=0xa3;  /*  MOV %EAX, a[0] */
  r[p++]=0x80;
  r[p++]=0x50;
  r[p++]=0x40;
  r[p++]=0x00;
  r[p++]=0xC3;  /*  RET (near return to calling procedure) */
}

int main() {
  if(!(p=malloc(1000000))) return 0;
  q=p;
  r=p;
  print();
  makeprogram();
  a[0]=0;
  printf("%d %d %d %d %d\n",(int)p,(int)q,(int)r,r[0],a[0]);
  puts("before call");
  (*q)();
  printf("after call %d\n",a[0]);
  free(p);
  return 0;
}
