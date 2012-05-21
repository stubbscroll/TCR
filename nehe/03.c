#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BPP 16
SDL_Surface *surface;
int videoflags;

void error(char *s,...) {
  va_list argptr;
  va_start(argptr,s);
  vfprintf(stderr,s,argptr);
  va_end(argptr);
  exit(1);
}

void resizeglscene(int w,int h) {
  if(!h) h=1;                               /* prevent division by zero */
  glViewport(0,0,w,h);                      /* resize viewport */
  glMatrixMode(GL_PROJECTION);              /* select projection matrix */
  glLoadIdentity();                         /* reset it */
  gluPerspective(45,(GLfloat)w/h,0.1,100);  /* calculate aspect ratio */
  glMatrixMode(GL_MODELVIEW);               /* select modelview matrix */
  glLoadIdentity();                         /* reset it */
}

int initgl() {
  glShadeModel(GL_SMOOTH);                  /* enable smooth shading */
  glClearColor(0,0,0,0);                    /* black red background: r,g,b,alpha */
  glClearDepth(1);                          /* depth buffer setup */
  glEnable(GL_DEPTH_TEST);                  /* enable depth testing */
  glDepthFunc(GL_LEQUAL)                    /* type of depth testing */;
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); /* really nice perspective calculations */
  return 1;
}

int drawglscene() {
  /* remember: positive x is right, positive y is up, positize z is out of the screen */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /* clear screen and depth buffer */
  glLoadIdentity();                         /* reset modelview matrix */
  glTranslatef(-1.5,0,-6);                  /* translate: move left -1.5 and into 6.0 */
  glBegin(GL_TRIANGLES);                    /* draw triangle */
    glColor3f(1,0,0);
    glVertex3f( 0, 1,0);                    /* top: red */
    glColor3f(0,1,0);
    glVertex3f(-1,-1,0);                    /* bottom left: green */
    glColor3f(0,0,1);
    glVertex3f( 1,-1,0);                    /* bottom right: blue */
  glEnd();
  glTranslatef(3,0,0);                      /* translate 3 right */
  glColor3f(0.5,0.5,1);                     /* blue square */
  glBegin(GL_QUADS);                        /* draw quad */
    glVertex3f(-1, 1,0);                    /* top left */
    glVertex3f( 1, 1,0);                    /* top right */
    glVertex3f( 1,-1,0);                    /* bottom right */
    glVertex3f(-1,-1,0);                    /* bottom left */
  glEnd();
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
  if(info->hw_available) videoflags|=SDL_HWSURFACE;   /* check if we can store surfaces in memory */
  else videoflags|=SDL_SWSURFACE;
  if(info->blit_hw) videoflags|=SDL_HWACCEL;          /* check if hardware blits can be done */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);         /* set up double buffering */
  surface=SDL_SetVideoMode(w,h,bpp,videoflags);
  if(!surface) error("Video mode set failed: %s\n",SDL_GetError());
  if(!initgl()) error("Couldn't initialize opengl.\n");
  resizeglscene(w,h);
  if(fullscreen) SDL_WM_ToggleFullScreen(surface);
  return 1;
}

void handlekeypress(SDL_keysym *keysym) {
  switch(keysym->sym) {
  case SDLK_ESCAPE:
    killglwindow();
  case SDLK_F1:
    SDL_WM_ToggleFullScreen(surface);                 /* toggle fullscreen */
    break;
  }
}

int main (int argc, char* argv[]) {
  SDL_Event event;
  int active=1;
  sdlglinit(640,480,BPP,0);
  while(1) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_ACTIVEEVENT:
        /* check if we have focus */
        if(event.active.gain==0) active=0;
        else active=1;
        break;
      case SDL_VIDEORESIZE:
        surface=SDL_SetVideoMode(event.resize.w,event.resize.h,BPP,videoflags);
        if(!surface) error("Couldn't get surface after resize: %s\n",SDL_GetError());
        resizeglscene(event.resize.w,event.resize.h);
        break;
      case SDL_KEYDOWN:
        handlekeypress(&event.key.keysym);
        break;
      case SDL_QUIT:
        goto theend;
      }
    }
    if(active) drawglscene();
  }
theend:
  killglwindow();
  return 0;
}
