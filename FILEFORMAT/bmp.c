#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* load and save BMP files with standard library only */
/* loading supports 8-bits RLE and 24-bits uncompressed */

typedef unsigned char uchar;
typedef unsigned int uint;

/* read stuff in little endian */
uint getword4(uchar *a) {
	return *a+(a[1]<<8)+(a[2]<<16)+(a[3]<<24);
}
int getword2(uchar *a) {
	return *a+(a[1]<<8);
}
void setword4(uchar *a,uint v) {
	a[0]=v&255; a[1]=(v>>8)&255; a[2]=(v>>16)&255; a[3]=(v>>24)&255;
}

/* load a bmp image. return image in 3 arrays R,G,B (one
   flat array for each colour component), 
   return size in x, y. return 0 if failed */
int loadimage(char *s,int *_x,int *_y,uchar **_R,uchar **_G,uchar **_B) {
	uchar *b,c,*R,*G,*B,c1,c2,c3;
	int len,i,j,offs,comp,bpp,from,atx,aty,x,y,v;
	FILE *f=fopen(s,"rb");
	if(!f) return 0;
	/* get file length */
	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);
	/* read entire file into b */
	if(!(b=malloc(len))) { fclose(f); return 0; }
	fread(b,len,1,f);
	if(fclose(f)) { free(b); return 0; }
	/* check BMP signature */
	if(b[0]!=0x42 || b[1]!=0x4d) { free(b); return 0; }
	offs=getword4(b+0xa);
	x=*_x=getword4(b+0x12);
	y=*_y=getword4(b+0x16);
	if(!(R=malloc(x*y))) { free(b); return 0; }
	if(!(G=malloc(x*y))) { free(R); free(b); return 0; }
	if(!(B=malloc(x*y))) { free(G); free(R); free(b); return 0; }
	*_R=R; *_G=G; *_B=B;
	/* get compression type and bit depth */
	comp=getword4(b+0x1e);
	bpp=getword2(b+0x1c);
	if(bpp==8 && !comp) {
		/* 8-bits RLE uncompressed: get palette from 0x36 */
		from=offs;
		for(j=y-1;j>=0;j--) {
			for(i=0;i<x;i++) {
				v=b[from+i];
				c1=b[0x36+v*4]; c2=b[0x37+v*4]; c3=b[0x38+v*4];
				R[i+j*x]=c3;
				G[i+j*x]=c2;
				B[i+j*x]=c1;
			}
			from+=(31+bpp*x)/32*4;
		}
	} else if(bpp==8 && comp==1) {
		/* 8-bits RLE compressed: get palette from 0x36 */
		atx=0; aty=y-1;
		from=offs;
		while(1) {
			c=b[from++];
			if(c) {
				v=b[from++];
				c1=b[0x36+v*4]; c2=b[0x37+v*4]; c3=b[0x38+v*4];
				while(c--) if(atx<x) {
					R[atx+aty*x]=c1;
					G[atx+aty*x]=c2;
					B[atx+++aty*x]=c3;
				} else atx++;
			} else {
				c=b[from++];
				switch(c) {
				case 0: atx=0; aty--; break;
				case 1: goto done;
				case 2: atx+=b[from++]; aty-=b[from++]; break;
				default:
					for(i=0;i<c;i++,from++) if(atx<x) {
						v=b[from];
						c1=b[0x36+v*4]; c2=b[0x37+v*4]; c3=b[0x38+v*4];
						R[atx+aty*x]=c1;
						G[atx+aty*x]=c2;
						B[atx+++aty*x]=c3;
					} else atx++;
				}
			}
		}
	} else if(bpp==24 && !comp) {
		/* 24-bits uncompressed */
		from=offs;
		for(j=y-1;j>=0;j--) {
			for(i=0;i<x;i++) {
				B[i+j*x]=b[from+3*i];
				G[i+j*x]=b[from+3*i+1];
				R[i+j*x]=b[from+3*i+2];
			}
			from+=(31+bpp*x)/32*4;
		}
	} else { free(b); return 0; }
done:
	free(b);
	return 1;
}

/* save a gray scale image (given as x*y floats) as bmp,
   return 1/0 upon success/failure, saves as 24-bit uncompressed */
int saveimage(char *s,int x,int y,uchar *R,uchar *G,uchar *B) {
	int w,i,j;
	unsigned char header[54]={
		0x42,0x4d,0,0,0,0,0,0,0,0,54,0,0,0,40,0,
		0,0,0,0,0,0,0,0,0,0,1,0,24,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0
	};
	unsigned char *b;
	FILE *f=fopen(s,"wb");
	if(!f) return 0;
	w=x*3;
	if(w&3) w+=4-(w&3);
	if(!(b=calloc(w*y,1))) { fclose(f); return 0; }
	setword4(header+2,54+w*y);
	setword4(header+0x12,x);
	setword4(header+0x16,y);
	setword4(header+0x22,w*y);
	for(j=0;j<y;j++) for(i=0;i<x;i++) {
		b[i*3+(y-j-1)*w]=B[i+j*x];
		b[i*3+(y-j-1)*w+1]=G[i+j*x];
		b[i*3+(y-j-1)*w+2]=R[i+j*x];
	}
	fwrite(header,54,1,f);
	fwrite(b,w*y,1,f);
	fclose(f);
	return 1;
}

int main(int argc,char **argv) {
	int x,y;
	uchar *r,*g,*b;
	if(argc<3) { puts("usage: bmp in.bmp out.bmp"); return 0; }
	if(loadimage(argv[1],&x,&y,&r,&g,&b)) saveimage(argv[2],x,y,r,g,b);
	return 0;
}
