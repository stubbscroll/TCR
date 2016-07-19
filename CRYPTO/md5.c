/* can be compiled with -O2 and -O3 now that the bug has been fixed, i guess */
#include <stdio.h>
#include <string.h>
typedef unsigned char uchar;
#define F(x,y,z) (((x)&(y))|((~(x))&(z)))
#define G(x,y,z) (((x)&(z))|((y)&(~(z))))
#define H(x,y,z) ((x)^(y)^(z))
#define I(x,y,z) ((y)^((x)|(~(z))))
#define ROT(x,n) (((x)<<(n))|((x)>>(32-(n))))
#define R1(a,b,c,d,k,s,i) a=a+F(b,c,d)+X[k]+i,a=ROT(a,s)+b
#define R2(a,b,c,d,k,s,i) a=a+G(b,c,d)+X[k]+i,a=ROT(a,s)+b
#define R3(a,b,c,d,k,s,i) a=a+H(b,c,d)+X[k]+i,a=ROT(a,s)+b
#define R4(a,b,c,d,k,s,i) a=a+I(b,c,d)+X[k]+i,a=ROT(a,s)+b
unsigned T[64];
/*  run this before md5() */
void md5init() {
	/* just to be sure, hardcode these values instead of doing trig stuff */
  T[0]=3614090360U;
  T[1]=3905402710U;
  T[2]=606105819U;
  T[3]=3250441966U;
  T[4]=4118548399U;
  T[5]=1200080426U;
  T[6]=2821735955U;
  T[7]=4249261313U;
  T[8]=1770035416U;
  T[9]=2336552879U;
  T[10]=4294925233U;
  T[11]=2304563134U;
  T[12]=1804603682U;
  T[13]=4254626195U;
  T[14]=2792965006U;
  T[15]=1236535329U;
  T[16]=4129170786U;
  T[17]=3225465664U;
  T[18]=643717713U;
  T[19]=3921069994U;
  T[20]=3593408605U;
  T[21]=38016083U;
  T[22]=3634488961U;
  T[23]=3889429448U;
  T[24]=568446438U;
  T[25]=3275163606U;
  T[26]=4107603335U;
  T[27]=1163531501U;
  T[28]=2850285829U;
  T[29]=4243563512U;
  T[30]=1735328473U;
  T[31]=2368359562U;
  T[32]=4294588738U;
  T[33]=2272392833U;
  T[34]=1839030562U;
  T[35]=4259657740U;
  T[36]=2763975236U;
  T[37]=1272893353U;
  T[38]=4139469664U;
  T[39]=3200236656U;
  T[40]=681279174U;
  T[41]=3936430074U;
  T[42]=3572445317U;
  T[43]=76029189U;
  T[44]=3654602809U;
  T[45]=3873151461U;
  T[46]=530742520U;
  T[47]=3299628645U;
  T[48]=4096336452U;
  T[49]=1126891415U;
  T[50]=2878612391U;
  T[51]=4237533241U;
  T[52]=1700485571U;
  T[53]=2399980690U;
  T[54]=4293915773U;
  T[55]=2240044497U;
  T[56]=1873313359U;
  T[57]=4264355552U;
  T[58]=2734768916U;
  T[59]=1309151649U;
  T[60]=4149444226U;
  T[61]=3174756917U;
  T[62]=718787259U;
  T[63]=3951481745U;
}
/*  b: message to decode, len: length of message, h: where to output digest */
void md5(uchar *b,int len,uchar *h) {
  static uchar M[16384],xx[64];
  unsigned *X=(unsigned *)xx;
  int l=len,i,j;
  unsigned A=0x67452301,B=0xefcdab89,C=0x98badcfe,D=0x10325476,AA,BB,CC,DD;
  if(len>16384-64) { h[0]=0; return; }
  memcpy(M,b,len);
  for(M[len++]=128;(len&63)!=56;) M[len++]=0;
  M[len]=(l*8)&255; M[len+1]=((l*8)>>8)&255; M[len+2]=((l*8)>>16)&255; M[len+3]=(l*8)>>24;
  M[len+4]=M[len+5]=M[len+6]=M[len+7]=0;
  len+=8;
  for(i=0;i<len;i+=64) {
    for(j=0;j<64;j++) xx[j]=M[i+j];
    AA=A,BB=B,CC=C,DD=D;
    R1(A,B,C,D, 0,7,T[ 0]); R1(D,A,B,C, 1,12,T[ 1]); R1(C,D,A,B, 2,17,T[ 2]); R1(B,C,D,A, 3,22,T[ 3]);
    R1(A,B,C,D, 4,7,T[ 4]); R1(D,A,B,C, 5,12,T[ 5]); R1(C,D,A,B, 6,17,T[ 6]); R1(B,C,D,A, 7,22,T[ 7]);
    R1(A,B,C,D, 8,7,T[ 8]); R1(D,A,B,C, 9,12,T[ 9]); R1(C,D,A,B,10,17,T[10]); R1(B,C,D,A,11,22,T[11]);
    R1(A,B,C,D,12,7,T[12]); R1(D,A,B,C,13,12,T[13]); R1(C,D,A,B,14,17,T[14]); R1(B,C,D,A,15,22,T[15]);
    R2(A,B,C,D, 1,5,T[16]); R2(D,A,B,C, 6, 9,T[17]); R2(C,D,A,B,11,14,T[18]); R2(B,C,D,A, 0,20,T[19]);
    R2(A,B,C,D, 5,5,T[20]); R2(D,A,B,C,10, 9,T[21]); R2(C,D,A,B,15,14,T[22]); R2(B,C,D,A, 4,20,T[23]);
    R2(A,B,C,D, 9,5,T[24]); R2(D,A,B,C,14, 9,T[25]); R2(C,D,A,B, 3,14,T[26]); R2(B,C,D,A, 8,20,T[27]);
    R2(A,B,C,D,13,5,T[28]); R2(D,A,B,C, 2, 9,T[29]); R2(C,D,A,B, 7,14,T[30]); R2(B,C,D,A,12,20,T[31]);
    R3(A,B,C,D, 5,4,T[32]); R3(D,A,B,C, 8,11,T[33]); R3(C,D,A,B,11,16,T[34]); R3(B,C,D,A,14,23,T[35]);
    R3(A,B,C,D, 1,4,T[36]); R3(D,A,B,C, 4,11,T[37]); R3(C,D,A,B, 7,16,T[38]); R3(B,C,D,A,10,23,T[39]);
    R3(A,B,C,D,13,4,T[40]); R3(D,A,B,C, 0,11,T[41]); R3(C,D,A,B, 3,16,T[42]); R3(B,C,D,A, 6,23,T[43]);
    R3(A,B,C,D, 9,4,T[44]); R3(D,A,B,C,12,11,T[45]); R3(C,D,A,B,15,16,T[46]); R3(B,C,D,A, 2,23,T[47]);
    R4(A,B,C,D, 0,6,T[48]); R4(D,A,B,C, 7,10,T[49]); R4(C,D,A,B,14,15,T[50]); R4(B,C,D,A, 5,21,T[51]);
    R4(A,B,C,D,12,6,T[52]); R4(D,A,B,C, 3,10,T[53]); R4(C,D,A,B,10,15,T[54]); R4(B,C,D,A, 1,21,T[55]);
    R4(A,B,C,D, 8,6,T[56]); R4(D,A,B,C,15,10,T[57]); R4(C,D,A,B, 6,15,T[58]); R4(B,C,D,A,13,21,T[59]);
    R4(A,B,C,D, 4,6,T[60]); R4(D,A,B,C,11,10,T[61]); R4(C,D,A,B, 2,15,T[62]); R4(B,C,D,A, 9,21,T[63]);
    A+=AA,B+=BB,C+=CC,D+=DD;
  }
  h[0]=A&255; h[1]=(A>>8)&255; h[2]=(A>>16)&255; h[3]=A>>24;
  h[4]=B&255; h[5]=(B>>8)&255; h[6]=(B>>16)&255; h[7]=B>>24;
  h[8]=C&255; h[9]=(C>>8)&255; h[10]=(C>>16)&255; h[11]=C>>24;
  h[12]=D&255; h[13]=(D>>8)&255; h[14]=(D>>16)&255; h[15]=D>>24;
}
char s[1024];
int main() {
  int i;
  uchar y[32];
  md5init();
	scanf("%1023s",s);
  md5((uchar *)s,1,y);
  for(i=0;i<16;i++) printf("%02x",y[i]);
  printf("\n");
  return 0;
}
