#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include <stdio.h>
#include <math.h>

float pi,posZ,ax,ay,az;
int running = 1;

FILE *debug;

double rand01(){return ((rand()%32768)/32768.0+(rand()%32768))/32768.0;}

/* Keyboard callback */
static void handle_key_down (SDL_keysym* keysym) {
    switch( keysym->sym ) {
    case SDLK_ESCAPE:
    case SDLK_q:
    case SDLK_x:
        running = 0;
        break;
    case SDLK_1:
         posZ-=0.5;
         break;
    case SDLK_2:
         posZ+=0.5;
         break;
    case SDLK_SPACE:
        // do something :)
        break;
    }
}

/* General event callback */
static void process_events () {
    SDL_Event event;

    /* Grab all the events off the queue */
    while (SDL_PollEvent(&event)) {
        switch( event.type ) {
        case SDL_KEYDOWN:
            /* key presses */
            handle_key_down( &event.key.keysym );
            break;
        case SDL_QUIT:
            /* quit requests (like Ctrl-c) */
            running = 0;
            break;
        }
    }
}

float rotate;
float kulepos,kuleh,fart,acc,fjaerkraft,friksjon;
SDL_Surface* img[7];
unsigned char *buf[7];
unsigned int texture[7];

#define RES 100

void render() {
   int i,j,k;
   float step,x1,y1,x2,y2,a,flat;
   /* kule */
	glLoadIdentity();
	glTranslatef(2.0f,0,posZ);
   glBindTexture(GL_TEXTURE_2D,texture[6]);
	glBegin(GL_TRIANGLES);
	step=pi*2/RES;
   for(a=0,i=0;i<RES;i++,a+=step) {
      x1=sin(a);
      y1=cos(a);
      x2=sin(a+step);
      y2=cos(a+step);
      glColor3f(0.5*sin(a)+0.5,0.5*cos(a)+0.5,0.2*sin(a/2)+0.2);
      if(kulepos<0) {
         flat=1+kulepos;
      } else flat=1;
      glNormal3f(0,0,1);
   	glTexCoord2f(0,0);
      glVertex3f(x2,flat*y2+kulepos,0);
   	glTexCoord2f(0,1);
      glVertex3f(x1,flat*y1+kulepos,0);
   	glTexCoord2f(1,1);
      glVertex3f(0,kulepos,0);
   }
	glEnd();
   /* kube */
	glLoadIdentity();
	glTranslatef(-1.0f,0.0f,posZ);
	glRotatef(rotate,ax,ay,az);
	/* bindtexture must appear before begin */
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
		glColor3f(1,1,1);
		glNormal3f(0,-1,0);
		glTexCoord2f(0,0);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		glTexCoord2f(0,1);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glTexCoord2f(1,1);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1,0);
		glVertex3f( 1.0f, 1.0f, 1.0f);
	glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(0,0);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		glTexCoord2f(0,1);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		glTexCoord2f(1,1);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glTexCoord2f(1,0);
		glVertex3f( 1.0f,-1.0f,-1.0f);
	glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
		glNormal3f(0,0,-1);
		glTexCoord2f(0,0);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		glTexCoord2f(0,1);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1,1);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		glTexCoord2f(1,0);
		glVertex3f( 1.0f,-1.0f, 1.0f);
	glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0);
		glVertex3f( 1.0f,-1.0f,-1.0f);
		glTexCoord2f(0,1);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glTexCoord2f(1,1);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glTexCoord2f(1,0);
		glVertex3f( 1.0f, 1.0f,-1.0f);
	glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   glBegin(GL_QUADS);
		glNormal3f(1,0,0);
		glTexCoord2f(0,0);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0,1);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glTexCoord2f(1,1);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glTexCoord2f(1,0);
		glVertex3f(-1.0f,-1.0f, 1.0f);
	glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
		glNormal3f(-1,0,0);
		glTexCoord2f(0,0);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		glTexCoord2f(0,1);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		glTexCoord2f(1,1);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		glTexCoord2f(1,0);
		glVertex3f( 1.0f,-1.0f,-1.0f);
	glEnd();
}

/* Render scene */
static void draw_scene () {
    /* clear image- and depth buffer to get a clean canvas */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    /* Now looking in negative Z-direction from origo
       +X is right and +Y is up */

    /* set up geometric transformations */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Render object(s) */
   render();

    /* Swap buffers to display result */
    SDL_GL_SwapBuffers( );
}

static void lys()
{
   float l1[]={.4,.4,.4,1};
   float l2[]={1,.6,.6,1};
   float l3[]={1,1,1,1};

   glLightfv(GL_LIGHT1,GL_AMBIENT,l1);
   glLightfv(GL_LIGHT2,GL_DIFFUSE,l2);
   glLightfv(GL_LIGHT3,GL_SPECULAR,l3);
   glEnable(GL_LIGHTING);
   glEnable(GL_COLOR_MATERIAL);

   glEnable(GL_LIGHT1);
   glEnable(GL_LIGHT2);
   glEnable(GL_LIGHT3);
}

/* Initialize OpenGL */
static void setup_opengl (int width, int height) {
    float ratio = (float) width / (float) height;
    /* Let viewport cover entire window */
    glViewport(0, 0, width, height);

    /* Change to the projection matrix and setup viewing frustrum */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* Set up a perspective camera at point (0,0,0), looking in negative Z-direction
       Angle of view is 60 degrees. Near and far clipping planes are 0.1 and 1024 */
    gluPerspective(60.0, ratio, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Gouraud shading model (smooth) */
    glShadeModel(GL_SMOOTH);

    /* Set black clear color */
    glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);									/* Depth Buffer Setup */
    /* Enable Z-buffer (for correct drawing order) */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);								/* The Type Of Depth Testing To Do */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	/* Really Nice Perspective Calculations */
}

float tid;

/* init of non-SDL/openGL stuff */
void init() {
   pi=2*acos(0);
   rotate=0;
   kulepos=6;
   fart=0;
   acc=-0.001;
   fjaerkraft=-0.02;
   friksjon=0.01;
   posZ=-8;

   ax=0.3;
   ay=1.0;
   az=1.7;
   tid=pi;
}

void error(char *s,...) {
   va_list argptr;
   va_start(argptr,s);
   vfprintf(stderr,s,argptr);
   va_end(argptr);
   exit(1);
}

SDL_Surface *LoadBMP(char *filename) {
  /* Load BMP file */
  SDL_Surface *image=SDL_LoadBMP(filename);
  if (!image) error("Couldn't load %s: %s\n", filename, SDL_GetError());  
  return image;
}

void loadtexture(unsigned int *textureptr,int w,int h,unsigned char *rgbptr) {
   /* select the allocated texture, and specify that it's a 2d-texture */
   glBindTexture(GL_TEXTURE_2D,*textureptr);
   /* linear interpolation of texture when magnified */
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   /* linear interpolation in the mipmap level of closest match */
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
   /* copy texture to graphics card, and create mipmaps */
   gluBuild2DMipmaps(GL_TEXTURE_2D,3,w,h,GL_RGB,GL_UNSIGNED_BYTE,rgbptr);
}

void tekstur() {
   int i,size,j;
   char c[64];
   unsigned char t;
   unsigned char *p;
   /* allocate space for texture in graphics card */
   glGenTextures(7,texture);
   for(i=0;i<7;i++) {
      sprintf(c,"t%d.bmp",i+1);
/*      fprintf(debug,"%s\n",c);*/
      img[i]=LoadBMP(c);
      buf[i]=(unsigned char*)malloc(512*512*3);
      memcpy(buf[i],img[i]->pixels,512*512*3);
      p=buf[i];

/*      fprintf(debug,"bits: %d\n",(int)img[i]->format->BytesPerPixel);*/
      size=3*512*512;
      /* reverser */
      for(j=0;j<size/2;j++) {
         t=p[j]; p[j]=p[size-j-1]; p[size-j-1]=t;
      }
      loadtexture(&texture[i],512,512,buf[i]);
   }
   glEnable(GL_TEXTURE_2D);
}

void sprett()
{
   kulepos+=fart;
   if(kulepos<0) {
      fart+=fjaerkraft*kulepos;
   } else fart+=acc;
/*   fprintf(debug,"pos %f fart %f acc %f\n",kulepos,fart,acc);*/
}

void animer() {
   float l1pos[]={100,200,50,1};
   float l2pos[]={-100,200,50,1};
   float l3pos[]={100,-200,50,1};
      rotate+=1;
      if(rotate>=360) {
         rotate=0;
         ax=rand01()-.5;
         ay=rand01()-.5;
         az=rand01()-.5;
      }
   l1pos[0]=100*sin(tid)+200*cos(tid);
   l1pos[1]=100*cos(tid)-200*sin(tid);
   l1pos[2]=50;

   l2pos[0]=100*sin(tid)+200*cos(tid);
   l2pos[1]=50;
   l2pos[2]=100*cos(tid)-200*sin(tid);

   l3pos[0]=50;
   l3pos[1]=100*sin(tid)+200*cos(tid);
   l3pos[2]=100*cos(tid)-200*sin(tid);

   glLightfv(GL_LIGHT1,GL_POSITION,l1pos);
   glLightfv(GL_LIGHT2,GL_POSITION,l2pos);
   glLightfv(GL_LIGHT3,GL_POSITION,l3pos);
   tid+=0.025;
}

/* Program entry point */
int main (int argc, char* argv[]) {
   debug=fopen("debug.txt","w");

    // window dimensions
    int width  = 640;
    int height = 480;
    const SDL_VideoInfo *info;

    /* initialize SDL's video subsystem */
    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf( stderr, "Video initialization failed: %s\n", SDL_GetError( ) );
        return -1;
    }

    /* retrieve video information */
    info = SDL_GetVideoInfo( );
    if (!info) {
        fprintf( stderr, "Video query failed: %s\n", SDL_GetError( ) );
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8 ); /* min 8bit red */
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8 ); /* min 8bit green */
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8 ); /* min 8bit blue */
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); /* 16bit depth buffer */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1); /* require double buffering */

    /* Set video mode */
    SDL_Surface * surface = SDL_SetVideoMode(width, height, info->vfmt->BitsPerPixel, SDL_OPENGL);
    if (!surface) {
        fprintf( stderr, "Video mode set failed: %s\n", SDL_GetError( ) );
        return -1;
    }

    /* OpenGL initialization */
    setup_opengl(width, height);

    init();
    lys();
    tekstur();

    /* main event loop */
    while (running) {
      animer();
        /* Process incoming events */
        process_events();
        /* Draw the screen */
        draw_scene();

      sprett();
        SDL_Delay(1000/500); /* limit to 5fps */
    }

    SDL_Quit(); /* unload SDL */
    fclose(debug);
    return 0;
}
