/* simpler example, just render text normally and print ascii art to stdout */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define SIZEX 200
#define SIZEY 40

void error(char *s) {
	puts(s);
	exit(1);
}

char s[]="Test, hfjgdjk.";

char g[SIZEY][SIZEX+1];

void drawbitmap(FT_Bitmap *bitmap,int x,int y,int left,int top) {
	int i,j;
	int atx=x+left;
	int aty=SIZEY-10-bitmap->rows;
	/* adjust for glyph going below baseline */
	aty+=bitmap->rows-top;
	for(j=0;j<bitmap->rows;j++) for(i=0;i<bitmap->width;i++) {
		if(i+atx<0 || i+atx>=SIZEX || j+aty<0 || j+aty>=SIZEY) continue;
		if(bitmap->buffer[i+j*bitmap->width]) g[aty+j][atx+i]='#';
	}
}

int main() {
	FT_Library library; /* the library */
	FT_Face face;       /* the font */
	int i,j,x,y;
	if(FT_Init_FreeType(&library)) error("init failed");
	if(FT_New_Face(library,"c:/windows/fonts/times.ttf",0,&face)) error("load font failed");
	/* set char height 20 (given as 1/64ths) with 100 dpi */
	if(FT_Set_Char_Size(face,0,20*64,100,100)) error("set char size failed");
	for(i=0;i<SIZEY;i++) for(j=0;j<=SIZEX;j++) g[i][j]=(j==SIZEX)?0:32;
	x=y=0;
	for(i=0;s[i];i++) {
		if(FT_Load_Char(face,s[i],FT_LOAD_RENDER)) continue;
		drawbitmap(&face->glyph->bitmap,x,y,face->glyph->bitmap_left,face->glyph->bitmap_top);
		x+=face->glyph->advance.x/64; /* advance is 64ths, divide to get pixel distance */
	}
	for(i=0;i<SIZEY;i++) puts(g[i]);
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	return 0;
}
