/*  masking without alpha channel!

    masking is achieved in the following way:
    - have two images, the image to display and its mask
    - the mask has white where the image should be transparent,
      and black where the image should not be
    - also, the image should be black where it is transparent
    - set the blending function to GL_DST_COLOR, GL_ZERO, which causes only black to be drawn,
      (white gets "multiplied" with the existing colour in the frame buffer, i think)
    - draw the mask, which results in black areas drawn where the mask contains black.
    - set the blending function to GL_ONE,GL_ONE, which just adds the r,g,b values together
      (i think)
    - draw the image, which is straightforward. there are no pixels where both the frame buffer
      and the texture are both non-black.

    - key functions: glBlendFunc() - defines the blending operations (pixel arithmetic).
*/
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BPP 16
SDL_Surface *surface;
int videoflags;
int fullscreen;
int keys[512];  /* needs to be this large */

int masking;
int scene;

GLuint texture[5];                          /* texture storage */
GLfloat roll;

void error(char *s,...) {
  SDL_Quit();
  va_list argptr;
  va_start(argptr,s);
  vfprintf(stderr,s,argptr);
  va_end(argptr);
  exit(1);
}

SDL_Surface *loadbmp(char *s) {
  int i;
  char p;
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
  glClearColor(0,0,0,0);                    /* black red background: r,g,b,alpha */
  glClearDepth(1);                          /* depth buffer setup */
  /* unfortunately, need to do this for each resize */
  if(!loadgltexture("20logo.bmp",&texture[0])) return 0;
  if(!loadgltexture("20mask1.bmp",&texture[1])) return 0;
  if(!loadgltexture("20image1.bmp",&texture[2])) return 0;
  if(!loadgltexture("20mask2.bmp",&texture[3])) return 0;
  if(!loadgltexture("20image2.bmp",&texture[4])) return 0;
  return 1;
}

void ourinit() {
  masking=1;
  memset(keys,0,sizeof(keys));
}

int drawglscene() {
  /* remember: positive x is right, positive y is up, positive z is out of the screen */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /* clear screen and depth buffer */
  glLoadIdentity();                         /* reset modelview matrix */
  glTranslatef(0,0,-2);
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_QUADS);
    glTexCoord2f(0,3-roll); glVertex3f(-1.1,-1.1,0);
    glTexCoord2f(3,-roll+3); glVertex3f(1.1,-1.1,0);
    glTexCoord2f(3,-roll); glVertex3f(1.1,1.1,0);
    glTexCoord2f(0,-roll); glVertex3f(-1.1,1.1,0);
  glEnd();
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  if(masking) glBlendFunc(GL_DST_COLOR,GL_ZERO);  /* blend screen with black colour (zero) */
  if(scene) {
    glTranslatef(0,0,-1);
    glRotatef(roll*360,0,0,1);
    if(masking) {
      glBindTexture(GL_TEXTURE_2D,texture[3]);
      glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.1f, -1.1f,  0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.1f, -1.1f,  0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.1f,  1.1f,  0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.1f,  1.1f,  0.0f);
      glEnd();
    }
    glBlendFunc(GL_ONE,GL_ONE);
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.1f, -1.1f,  0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.1f, -1.1f,  0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.1f,  1.1f,  0.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.1f,  1.1f,  0.0f);
    glEnd();
  } else {
    if(masking) {
      glBindTexture(GL_TEXTURE_2D,texture[1]);
      glBegin(GL_QUADS);
        glTexCoord2f(roll+0.0f, 0.0f); glVertex3f(-1.1f, -1.1f,  0.0f);
        glTexCoord2f(roll+4.0f, 0.0f); glVertex3f( 1.1f, -1.1f,  0.0f);
        glTexCoord2f(roll+4.0f, 4.0f); glVertex3f( 1.1f,  1.1f,  0.0f);
        glTexCoord2f(roll+0.0f, 4.0f); glVertex3f(-1.1f,  1.1f,  0.0f);
      glEnd();
    }
    glBlendFunc(GL_ONE,GL_ONE);
    glBindTexture(GL_TEXTURE_2D,texture[2]);
    glBegin(GL_QUADS);
      glTexCoord2f(roll+0.0f, 0.0f); glVertex3f(-1.1f, -1.1f,  0.0f);
      glTexCoord2f(roll+4.0f, 0.0f); glVertex3f( 1.1f, -1.1f,  0.0f);
      glTexCoord2f(roll+4.0f, 4.0f); glVertex3f( 1.1f,  1.1f,  0.0f);
      glTexCoord2f(roll+0.0f, 4.0f); glVertex3f(-1.1f,  1.1f,  0.0f);
    glEnd();
  }
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  roll+=0.002;
  if(roll>1) roll=-1;
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
  case SDLK_SPACE:                    /* change scene */
    scene^=1;
    break;
  case SDLK_m:                    /* toggle masking on/off */
    masking^=1;
    break;
  }
}

void heldkeys() {
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
    if(active) drawglscene(),SDL_Delay(1000/70.0);
  }
theend:
  killglwindow();
  return 0;
}
