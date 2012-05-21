#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BPP 16
SDL_Surface *surface;
int videoflags;
int fullscreen;

GLuint texture[2];                          /* texture storage */
GLuint base;                                /* base display list for font */
GLfloat cnt1,cnt2;                          /* counter for effects */

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
  /*  apparently, it is not good to call glGenTextures() many times in the program.
      it is a chance that previously generated textures get overwritten. having said that,
      we got away with that in lesson 6, */
  glGenTextures(1,texture);                 /* create the texture */
  glBindTexture(GL_TEXTURE_2D,*texture);    /* bind it */
  glTexImage2D(GL_TEXTURE_2D,0,3,pic->w,pic->h,0,GL_BGR,GL_UNSIGNED_BYTE,pic->pixels);  /* generate the texture */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); /* linear filtering if texture is shown smaller than its actual size */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); /* linear filtering if texture is shown larger than its actual size */
  SDL_FreeSurface(pic);                     /* throw texture */
  return 1;
}

/* build font from the two textures in texture[0..1] */
#define Z 0.0625
void buildfont() {
  int i;
  float cx,cy;
  base=glGenLists(256);
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  for(i=0;i<256;i++) {
    /* nb, sdl reads bmp files unconverted (rotated 180 degrees) */
    cx=1-i%16/16.0;
    cy=1-i/16/16.0;
    glNewList(base+255-i,GL_COMPILE);
      glBegin(GL_QUADS);
        glTexCoord2f(cx-Z,cy);              /* bottom left */
        glVertex2i(0,0);
        glTexCoord2f(cx,cy);            /* bottom right */
        glVertex2i(16,0);
        glTexCoord2f(cx,cy-Z);                /* top right */
        glVertex2i(16,16);
        glTexCoord2f(cx-Z,cy-Z);
        glVertex2i(0,16);
      glEnd();
      glTranslated(10,0,0);                   /* move to the right of the character */
    glEndList();
  }
}
#undef Z

char hemmelig[1024];
void glprintf(GLint x,GLint y,int set,char *s,...) {
  /* vsprintf magic */
  va_list args;
  va_start(args,s);
  vsprintf(hemmelig,s,args);
  va_end(args);
  /* rest of routine */
  set&=1;
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();                             /* store the old projection matrix */
  glLoadIdentity();
  glOrtho(0,640,0,480,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();                             /* store the old model view matrix */
  glLoadIdentity();
  glTranslated(x,y,0);
  glListBase(base-32+(128*set));
  glCallLists(strlen(hemmelig),GL_UNSIGNED_BYTE,hemmelig);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();                              /* restore matrices */
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
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
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  /* unfortunately, need to do this for each resize */
  if(!loadgltexture("17font.bmp",&texture[0])) return 0;
  if(!loadgltexture("17bumps.bmp",&texture[1])) return 0;
  buildfont();
  return 1;
}

void ourinit() {
  cnt1=cnt2=0;
}

int drawglscene() {
  /* remember: positive x is right, positive y is up, positive z is out of the screen */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /* clear screen and depth buffer */
  glLoadIdentity();                         /* reset modelview matrix */
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glTranslatef(0,0,-5);                     /* translate: move into 5.0 */
  glRotatef(45,0,0,1);
  glRotatef(cnt1*30,1,1,0);
  glDisable(GL_BLEND);                      /* disable blending */
  glColor3f(1,1,1);                         /* display solid white thing */
  glBegin(GL_QUADS);
    glTexCoord2d(0,0);                      /* hmm, not sure if texcoord is needed */
    glVertex2f(-1,1);
    glTexCoord2d(1,0);
    glVertex2f(1,1);
    glTexCoord2f(1,1);
    glVertex2f(1,-1);
    glTexCoord2d(0,1);
    glVertex2f(-1,-1);
  glEnd();
  glRotatef(90,1,1,0);
  glBegin(GL_QUADS);                        /* draw a new slightly rotated white thing on top */
    glTexCoord2d(0,0);
    glVertex2f(-1,1);
    glTexCoord2d(1,0);
    glVertex2f(1,1);
    glTexCoord2d(1,1);
    glVertex2f(1,-1);
    glTexCoord2d(0,1);
    glVertex2f(-1,-1);
  glEnd();
  glEnable(GL_BLEND);
  glLoadIdentity();
  /* pulsing colours based on text position */
  glColor3f(cos(cnt1),sin(cnt2),1-0.5*cos(cnt1+cnt2));
  glprintf((int)(280+250*cos(cnt1)),(int)(235+200*sin(cnt2)),9,"Trallala");
  glColor3f(sin(cnt2),1-.5*cos(cnt1+cnt2),cos(cnt1));
  glprintf((int)(280+230*cos(cnt2)),(int)(235+200*sin(cnt1)),1,"OpenGL %d",50);
  glColor3f(0,0,1);
  glprintf((int)(240+200*cos((cnt2+cnt1)/5)),2,0,"Ruben %.2f",cnt1);
  glColor3f(1,1,1);
  glprintf((int)(242+200*cos((cnt2+cnt1)/5)),2,0,"Ruben %.2f",cnt1);
  cnt1+=0.01;
  cnt2+=0.0081;
  SDL_GL_SwapBuffers();                     /* show our changes! needed when double buffering is on */
  return 1;
}

void killglwindow() {
  glDeleteLists(base,256);                  /* nice people clean up */
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
    if(active) drawglscene(),SDL_Delay(1000/70.0);
  }
theend:
  killglwindow();
  return 0;
}
