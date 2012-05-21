#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BPP 16
#define MAX 256
SDL_Surface *surface;
int videoflags;
int fullscreen;
int keys[512];                                    /* needs to be this large */

char s[MAX];

typedef struct { float x,y,z,u,v; } s_vertex;     /* vertex: 3d coordinates and texture coordinates */
typedef struct { s_vertex vertex[3]; } s_triangle;/* triangle: 3 vertices */

int numtriangles;
s_triangle  *sector;
float yrot,xpos,zpos,walkbias,walkbiasangle,lookupdown;
float d2r,pi;
int filter;
GLuint texture[3];

void error(char *s,...) {
  SDL_Quit();
  va_list argptr;
  va_start(argptr,s);
  vfprintf(stderr,s,argptr);
  va_end(argptr);
  exit(1);
}

void readstr(FILE *f,char *s,int maks) {
  do fgets(s,maks,f); while(s[0]=='/' || s[0]=='\n' || s[0]=='\r');
}

void setupworld(char *filename) {
  float x,y,z,u,v;
  int i,j;
  FILE *f=fopen(filename,"r");
  if(!f) error("Couldn't open file %s.\n",filename);
  readstr(f,s,MAX);
  sscanf(s,"NUMPOLLIES %d",&numtriangles);
  sector=(s_triangle *)malloc(numtriangles*sizeof(s_triangle));
  for(i=0;i<numtriangles;i++) for(j=0;j<3;j++) {
    readstr(f,s,MAX);
    sscanf(s,"%f %f %f %f %f",&sector[i].vertex[j].x,&sector[i].vertex[j].y,&sector[i].vertex[j].z,&sector[i].vertex[j].u,&sector[i].vertex[j].v);
  }
  fclose(f);
}

SDL_Surface *loadbmp(char *s) {
  SDL_Surface *image=SDL_LoadBMP(s);
  if (!image) error("Couldn't load %s: %s\n",s,SDL_GetError());
  return image;
}

int loadgltexture(char *s,int *texture) {
  SDL_Surface *pic=loadbmp(s);              /* load our texture */
  glGenTextures(3,texture);                 /* create the texture */
  /* create nearest filtered texture */
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,3,pic->w,pic->h,0,GL_BGR,GL_UNSIGNED_BYTE,pic->pixels);
  /* create linear filtered texture */
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,0,3,pic->w,pic->h,0,GL_BGR,GL_UNSIGNED_BYTE,pic->pixels);
  /* create mipmapped texture */
  glBindTexture(GL_TEXTURE_2D,texture[2]);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST);
  gluBuild2DMipmaps(GL_TEXTURE_2D,3,pic->w,pic->h,GL_BGR,GL_UNSIGNED_BYTE,pic->pixels); /* build mipmap data! */
  SDL_FreeSurface(pic);                     /* throw texture */
  return 1;
}
void resizeglscene(int w,int h) {
  if(!h) h=1;                               /* prevent division by zero */
  glViewport(0,0,w,h);                      /* resize viewport */
  glMatrixMode(GL_PROJECTION);              /* select projection matrix */
  glLoadIdentity();                         /* reset it */
  gluPerspective(45,(GLfloat)w/h,0.1,100);  /* set fov angle, aspect ratio, near clipping plane (z), far clipping plane (z) */
  glMatrixMode(GL_MODELVIEW);               /* select modelview matrix */
  glLoadIdentity();                         /* reset it */
}

/* stuff to be initialized for change between fullscreen/windowed */
int initgl() {
  glEnable(GL_TEXTURE_2D);                  /* enable texture mapping */
  glShadeModel(GL_SMOOTH);                  /* select shade model: smooth */
  glClearColor(0,0,0,0.5);                  /* black background: r,g,b,alpha */
  glClearDepth(1);                          /* depth buffer setup */
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); /* really nice perspective calculations */
  if(!loadgltexture("10mud.bmp",&texture[0])) return 0;
  glEnable(GL_DEPTH_TEST);                  /* enable depth testing */
  glDepthFunc(GL_LEQUAL)                    /* type of depth testing */;
  return 1;
}

void ourinit() {
  memset(keys,0,sizeof(keys));
  setupworld("10world.txt");
  pi=2*acos(0);
  d2r=pi/180;
  xpos=zpos=walkbias=yrot=walkbiasangle=lookupdown=0;
}

void ourcleanup() {
  free(sector);
}

int drawglscene() {
  int i,j;
  /* remember: positive x is right, positive y is up, positive z is out of the screen */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /* clear screen and depth buffer */
  glLoadIdentity();
  glRotatef(lookupdown,1,0,0);
  glRotatef(360-yrot,0,1,0);
  glTranslatef(-xpos,-walkbias-0.25,-zpos);
  glBindTexture(GL_TEXTURE_2D,texture[filter]);
  for(i=0;i<numtriangles;i++) {
    glBegin(GL_TRIANGLES);
      glNormal3f(0,0,1);                              /* um, is this really correct? */
      for(j=0;j<3;j++) {
        glTexCoord2f(sector[i].vertex[j].u,sector[i].vertex[j].v);
        glVertex3f(sector[i].vertex[j].x,sector[i].vertex[j].y,sector[i].vertex[j].z);
      }
    glEnd();
  }
  SDL_GL_SwapBuffers();                     /* show our changes! needed when double buffering is on */
  return 1;
}

void killglwindow() {
  SDL_Quit();
  exit(0);
}

int sdlglinit(int w,int h,int bpp,int fullscreen) {
  const SDL_VideoInfo *info;
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0) error("Video initialization failed: %s\n",SDL_GetError());
  info=SDL_GetVideoInfo();
  if(!info) error("Video query failed: %s\n",SDL_GetError());
  videoflags=SDL_OPENGL                               /* enable opengl in sdl */
    |SDL_GL_DOUBLEBUFFER                              /* enable double buffering */
    |SDL_HWPALETTE                                    /* store the palette in hardware */
    |SDL_RESIZABLE;                                   /* enable windows resizing */
  if(fullscreen) videoflags|=SDL_FULLSCREEN;
  if(info->hw_available) videoflags|=SDL_HWSURFACE;   /* check if we can store surfaces in memory */
  else videoflags|=SDL_SWSURFACE;
  if(info->blit_hw) videoflags|=SDL_HWACCEL;          /* check if hardware blits can be done */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);         /* set up double buffering */
  surface=SDL_SetVideoMode(w,h,bpp,videoflags);
  if(!surface) error("Video mode set failed: %s\n",SDL_GetError());
  if(!initgl()) error("Couldn't initialize opengl.\n");
  resizeglscene(w,h);
  return 1;
}

void handlekeypress(SDL_keysym *keysym) {
  switch(keysym->sym) {
  case SDLK_ESCAPE:
    killglwindow();
  case SDLK_F1:
    fullscreen^=1;
    sdlglinit(640,480,BPP,fullscreen);
    break;
  case SDLK_f:                    /* toggle through filters */
    filter=(filter+1)%3;
    break;
  }
}

void heldkeys() {
  if(keys[SDLK_LEFT]) yrot+=1.5;      /* rotate */
  if(keys[SDLK_RIGHT]) yrot-=1.5;
  if(keys[SDLK_UP]) {
    xpos-=(float)sin(yrot*d2r)*0.05;
    zpos-=(float)cos(yrot*d2r)*0.05;
    walkbiasangle+=10;
    if(walkbiasangle>=360) walkbiasangle-=360;
    walkbias=(float)sin(walkbiasangle*d2r)/20;
  }
  if(keys[SDLK_DOWN]) {
    xpos+=(float)sin(yrot*d2r)*0.05;
    zpos+=(float)cos(yrot*d2r)*0.05;
    walkbiasangle-=10;
    if(walkbiasangle<0) walkbiasangle+=360;
    walkbias=(float)sin(walkbiasangle*d2r)/20;
  }
  if(keys[SDLK_PAGEDOWN]) {
    lookupdown+=0.4;
    if(lookupdown>90) lookupdown=90;
  }
  if(keys[SDLK_PAGEUP]) {
    lookupdown-=0.4;
    if(lookupdown<-90) lookupdown=-90;
  }
  if(keys[SDLK_UP]+keys[SDLK_DOWN]<1) walkbiasangle=0,walkbias=0;
}

int main (int argc, char* argv[]) {
  SDL_Event event;
  int active=1;
  fullscreen=0;
  ourinit();
  sdlglinit(640,480,BPP,fullscreen);
  while(1) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_VIDEORESIZE:
        surface=SDL_SetVideoMode(event.resize.w,event.resize.h,BPP,videoflags);
        if(!surface) error("Couldn't get surface after resize: %s\n",SDL_GetError());
        initgl(); /* after setting video mode, we lost every gl setting. set again. */
        resizeglscene(event.resize.w,event.resize.h);
        break;
      case SDL_KEYDOWN:
        keys[event.key.keysym.sym]=1;
        handlekeypress(&event.key.keysym);
        break;
      case SDL_KEYUP:
        keys[event.key.keysym.sym]=0;
        break;
      case SDL_QUIT:
        goto theend;
      }
    }
    /* check for held keys */
    heldkeys();
    if(active) drawglscene(),SDL_Delay(1000.0/70);
  }
theend:
  ourcleanup();
  killglwindow();
  return 0;
}
