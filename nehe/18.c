/*  usage:
    f -               toggle filter ("nearest", linear, mipmapping)
    l -               toggle light on/off
    pageup/pagedown - move in/out of screen
    up/down -         x-rotation speed
    left/right -      y-rotation speed
    space -           change object
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

int part1,part2,p1,p2;

GLfloat xrot,yrot;                          /* rotations around each axis */
GLfloat xspeed,yspeed;                      /* speed of rotation */
GLfloat z;                                  /* z-pos */
GLuint filter;                              /* which filter to use */
GLuint texture[3];                          /* texture storage */
int light;                                  /* we have light? */
GLfloat ambient[]={.5,.5,.5,1};             /* values for ambient light */
GLfloat diffuse[]={1,1,1,1};                /* values for diffuse light */
GLfloat lightpos[]={0,0,2,1};               /* light position */

GLUquadricObj *quadratic;
GLuint object;

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
  glEnable(GL_DEPTH_TEST);                  /* enable depth testing */
  glDepthFunc(GL_LEQUAL)                    /* type of depth testing */;
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); /* really nice perspective calculations */
  /* unfortunately, need to do this for each resize */
  if(!loadgltexture("18wall.bmp",&texture[0])) return 0;
  glLightfv(GL_LIGHT1,GL_AMBIENT,ambient);  /* set up ambient light */
  glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse);  /* set up diffuse light */
  glLightfv(GL_LIGHT1,GL_POSITION,lightpos);/* position the light */
  glEnable(GL_LIGHT1);                      /* let there be light! */
  quadratic=gluNewQuadric();                /* fetch pointer to new quadric */
  gluQuadricNormals(quadratic,GLU_SMOOTH);  /* create smooth normals */
  gluQuadricTexture(quadratic,GL_TRUE);     /* create texture coords */
  return 1;
}

void ourinit() {
  xrot=yrot=xspeed=yspeed=0;
  z=-5;
  light=1;
  filter=0;
  p1=0; p2=1;
  object=0;
  part1=part2=0;
  memset(keys,0,sizeof(keys));
}

void drawcube() {
  glBegin(GL_QUADS);
    /* front face */
    glNormal3f( 0.0f, 0.0f, 1.0f);          /* the normal vector of the following quad. always pointing away from the cube. lighting needs normals */
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    /* back face */
    glNormal3f( 0.0f, 0.0f,-1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    /* top face */
    glNormal3f( 0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    /* bottom face */
    glNormal3f( 0.0f,-1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    /* right face */
    glNormal3f( 1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    /* left face */
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
  glEnd();
}

int drawglscene() {
  /* remember: positive x is right, positive y is up, positive z is out of the screen */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /* clear screen and depth buffer */
  glLoadIdentity();                         /* reset modelview matrix */
  glTranslatef(0,0,z);                      /* translate: move according to z */
  glRotatef(xrot,1,0,0);                    /* rotate each axis */
  glRotatef(yrot,0,1,0);
  glBindTexture(GL_TEXTURE_2D,texture[filter]);  /* select texture based on chosen filter */
  switch(object) {
  case 0:
    drawcube();
    break;
  case 1:
    glTranslatef(0,0,-1.5);
    gluCylinder(quadratic,1,1,3,32,32);     /* cylinder! bottom radius, top radius, length, #sectors, #z */
    break;
  case 2:
    gluDisk(quadratic,0.5,1.5,32,32);       /* disk! inner radius, outer radius, #sectors, #rings */
    break;
  case 3:
    gluSphere(quadratic,1.3,32,32);         /* sphere, radius, #sectors, #z */
    break;
  case 4:
    glTranslatef(0,0,-1.5);
    gluCylinder(quadratic,1,0,3,32,32);     /* cone */
    break;
  case 5:
    part1+=p1;
    part2+=p2;
    if(part1>359) p1=0,part1=0,p2=1,part2=0;
    if(part2>359) p1=1,p2=0;
    gluPartialDisk(quadratic,0.5,1.5,32,32,part1,part2-part1);  /* disk "sector" */
    break;
  }
  xrot+=xspeed;
  yrot+=yspeed;
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
  case SDLK_l:                    /* toggle light on/off */
    light^=1;
    if(light) glEnable(GL_LIGHTING);
    else glDisable(GL_LIGHTING);
    break;
  case SDLK_SPACE:
    object=(object+1)%6;
    break;
  }
}

void heldkeys() {
  if(keys[SDLK_PAGEUP]) z-=0.02;    /* move into screen */
  if(keys[SDLK_PAGEDOWN]) z+=0.02;  /* move out of screen */
  if(keys[SDLK_UP]) xspeed-=0.01;    /* change rotation speed */
  if(keys[SDLK_DOWN]) xspeed+=0.01;
  if(keys[SDLK_LEFT]) yspeed-=0.01;
  if(keys[SDLK_RIGHT]) yspeed+=0.01;
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
