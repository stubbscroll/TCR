#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
typedef long long ll;
typedef unsigned int uint;
#define BPP 16
SDL_Surface *surface;
int videoflags;
int fullscreen;
int keys[512];  /* needs to be this large */

char vline[11][10];
char hline[10][11];
char filled;                                /* grid filled in? */
char gameover;                              /* is it game over? */
char anti;                                  /* antialisasing on? */

int delay;
int adjust;
int lives;
int level;
int level2;
int stage;

typedef struct { int fx,fy,x,y; float spin; } s_object;
s_object player,enemy[9],hourglass;

int steps[]={1,2,4,5,10,20};

GLuint texture[3];
GLuint base;                                /* display list for font */

void error(char *s,...) {
  FILE *f;
  va_list argptr;
  va_start(argptr,s);
  vfprintf(stderr,s,argptr);
  f=fopen("JALL","w");
  vfprintf(f,s,argptr);
  fclose(f);
  va_end(argptr);
  SDL_Quit();
  exit(1);
}

/* sound routines */

#define SOUNDS 4
#define FREQ 22050
#define BUF 512
typedef struct {
  Uint8 *buffer;
  Uint32 len;
  Uint32 pos;
  int loop;
} s_sample;
s_sample sounds[SOUNDS],cursound;

void loadsound(const char *s,int num) {
  SDL_AudioSpec spec;
  Uint8 *buffer;
  Uint32 len;
  SDL_AudioCVT cvt;
  SDL_AudioSpec *sound=SDL_LoadWAV(s,&spec,&buffer,&len);
  if(!sound) error("can't open sound file %s: %s\n",s,SDL_GetError());
  /* convert sound to 16-bit stereo, 44khz */
  SDL_BuildAudioCVT(&cvt,spec.format,spec.channels,spec.freq,AUDIO_S16,2,FREQ);
  cvt.buf=(Uint8 *)malloc(len*cvt.len_mult);
  memcpy(cvt.buf,buffer,len);
  cvt.len=len;
  SDL_ConvertAudio(&cvt);
  sounds[num].buffer=buffer;
  sounds[num].len=len;
  sounds[num].loop=0;
  SDL_FreeWAV(buffer);
}

void playsound(int num) {
  cursound=sounds[num];
}

/* this function is called by the callback thingy */
void mixaudio(void *unused,Uint8 *stream,int len) {
  Uint32 amount;
loop:
  if(!cursound.buffer) return;
  amount=cursound.len-cursound.pos;
  if(amount>len) amount=len;
  if(amount==0) {
    if(cursound.loop || !cursound.len) { cursound.buffer=0; return; }
    else { cursound.pos=0; goto loop; }
  }
  SDL_MixAudio(stream,&cursound.buffer[cursound.pos],amount,SDL_MIX_MAXVOLUME);
  cursound.pos+=amount;
}

void initsound() {
  SDL_AudioSpec fmt;
  fmt.freq=FREQ;
  fmt.format=AUDIO_S16;
  fmt.channels=2;
  fmt.samples=BUF;
  fmt.callback=mixaudio;  /* specify callback function */
  fmt.userdata=0;
  if(SDL_OpenAudio(&fmt,0)<0) error("couldn't initialize sdl sound: %s\n",SDL_GetError());
  cursound.buffer=0;
}

void oursoundinit() {
  loadsound("21complete.wav",0);
  loadsound("21die.wav",1);
  loadsound("21freeze.wav",2);  sounds[2].loop=1;
  loadsound("21hourglass.wav",3);
}

void shutdownsound() {
  int i;
  for(i=0;i<SOUNDS;i++) free(sounds[i].buffer);
}

/* end of sound */

void resetobjects() {
  int i;
  player.x=player.y=player.fx=player.fy=0;
  for(i=0;i<stage*level;i++) {
    enemy[i].x=5+rand()%6;
    enemy[i].y=rand()%11;
    enemy[i].fx=enemy[i].x*60;
    enemy[i].fy=enemy[i].y*40;
  }
}

SDL_Surface *loadbmp(char *s) {
  SDL_Surface *image=SDL_LoadBMP(s);
  if (!image) error("Couldn't load %s: %s\n",s,SDL_GetError());
  return image;
}

int loadgltexture(char *s,int *texture) {
  SDL_Surface *pic=loadbmp(s);              /* load our texture */
  glGenTextures(3,texture);                 /* create the texture */
  /* create linear filtered texture */
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glTexImage2D(GL_TEXTURE_2D,0,3,pic->w,pic->h,0,GL_BGR,GL_UNSIGNED_BYTE,pic->pixels);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  SDL_FreeSurface(pic);                     /* throw texture */
  return 1;
}

void buildfont() {
  int i;
  float cx,cy;
  base=glGenLists(256);
  glBindTexture(GL_TEXTURE_2D,texture[0]);
#define Z 0.0625
  for(i=0;i<256;i++) {
    /* nb, sdl reads bmp files unconverted (rotated 180 degrees) */
    cx=1-i%16/16.0;
    cy=1-i/16/16.0;
    glNewList(base+255-i,GL_COMPILE);
      glBegin(GL_QUADS);
        glTexCoord2f(cx-Z,cy);
        glVertex2i(0,16);
        glTexCoord2f(cx,cy);
        glVertex2i(16,16);
        glTexCoord2f(cx,cy-Z);
        glVertex2i(16,0);
        glTexCoord2f(cx-Z,cy-Z);
        glVertex2i(0,0);
      glEnd();
      glTranslated(15,0,0);                   /* move to the right of the character */
    glEndList();
  }
#undef Z
}

void killfont() {
  glDeleteLists(base,256);
}

void glprintf(GLint x,GLint y,int set, const char *fmt,...) {
  char text[1024];
  va_list ap;
  va_start(ap,fmt);
  vsprintf(text,fmt,ap);
  va_end(ap);
  set&=1;
  glEnable(GL_TEXTURE_2D);
  glLoadIdentity();
  glTranslated(x,y,0);
  glListBase(base-32+128*set);
  if(!set) glScalef(1.5,2,1);
  glCallLists(strlen(text),GL_UNSIGNED_BYTE,text);
  glDisable(GL_TEXTURE_2D);
}

void resizeglscene(int w,int h) {
  glViewport(0,0,w,h);                      /* resize viewport */
  glMatrixMode(GL_PROJECTION);              /* select projection matrix */
  glLoadIdentity();                         /* reset it */
  glOrtho(0,w,h,0,-1,1);
  glMatrixMode(GL_MODELVIEW);               /* select modelview matrix */
  glLoadIdentity();                         /* reset it */
}

/* stuff to be initialized for change between fullscreen/windowed */
int initgl() {
  glShadeModel(GL_SMOOTH);                  /* select shade model: smooth */
  glClearColor(0,0,0,.5);                   /* black background: r,g,b,alpha */
  glClearDepth(1);                          /* depth buffer setup */
  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  /* unfortunately, need to do this for each resize */
  if(!loadgltexture("21font.bmp",&texture[0])) return 0;
  if(!loadgltexture("21image.bmp",&texture[1])) return 0;
  buildfont();
  return 1;
}

void ourinit() {
  anti=1;
  adjust=3;
  lives=5;
  level=level2=1;
  stage=1;
  memset(keys,0,sizeof(keys));
}

int drawglscene() {
  int i,j;
  /* remember: positive x is right, positive y is up, positive z is out of the screen */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); /* clear screen and depth buffer */
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glColor3f(1,.5,1);
  glprintf(207,24,0,"GRID CRAZY");
  glColor3f(1,1,0);
  glprintf(20,20,1,"Level:%2d",level2);
  glprintf(20,40,1,"Stage:%2d",stage);
  if(gameover) {
    glColor3ub(rand()%255,rand()%255,rand()%255);
    glprintf(472,20,1,"GAME OVER");
    glprintf(456,40,1,"PRESS SPACE");
  }
  /* draw n copies of player lives */
  for(i=0;i<lives-1;i++) {
    glLoadIdentity();
    glTranslatef(490+i*40,40,0);
    glRotatef(-player.spin,0,0,1);
    glColor3f(0,1,0);
    glBegin(GL_LINES);
      glVertex2d(-5,-5);
      glVertex2d(5,5);
      glVertex2d(5,-5);
      glVertex2d(-5,5);
    glEnd();
    glRotatef(-player.spin*0.5,0,0,1);
    glColor3f(0,.75,0);
    glBegin(GL_LINES);
      glVertex2d(-7,0);
      glVertex2d(7,0);
      glVertex2d(0,-7);
      glVertex2d(0,7);
    glEnd();
  }
  filled=1;
  glLineWidth(2);
  glDisable(GL_LINE_SMOOTH);
  glLoadIdentity();
  for(i=0;i<11;i++) for(j=0;j<11;j++) {
    if(hline[i][j]) glColor3f(1,1,1);
    else glColor3f(0,.5,1);
    if(i<10) {
      if(!hline[i][j]) filled=0;
      glBegin(GL_LINES);
        glVertex2d(20+i*60,70+j*40);
        glVertex2d(80+i*60,70+j*40);
      glEnd();
    }
    if(vline[i][j]) glColor3f(1,1,1);
    else glColor3f(0,.5,1);
    if(j<10) {
      if(!vline[i][j]) filled=0;
      glBegin(GL_LINES);
        glVertex2d(20+i*60,70+j*40);
        glVertex2d(20+i*60,110+j*40);
      glEnd();
    }
    glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,texture[1]);
    if(i<10 && j<10) {
      if(hline[i][j]&&hline[i][j+1]&&vline[i+1][j]&&vline[i][j]) {
        glBegin(GL_QUADS);
          glTexCoord2f(i/10.0+.1,1-j/10.0);
          glVertex2d(79+i*60,71+j*40);
          glTexCoord2f(i/10.0,1-j/10.0);
          glVertex2d(21+i*60,71+j*40);
          glTexCoord2f(i/10.0,1-j/10.0+0.1);
          glVertex2f(21+i*60,109+j*40);
          glTexCoord2f(i/10.0+0.1,1-j/10.0+0.1);
          glVertex2f(79+i*60,109+j*40);
        glEnd();
      }
    }
    glDisable(GL_TEXTURE_2D);
  }
  glLineWidth(1);
  if(anti) glEnable(GL_LINE_SMOOTH);
  if(hourglass.fx==1) {
    glLoadIdentity(),
    glTranslatef(20+hourglass.x*60,70+hourglass.y*40,0);
    glRotatef(hourglass.spin,0,0,1);
    glColor3ub(rand()%255,rand()%255,rand()%255 );
    glBegin(GL_LINES);
      glVertex2d(-5,-5);
      glVertex2d(5,5);
      glVertex2d(5,-5);
      glVertex2d(-5,5);
      glVertex2d(-5,5);
      glVertex2d(5,5);
      glVertex2d(-5,-5);
      glVertex2d(5,-5);
    glEnd();
  }
  glLoadIdentity();
  glTranslatef(player.fx+20,player.fy+70,0);
  glRotatef(player.spin,0,0,1);
  glColor3f(0,1,0);
  glBegin(GL_LINES);
    glVertex2d(-5,-5);
    glVertex2d(5,5);
    glVertex2d(5,-5);
    glVertex2d(-5,5);
  glEnd();
  glRotatef(player.spin*.5,0,0,1);
  glColor3f(0,.75,0);
  glBegin(GL_LINES);
    glVertex2d(-7,0);
    glVertex2d(7,0);
    glVertex2d(0,-7);
    glVertex2d(0,7);
  glEnd();
  for(i=0;i<stage*level;i++) {
    glLoadIdentity();
    glTranslatef(enemy[i].fx+20,enemy[i].fy+70,0);
    glColor3f(1,.5,.5);
    glBegin(GL_LINES);
      glVertex2d(0,-7);
      glVertex2d(-7,0);
      glVertex2d(-7,0);
      glVertex2d(0,7);
      glVertex2d(0,7);
      glVertex2d(7,0);
      glVertex2d(7,0);
      glVertex2d(0,-7);
    glEnd();
    glRotatef(enemy[i].spin,0,0,1);
    glColor3f(1,0,0);
    glBegin(GL_LINES);
      glVertex2d(-7,-7);
      glVertex2d(7,7);
      glVertex2d(-7,7);
      glVertex2d(7,-7);
    glEnd();
  }
  SDL_GL_SwapBuffers();                     /* show our changes! needed when double buffering is on */
  return 1;
}

void killglwindow() {
  killfont();
  SDL_Quit();
  exit(0);
}

int sdlglinit(int w,int h,int bpp,int fullscreen) {
  const SDL_VideoInfo *info;
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO)<0) error("Video initialization failed: %s\n",SDL_GetError());
  info=SDL_GetVideoInfo();
  if(!info) {
    videoflags=SDL_OPENGL|SDL_GL_DOUBLEBUFFER|SDL_HWPALETTE|SDL_RESIZABLE|SDL_SWSURFACE;
  } else {
    videoflags=SDL_OPENGL                               /* enable opengl in sdl */
      |SDL_GL_DOUBLEBUFFER                              /* enable double buffering */
      |SDL_HWPALETTE                                    /* store the palette in hardware */
      |SDL_RESIZABLE;                                   /* enable windows resizing */
    if(fullscreen) videoflags|=SDL_FULLSCREEN;
    if(info->hw_available) videoflags|=SDL_HWSURFACE;   /* check if we can store surfaces in memory */
    else videoflags|=SDL_SWSURFACE;
    if(info->blit_hw) videoflags|=SDL_HWACCEL;          /* check if hardware blits can be done */
  }
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
  case SDLK_a:
    anti^=1;
    break;
  case SDLK_SPACE:
    if(gameover) {
      gameover=0;
      filled=1;
      level=level2=1;
      stage=0;
      lives=5;
      memset(hline,0,sizeof(hline));
      memset(vline,0,sizeof(vline));
    }
    break;
  }
}

void heldkeys() {
  if(keys[SDLK_RIGHT] && player.x<10 && player.fx==player.x*60 && player.fy==player.y*40) hline[player.x++][player.y]=1;
  if(keys[SDLK_LEFT] && player.x>0 && player.fx==player.x*60 && player.fy==player.y*40) hline[--player.x][player.y]=1;
  if(keys[SDLK_DOWN] && player.y<10 && player.fx==player.x*60 && player.fy==player.y*40) vline[player.x][player.y++]=1;
  if(keys[SDLK_UP] && player.y>0 && player.fx==player.x*60 && player.fy==player.y*40) vline[player.x][--player.y]=1;
}

void dostuff() {
  int i,j;
  if(!gameover) {
    for(i=0;i<stage*level;i++) {
      if(enemy[i].x<player.x && enemy[i].fy==enemy[i].y*40) enemy[i].x++;
      if(enemy[i].x>player.x && enemy[i].fy==enemy[i].y*40) enemy[i].x--;
      if(enemy[i].y<player.y && enemy[i].fx==enemy[i].x*60) enemy[i].y++;
      if(enemy[i].y>player.y && enemy[i].fx==enemy[i].x*60) enemy[i].y--;
      if(delay>3-level && hourglass.fx!=2) {
        delay=0;
        for(j=0;j<stage*level;j++) {
          if(enemy[j].fx<enemy[j].x*60) enemy[j].fx+=steps[adjust],enemy[j].spin+=steps[adjust];
          if(enemy[j].fx>enemy[j].x*60) enemy[j].fx-=steps[adjust],enemy[j].spin-=steps[adjust];
          if(enemy[j].fy<enemy[j].y*40) enemy[j].fy+=steps[adjust],enemy[j].spin+=steps[adjust];
          if(enemy[j].fy>enemy[j].y*40) enemy[j].fy-=steps[adjust],enemy[j].spin-=steps[adjust];
        }
      }
      if(enemy[i].fx==player.fx && enemy[i].fy==player.fy) {
        if(!--lives) gameover=1;
        resetobjects();
        playsound(1);
      }
    }
    if(player.fx<player.x*60) player.fx+=steps[adjust];
    if(player.fx>player.x*60) player.fx-=steps[adjust];
    if(player.fy<player.y*40) player.fy+=steps[adjust];
    if(player.fy>player.y*40) player.fy-=steps[adjust];
  }
  if(filled) {
    playsound(0);
    stage++;
    if(stage>3) {
      stage=1;
      level++;
      level2++;
      if(level>3) {
        level=3;
        lives++;
        if(lives>5) lives=5;
      }
      resetobjects();
      for(i=0;i<11;i++) for(j=0;j<11;j++) {
        if(i<10) hline[i][j]=0;
        if(j<10) vline[i][j]=0;
      }
    }
  }
  if(player.fx==hourglass.x*60 && player.fy==hourglass.y*40 && hourglass.fx==1) {
    playsound(2);
    hourglass.fx=2;
    hourglass.fy=0;
  }
  player.spin+=0.5*steps[adjust];
  if(player.spin>360) player.spin-=360;
  hourglass.spin-=0.25*steps[adjust];
  if(hourglass.spin<0) hourglass.spin+=360;
  hourglass.fy+=steps[adjust];
  if(hourglass.fx==0 && hourglass.fy>6000/level) {
    playsound(3);
    /* TODO playsound hourglass.wav, with loop */
    hourglass.x=rand()%10+1;
    hourglass.y=rand()%11;
    hourglass.fx=1;
    hourglass.fy=0;
  }
  if(hourglass.fx==1 && hourglass.fy>6000/level) hourglass.fx=hourglass.fy=0;
  if(hourglass.fx==2 && hourglass.fy>500+500*level) {
    /* TODO turn off sound */
    hourglass.fx=hourglass.fy=0;
  }
  delay++;
}

int main (int argc, char* argv[]) {
  int done=0;
  SDL_Event event;
  fullscreen=0;
  sdlglinit(640,480,BPP,fullscreen);
  initsound();
  ourinit();
  oursoundinit();
  resetobjects();
  while(!done) {
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
    dostuff();
    drawglscene();
    SDL_Delay(1000/70.0);
  }
theend:
  shutdownsound();
  killglwindow();
  return 0;
}
