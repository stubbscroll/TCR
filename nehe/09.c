#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BPP 16
SDL_Surface *surface;
int videoflags;
int fullscreen;
int keys[512];                              /* needs to be this large */

int twinkle;                                /* we have twinkle? */
#define NUM 50                              /* number of stars */
typedef struct { int r,g,b; GLfloat dist,angle; } s_star; /* star colour, distance from center, angle */
s_star star[NUM];                           /* many stars! */

GLfloat zoom;                               /* viewing distance from stars */
GLfloat tilt;                               /* tilt the view */
GLfloat spin;                               /* spin twinkling stars */
GLuint loop;
GLuint texture[1];

void error(char *s,...) {
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
  /* create nearest filtered texture */
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,0,3,pic->w,pic->h,0,GL_BGR,GL_UNSIGNED_BYTE,pic->pixels);
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
  if(!loadgltexture("09star.bmp",&texture[0])) return 0;
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);         /* blending function for translucency based on source alpha value */
  glEnable(GL_BLEND);
  return 1;
}

void ourinit() {
  int i;
  memset(keys,0,sizeof(keys));
  zoom=-15;
  tilt=90;
  spin=0;
  for(i=0;i<NUM;i++) {
    star[i].angle=0;
    star[i].dist=5.0*i/NUM;
    star[i].r=rand()%255;                   /* star gets random colour */
    star[i].g=rand()%255;
    star[i].b=rand()%255;
  }
}

int drawglscene() {
  int i;
  /* remember: positive x is right, positive y is up, positive z is out of the screen */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /* clear screen and depth buffer */
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  for(i=0;i<NUM;i++) {
    glLoadIdentity();
    glTranslatef(0,0,zoom);                 /* move into screen */
    glRotatef(tilt,1,0,0);                  /* tilt the view! */
    glRotatef(star[i].angle,0,1,0);         /* rotate to star's angle */
    glTranslatef(star[i].dist,0,0);         /* move to star position */
    glRotatef(-star[i].angle,0,1,0);        /* cancel rotation */
    glRotatef(-tilt,1,0,0);                 /* cancel tilt */
    if(twinkle) {
      glColor4ub(star[NUM-i-1].r,star[NUM-i-1].g,star[NUM-i-1].b,255);  /* set colour */
      glBegin(GL_QUADS);                    /* draw texture */
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
      glEnd();
    }
    glRotatef(spin,0,0,1);                  /* rotate star on x-axis */
    glColor4ub(star[i].r,star[i].g,star[i].b,255);  /* set colour */
    glBegin(GL_QUADS);                      /* draw texture */
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();
    spin+=0.01;
    star[i].angle+=(float)i/NUM;
    star[i].dist-=0.01;
    if(star[i].dist<0) {
      star[i].dist+=5;      /* star hit center, make a new star far away *
      star[i].r=rand()&255; /* new colour */
      star[i].g=rand()&255;
      star[i].b=rand()&255;
    }
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
  case SDLK_t:                    /* toggle twinkle */
    twinkle^=1;
    break;
  }
}

void heldkeys() {
  if(keys[SDLK_PAGEUP]) zoom-=0.2;    /* move into screen */
  if(keys[SDLK_PAGEDOWN]) zoom+=0.2;  /* move out of screen */
  if(keys[SDLK_UP]) tilt-=0.5;     /* change tilt */
  if(keys[SDLK_DOWN]) tilt+=0.5;
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
    if(active) drawglscene();
  }
theend:
  killglwindow();
  return 0;
}
