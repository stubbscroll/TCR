#include <stdio.h>
#include <SDL/SDL.h>

/* try o3show lion.bmp out.bmp 1 */

/* compiler options:
   -wall -lmingw32 -llibsdl -lopengl32 -lglu32 -lgdi32 -lwinmm -lsdlmain -lsdl
*/

/* oppgave c:
   
   Forskjellen mellom de to teknikkene er at smooth-filteret tar
   gjennomsnitt av alle verdiene innen masken, mens medianfilteret
   tar medianverdien innen masken. Når det forekommer støy som fører
   til kraftig avvikende pixelverdier, vil dette påvirke smooth-
   filteret til å returnere avvikende verdier. Medianfilteret
   vil derimot ikke la seg påvirke veldig av slike ytterpunktverdier.
   Medianfilteret fungerer meget bra på såkalt salt- og pepperstøy.
   Smooth-filteret er lineært fordi det kan skrives på formen:
      \sum_{all i} p_i*s_i
   der p_i er pixelverdi og s_i er skalar, dvs output-verdien er
   en lineærkombinasjon av alle pixels i masken. Medianfilteret er
   ikke lineært fordi det ikke kan uttrykkes på ovennevnte form.
*/

typedef unsigned char uchar;
typedef struct { uchar b, g, r, a; } pixel;

void error(char *s,...) {
   va_list argptr;
   va_start(argptr,s);
   vfprintf(stderr,s,argptr);
   va_end(argptr);
   exit(1);
}

void LoadBMP(char *filename, SDL_Surface *surface) {
  /* Load BMP file */
  SDL_Surface* image=SDL_LoadBMP(filename);
  SDL_Rect area = {0, 0, image->w, image->h};
  if (!image) error("Couldn't load %s: %s\n", filename, SDL_GetError());  
  /* Copy loaded bitmap to surface */
  SDL_BlitSurface(image, NULL, surface, &area);
  SDL_FreeSurface(image);
}

pixel *from,*to;
int x,y;

uchar confine(int val) {
   if(val<0) return 0;
   else if(val>255) return 255;
   else return (uchar)val;
}

uchar maks(uchar r,uchar g,uchar b) {return r>=g&&r>=b?r:(g>=r&&g>=b?g:b);}
uchar avg(uchar r,uchar g,uchar b) {return (uchar)(((int)r+g+b)/3);}

void lek(pixel *a,pixel *b) {
   b->r=255-a->r;
   b->g=confine((int)a->g*2);
   b->b=confine((int)a->b-10);
}
void grey1(pixel *a,pixel *b) {b->r=b->g=b->b=maks(a->r,a->g,a->b);}
void grey2(pixel *a,pixel *b) {b->r=b->g=b->b=avg(a->r,a->g,a->b);}

void colour(pixel *from, pixel *to,void (*lek)(pixel *,pixel *)) {
   int i,j;
   for(i=0;i<x;i++) for(j=0;j<y;j++) (*lek)(&from[i+j*x],&to[i+j*x]);
}

double smooth[]={1,1,1,1,1,1,1,1,1};
double sharp[]={0,-1,0,-1,5,-1,0,-1,0};
double edgeh[]={-1,0,1,-1,0,1,-1,0,1};
double edgev[]={-1,-1,-1,0,0,0,1,1,1};
double random[9];

void normal(double *f,int n) {
   int i=0;
   double sum=0;
   for(i=0;i<n;i++) sum+=f[i];
   for(i=0;i<n;i++) f[i]/=sum;
}

void conv(pixel *from, pixel *to,int n,double *f) {
   int i,j,u,v,m=n/2,s,t;
   for(i=m;i<x-m;i++) for(j=m;j<y-m;j++) {
      double r=0,g=0,b=0;
      for(s=0,u=-m;u<=m;u++,s++) for(t=0,v=-m;v<=m;v++,t++) {
         r+=from[i+u+(j+v)*x].r*f[s+t*n];
         g+=from[i+u+(j+v)*x].g*f[s+t*n];
         b+=from[i+u+(j+v)*x].b*f[s+t*n];
      }
      to[i+j*x].r=confine((int)r);
      to[i+j*x].g=confine((int)g);
      to[i+j*x].b=confine((int)b);
   }
}

int compgr(const void *A,const void *B) {
   pixel *a=(pixel *)A; pixel *b=(pixel *)B;
   return (int)a->a-(int)b->a;
}

void median(pixel *from,pixel *to,int n) {
   int i,j,u,v,m=n/2,p,pos=n*n/2;
   pixel *sort=(pixel *)malloc(n*n*sizeof(pixel));
   for(i=m;i<x-m;i++) for(j=m;j<y-m;j++) {
      for(p=0,u=-m;u<=m;u++) for(v=-m;v<=m;v++,p++) {
         sort[p]=from[i+u+(j+v)*x];
         sort[p].a=avg(sort[p].r,sort[p].g,sort[p].b);
      }
      qsort(sort,n*n,sizeof(pixel),compgr);
      to[i+j*x].r=sort[pos].r;
      to[i+j*x].g=sort[pos].g;
      to[i+j*x].b=sort[pos].b;
   }
   free(sort);
}

void filter(SDL_Surface *w,int f) {
   x=w->w; y=w->h;
   to=(pixel *)w->pixels;
   from=(pixel *)malloc(x*y*sizeof(pixel));
   memcpy(from,to,sizeof(pixel)*x*y);
   if(f==1) colour(from,to,lek); /* lek */
   else if(f==2) colour(from,to,grey1); /* grå = høyest rgb */
   else if(f==3) colour(from,to,grey2); /* grå = rgb-gj.snitt */
   else if(f==4) conv(from,to,3,smooth); /* smooth */
   else if(f==5) conv(from,to,3,sharp); /* sharp */
   else if(f==6) conv(from,to,3,edgeh); /* horizontal edge */
   else if(f==7) conv(from,to,3,edgev); /* vertical edge */
   else if(f==8) median(from,to,3); /* median */
   else if(f==9) conv(from,to,3,random); /* random filter */
   free(from);
}

int main(int argc,char **argv) {
   SDL_Surface *window=SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
   SDL_Event event;
   int f=0,i;
   if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO)<0) error("unable to init SDL\n");
   normal(smooth,9);
   normal(sharp,9);
   for(i=0;i<9;i++) random[i]=((rand()%1024)/64.0-8)/16.0;
   if(argc>2) {
      LoadBMP(argv[1],window);
      for(i=3;i<argc;i++) {
         f=strtol(argv[i],0,10);
         filter(window,f);
      }
      if(strcmp(argv[2],".")) SDL_SaveBMP(window,argv[2]);
   } else { 
      SDL_Quit();
      fprintf(stderr,"usage: o3show load.bmp save.bmp [filter]\nwhere filter is a number from 1-8\n");
      exit(0);
   }
   SDL_UpdateRect(window,0,0,0,0);
   while(SDL_WaitEvent(&event)>=0) {
      if(event.type==SDL_QUIT) {
         SDL_Quit();
         break;
      }
   }
   return 0;
}
