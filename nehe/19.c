#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXPARTICLES 1000

#define BPP 16
SDL_Surface *surface;
int videoflags;
int fullscreen;
int keys[512];  /* needs to be this large */

int rainbow;
float slowdown,xspeed,yspeed,zoom;

GLuint col;
GLuint texture[1];

typedef struct {
  int active;                               /* active? */
  float life;                               /* particle life (how long left) */
  float fade;                               /* fade speed */
  float r,g,b;                              /* colours */
  float x,y,z;                              /* pos */
  float xi,yi,zi;                           /* dir */
  float xg,yg,zg;                           /* gravity */
} s_part;

s_part particle[MAXPARTICLES];
GLfloat colours[12][3]={
  {1,.5,.5},{1,.75,.5},{1,1,.5},{.75,1,.5},
  {.5,1,.5},
  {.5,1.,75},
  {.5,1,1},
  {.5,.75,1},{.5,.5,1},
  {.75,.5,1},{1,.5,1},{1,.5,.75}
};

void error(char *s,...) {
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
  if(!h) h=1;                               /* apparently this prevents division by zero */
  glViewport(0,0,w,h);                      /* resize viewport */
  glMatrixMode(GL_PROJECTION);              /* select projection matrix */
  glLoadIdentity();                         /* reset it */
  gluPerspective(45,(GLfloat)w/h,0.1,200);  /* calculate aspect ratio, view distance 200 */
  glMatrixMode(GL_MODELVIEW);               /* select modelview matrix */
  glLoadIdentity();                         /* reset it */
}

int initgl() {
  int i;
  glShadeModel(GL_SMOOTH);                  /* enable smooth shading */
  glClearColor(0,0,0,0);                    /* black background: r,g,b,alpha */
  glClearDepth(1);                          /* depth buffer setup */
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); /* really nice perspective calculations */
  glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
  if(!loadgltexture("19part.bmp",&texture[0])) return 0;
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  return 1;
}

void ourinit() {
  int i;
  rainbow=1;
  slowdown=2;
  zoom=-40;
  col=0;
  for(i=0;i<MAXPARTICLES;i++) {
    particle[i].active=1;
    particle[i].life=1;
    particle[i].fade=rand()%100/1000.0+0.003;
    particle[i].r=colours[i%12][0];
    particle[i].g=colours[i%12][1];
    particle[i].b=colours[i%12][2];
    particle[i].xi=(rand()%50-26)*10;
    particle[i].yi=(rand()%50-25)*10;
    particle[i].zi=(rand()%50-25)*10;
    particle[i].xg=0;
    particle[i].yg=-8;
    particle[i].zg=0;
    particle[i].x=particle[i].y=particle[i].z=0;
  }
}

int drawglscene() {
  int i;
  float x,y,z;
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /* clear screen and depth buffer */
  glLoadIdentity();                         /* reset modelview matrix */
  for(i=0;i<MAXPARTICLES;i++) if(particle[i].active) {
    x=particle[i].x;
    y=particle[i].y;
    z=particle[i].z+zoom;
    glColor4f(particle[i].r,particle[i].g,particle[i].b,particle[i].life);
    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2d(1,1);
      glVertex3f(x+.5,y+.5,z);
      glTexCoord2d(0,1);
      glVertex3f(x-.5,y+.5,z);
      glTexCoord2d(1,0);
      glVertex3f(x+.5,y-.5,z);
      glTexCoord2d(0,0);
      glVertex3f(x-.5,y-.5,z);
    glEnd();
    particle[i].x+=particle[i].xi/(slowdown*1000.0);
    particle[i].y+=particle[i].yi/(slowdown*1000.0);
    particle[i].z+=particle[i].zi/(slowdown*1000.0);
    particle[i].xi+=particle[i].xg;
    particle[i].yi+=particle[i].yg;
    particle[i].zi+=particle[i].zg;
    particle[i].life-=particle[i].fade;
    if(particle[i].life<0) {
      particle[i].life=1;
      particle[i].fade=rand()%100/1000.0+0.003;
      particle[i].x=particle[i].y=particle[i].z=0;
      particle[i].xi=xspeed+(rand()%60-32)*10;
      particle[i].yi=yspeed+(rand()%60-30)*10;
      particle[i].zi=rand()%60-30;
      particle[i].r=colours[col][0];
      particle[i].g=colours[col][1];
      particle[i].b=colours[col][2];
    }
    if(keys[SDLK_KP8] && particle[i].yg<1.5) particle[i].yg+=0.01;
    if(keys[SDLK_KP2] && particle[i].yg>-1.5) particle[i].yg-=0.01;
    if(keys[SDLK_KP6] && particle[i].xg<1.5) particle[i].xg+=0.01;
    if(keys[SDLK_KP4] && particle[i].xg>-1.5) particle[i].xg+=0.01;
    if(keys[SDLK_TAB]) {
      particle[i].x=particle[i].y=particle[i].z=0;
      particle[i].xi=(rand()%50-26)*10;
      particle[i].xi=(rand()%50-25)*10;
      particle[i].xi=(rand()%50-25)*10;
    }
  }
  if(rainbow) col=(col+1)%12;
  SDL_GL_SwapBuffers();                     /* needed when double buffering is on */
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
  if(fullscreen) videoflags|=SDL_FULLSCREEN;
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
  case SDLK_RETURN:
    rainbow^=1;
    break;
  case SDLK_SPACE:
    rainbow=0;
    col=(col+1)%12;
    break;
  }
}

void heldkeys() {
  if(keys[SDLK_PLUS] && slowdown>1) slowdown-=0.01;
  if(keys[SDLK_MINUS] && slowdown<4) slowdown+=0.01;
  if(keys[SDLK_PAGEUP]) zoom+=0.1;
  if(keys[SDLK_PAGEDOWN]) zoom-=0.1;
  if(keys[SDLK_UP] && yspeed<200) yspeed+=1;
  if(keys[SDLK_DOWN] && yspeed>-200) yspeed-=1;
  if(keys[SDLK_RIGHT] && xspeed<200) xspeed+=1;
  if(keys[SDLK_LEFT] && xspeed>-200) xspeed-=1;
}

int main (int argc, char* argv[]) {
  SDL_Event event;
  ourinit();
  sdlglinit(640,480,BPP,0);
  while(1) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_VIDEORESIZE:
        surface=SDL_SetVideoMode(event.resize.w,event.resize.h,BPP,videoflags);
        if(!surface) error("Couldn't get surface after resize: %s\n",SDL_GetError());
        initgl();
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
    heldkeys();
    drawglscene(),SDL_Delay(1000/70.0);
  }
theend:
  killglwindow();
  return 0;
}
