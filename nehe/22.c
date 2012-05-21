#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* set next line to 0 to disable extensions */
#define __ARB_ENABLE 1
#define MAXEMBOSS 0.01
#define MAXEXTENSIONSPACE 10240
#define MAXEXTENSIONLENGTH 256
int multitexturesupported;
int usemultitexture;
GLint maxtexelunits;

GLuint  filter;
GLuint  texture[3];
GLuint  bump[3];
GLuint  invbump[3];
GLuint  gllogo;
GLuint  multilogo;
GLfloat lightambient[]  = { 0.2, 0.2, 0.2};
GLfloat lightdiffuse[]  = { 1.0, 1.0, 1.0};
GLfloat lightposition[] = { 0.0, 0.0, 2.0};
GLfloat grey[]    = { 0.5 , 0.5, 0.5, 1.0};

GLfloat data[]= {
  /* FRONT FACE */
  0.0f, 0.0f,   -1.0f, -1.0f, +1.0f,
  1.0f, 0.0f,   +1.0f, -1.0f, +1.0f,
  1.0f, 1.0f,   +1.0f, +1.0f, +1.0f,
  0.0f, 1.0f,   -1.0f, +1.0f, +1.0f,
  /* BACK FACE */
  1.0f, 0.0f,   -1.0f, -1.0f, -1.0f,
  1.0f, 1.0f,   -1.0f, +1.0f, -1.0f,
  0.0f, 1.0f,   +1.0f, +1.0f, -1.0f,
  0.0f, 0.0f,   +1.0f, -1.0f, -1.0f,
  /* Top Face */
  0.0f, 1.0f,   -1.0f, +1.0f, -1.0f,
  0.0f, 0.0f,   -1.0f, +1.0f, +1.0f,
  1.0f, 0.0f,   +1.0f, +1.0f, +1.0f,
  1.0f, 1.0f,   +1.0f, +1.0f, -1.0f,
  /* Bottom Face */
  1.0f, 1.0f,   -1.0f, -1.0f, -1.0f,
  0.0f, 1.0f,   +1.0f, -1.0f, -1.0f,
  0.0f, 0.0f,   +1.0f, -1.0f, +1.0f,
  1.0f, 0.0f,   -1.0f, -1.0f, +1.0f,
  /* Right Face */
  1.0f, 0.0f,   +1.0f, -1.0f, -1.0f,
  1.0f, 1.0f,   +1.0f, +1.0f, -1.0f,
  0.0f, 1.0f,   +1.0f, +1.0f, +1.0f,
  0.0f, 0.0f,   +1.0f, -1.0f, +1.0f,
  /* Left Face */
  0.0f, 0.0f,   -1.0f, -1.0f, -1.0f,
  1.0f, 0.0f,   -1.0f, -1.0f, +1.0f,
  1.0f, 1.0f,   -1.0f, +1.0f, +1.0f,
  0.0f, 1.0f,   -1.0f, +1.0f, -1.0f
};

PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB=0;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB=0;
PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB=0;
PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB=0;
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB=0;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB=0;

#define BPP 16
SDL_Surface *surface;
int videoflags;
int fullscreen;
int keys[512];  /* needs to be this large */

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

void createtextures(GLuint *texture,SDL_Surface *pic) {
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
}

int loadgltexture(char *s,int *texture) {
  SDL_Surface *pic=loadbmp(s);              /* load our texture */
  glGenTextures(3,texture);                 /* create the texture */
  SDL_FreeSurface(pic);                     /* throw texture */
  createtextures(texture,pic);
  return 1;
}

char alpha[1048576]; /* hope this is large enough */

void loadbumpmaps() {
  SDL_Surface *pic;
  int i,a;
  pic=loadbmp("22bump.bmp");
  glPixelTransferf(GL_RED_SCALE,0.5f); /* scale 50%: half intensity */
  glPixelTransferf(GL_GREEN_SCALE,0.5f);
  glPixelTransferf(GL_BLUE_SCALE,0.5f);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); /* no wrapping */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  glGenTextures(3,bump);
  createtextures(bump,pic);
  /* invert the bumpmap */
  char *x=(char *)pic->pixels;
  for(i=0;i<3*pic->w*pic->h;i++) x[i]^=255;
  glGenTextures(3,invbump);
  createtextures(invbump,pic);
  SDL_FreeSurface(pic);                     /* throw texture */
  /* load logo bitmaps */
  pic=loadbmp("22openglalpha.bmp");
  for(a=0;a<pic->w*pic->h;a++) alpha[4*a+3]=((char *)(pic->pixels))[a*3];
  SDL_FreeSurface(pic);
  pic=loadbmp("22opengl.bmp");
  for(a=0;a<<pic->w*pic->h;a++) {
    alpha[4*a]=((char *)(pic->pixels))[a*3];
    alpha[4*a+1]=((char *)(pic->pixels))[a*3+1];
    alpha[4*a+2]=((char *)(pic->pixels))[a*3+2];
  }
  SDL_FreeSurface(pic);
}

int isinstring(char *haystack,char *needle) {
  int pos=0,maxpos=strlen(needle)-1,len=strlen(haystack),i;
  for(i=0;i<len;i++) if(i==0 || haystack[i-1]=='\n') {
    pos=0;
    while(haystack[i]!='\n') {
      if(haystack[i]==needle[pos]) pos++;
      if(pos>maxpos && haystack[i+1]=='\n') return 1;
      i++;
    }
  }
  return 0;
}

int initmultitexture() {
  char *ext;
  int len,i;
  ext=strdup(glGetString(GL_EXTENSIONS));
  len=strlen(ext);
  for(i=0;i<len;i++) if(ext[i]==' ') ext[i]='\n';
  if(isinstring(ext,"GL_ARB_multitexture") && __ARB_ENABLE && isinstring(ext,"GL_EXT_texture_env_combine")) {
    glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxtexelunits);
    glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) wglGetProcAddress("glMultiTexCoord1fARB");
    glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
    glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) wglGetProcAddress("glMultiTexCoord3fARB");
    glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) wglGetProcAddress("glMultiTexCoord4fARB");
    glActiveTextureARB   = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
    glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
    return 1;
  }
  usemultitexture=0;
  free(ext);
  return 0;
}

void initlights() {
  glLightfv(GL_LIGHT1,GL_AMBIENT,lightambient);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,lightdiffuse);
  glLightfv(GL_LIGHT1,GL_POSITION,lightposition);
  glEnable(GL_LIGHT1);
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
  glClearColor(0,0,0,0);                    /* black background: r,g,b,alpha */
  glClearDepth(1);                          /* depth buffer setup */
  glEnable(GL_DEPTH_TEST);                  /* enable depth testing */
  glDepthFunc(GL_LEQUAL)                    /* type of depth testing */;
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); /* really nice perspective calculations */
  /* unfortunately, need to do this for each resize */
  if(!loadgltexture("22blablTEMP.bmp",&texture[0])) return 0;
  loadbumpmaps();
  return 1;
}

void ourinit() {
  memset(keys,0,sizeof(keys));
  multitexturesupported=0;
  usemultitexture=1;
  maxtexelunits=1;
  filter=1;
}

int drawglscene() {
  /* remember: positive x is right, positive y is up, positive z is out of the screen */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /* clear screen and depth buffer */
  glLoadIdentity();                         /* reset modelview matrix */

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
    break;
  case SDLK_l:                    /* toggle light on/off */
    break;
  }
}

void heldkeys() {
  if(keys[SDLK_PAGEUP]);
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
