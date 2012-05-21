#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BPP 16
SDL_Surface *surface;
int videoflags;
int fullscreen;

#define X 45
#define Y 45
float points[X][Y][3];
float d2r;
int wigglecount;

GLfloat xrot,yrot,zrot;                     /* rotations around each axis */
GLuint texture[1];                          /* texture storage */

void error(char *s,...) {
  /* TODO quit graphics mode and not exit the code */
  SDL_Quit();
  va_list argptr;
  va_start(argptr,s);
  vfprintf(stderr,s,argptr);
  va_end(argptr);
  exit(1);
}

SDL_Surface *loadbmp(char *s) {
  SDL_Surface *image=SDL_LoadBMP(s);
  if (!image) error("Couldn't load %s: %s\n",s,SDL_GetError());
  return image;
}

int loadgltexture(char *s,int *texture) {
  SDL_Surface *pic=loadbmp(s);              /* load our texture */
  glGenTextures(1,texture);                 /* create the texture */
  glBindTexture(GL_TEXTURE_2D,*texture);    /* bind it */
  glTexImage2D(GL_TEXTURE_2D,0,3,pic->w,pic->h,0,GL_BGR,GL_UNSIGNED_BYTE,pic->pixels);  /* generate the texture */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); /* linear filtering if texture is shown smaller than its actual size */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); /* linear filtering if texture is shown larger than its actual size */
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
  glClearColor(0,0,0,0);                    /* black red background: r,g,b,alpha */
  glClearDepth(1);                          /* depth buffer setup */
  glEnable(GL_DEPTH_TEST);                  /* enable depth testing */
  glDepthFunc(GL_LEQUAL)                    /* type of depth testing */;
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); /* really nice perspective calculations */
  /* unfortunately, need to do this for each resize */
  if(!loadgltexture("11tim.bmp",&texture[0])) return 0;
  glPolygonMode(GL_BACK,GL_FILL);           /* back of polygon is filled */
  glPolygonMode(GL_FRONT,GL_LINE);          /* front of polygon has lines */
  return 1;
}

void ourinit() {
  int x,y;
  float modx=0.2*45/X,mody=0.2*45/Y;
  d2r=2*acos(0)/180;
  xrot=yrot=zrot=0;
  wigglecount=0;
  for(x=0;x<X;x++) for(y=0;y<Y;y++) {
    points[x][y][0]=x*modx-4.5;
    points[x][y][1]=y*mody-4.5;
    points[x][y][2]=sin(x*8*d2r);
  }
}

int drawglscene() {
  int x,y;
  float x1,x2,y1,y2,hold;
  /* remember: positive x is right, positive y is up, positive z is out of the screen */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /* clear screen and depth buffer */
  glLoadIdentity();                         /* reset modelview matrix */
  glTranslatef(0,0,-12);                     /* translate: move into 12.0 */
  glRotatef(xrot,1,0,0);                    /* rotate each axis */
  glRotatef(yrot,0,1,0);
  glRotatef(zrot,0,0,1);
  glBindTexture(GL_TEXTURE_2D,texture[0]);  /* select texture, remember to do it before glBegin() */
  glBegin(GL_QUADS);
  for(x=0;x<X-1;x++) for(y=0;y<Y-1;y++) {
    x1=x/(X-1.); y1=y/(Y-1.);
    x2=(x+1)/(X-1.); y2=(y+1)/(Y-1.);
    glTexCoord2f(x1,y1);
    glVertex3f(points[x][y][0],points[x][y][1],points[x][y][2]);
    glTexCoord2f(x1,y2);
    glVertex3f(points[x][y+1][0],points[x][y+1][1],points[x][y+1][2]);
    glTexCoord2f(x2,y2);
    glVertex3f(points[x+1][y+1][0],points[x+1][y+1][1],points[x+1][y+1][2]);
    glTexCoord2f(x2,y1);
    glVertex3f(points[x+1][y][0],points[x+1][y][1],points[x+1][y][2]);
  }
  glEnd();
  if(wigglecount==2) {
    for(y=0;y<Y;y++) {
      hold=points[0][y][2];
      for(x=0;x<X-1;x++) points[x][y][2]=points[x+1][y][2];
      points[X-1][y][2]=hold;
    }
    wigglecount=0;
  }
  wigglecount++;
  xrot+=0.3;
  yrot+=0.2;
  zrot+=0.4;
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
  }
}

int main (int argc, char* argv[]) {
  SDL_Event event;
  int active=1;
  fullscreen=0;
  sdlglinit(640,480,BPP,fullscreen);
  ourinit();
  while(1) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_VIDEORESIZE:
        surface=SDL_SetVideoMode(event.resize.w,event.resize.h,BPP,videoflags);
        if(!surface) error("Couldn't get surface after resize: %s\n",SDL_GetError());
        initgl(); /* needed before resize to avoid every setting being reset, like
                    background colour, depth settings etc. the absence of this line causes
                    wrong depth stuff. */
        resizeglscene(event.resize.w,event.resize.h);
        break;
      case SDL_KEYDOWN:
        handlekeypress(&event.key.keysym);
        break;
      case SDL_QUIT:
        goto theend;
      }
    }
    if(active) drawglscene(),SDL_Delay(1000.0/70);
  }
theend:
  killglwindow();
  return 0;
}
