#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#endif
#include <math.h>
#include "holger_bmp.h"

static int window_width = 640;
static int window_height = 480;
GLuint	texture[3];
double PI;

/*  lys */
GLfloat ambientlys[]={0,0,1,1};
GLfloat diffuselys[]={1,.4,.4,1};
GLfloat lyspos[]={4,10,4,1};

unsigned char *LoadBMP(char *Filename, int *width, int *height) {
	FILE *File=NULL;
	if (!Filename)								// Make Sure A Filename Was Given
	{
		return NULL;							// If Not Return NULL
	}
	File=fopen(Filename,"r");
	if (File)								// Does The File Exist?
	{
		fclose(File);							// Close The Handle
        return (unsigned char*)load_bmp_char(Filename, width, height);
	}
	return NULL;								// If Load Failed Return NULL
}

int LoadGLTextures()								// Load Bitmaps And Convert To Textures
{
	int Status=0;
  int width, height;
	unsigned char *TextureImage[1];
	memset(TextureImage,0,sizeof(void *)*1);
	if (TextureImage[0]=LoadBMP("pavement.bmp", &width, &height))
	{
		Status=1;
		glGenTextures(1, &texture[0]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		// Generate The Texture
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
		free(TextureImage[0]);						// Free The Image Structure
	}
	
	if (TextureImage[0]=LoadBMP("grass.bmp", &width, &height))
	{
		Status+=2;
		glGenTextures(1, &texture[1]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		// Generate The Texture
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
		free(TextureImage[0]);						// Free The Image Structure
	}
	
	if (TextureImage[0]=LoadBMP("building.bmp", &width, &height))
	{
		Status+=4;
		glGenTextures(1, &texture[2]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		// Generate The Texture
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
		free(TextureImage[0]);						// Free The Image Structure
	}
	return Status==7;								// Return The Status
}

void drawCube(float width, float height, float depth){
  //  bakkeplanet er y=0
  //  la firkantens origo være 0, og størrelsen være gitt ved input
  //  definer flatene counterclockwise

  width/2; depth/2; // width=x, depth=z
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glBegin(GL_QUADS);
    //  topp
    glColor3f(0,0,1);
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(width,height,-depth);
    glTexCoord2f(0,1); glVertex3f(width,height,depth);
    glTexCoord2f(1,1); glVertex3f(-width,height,depth);
    glTexCoord2f(1,0); glVertex3f(-width,height,-depth);
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[2]);
  glBegin(GL_QUADS);
    //  front
    glColor3f(1,0,0);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-width,height,depth);
    glTexCoord2f(0,1); glVertex3f(-width,0,depth);
    glTexCoord2f(1,1); glVertex3f(width,0,depth);
    glTexCoord2f(1,0); glVertex3f(width,height,depth);
    
    //  back
    glColor3f(0,1,0);
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(-width,height,-depth);
    glTexCoord2f(0,1); glVertex3f(-width,0,-depth);
    glTexCoord2f(1,1); glVertex3f(width,0,-depth);
    glTexCoord2f(1,0); glVertex3f(width,height,-depth);
    
    //  left
    glColor3f(1,1,0);
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(-width,height,-depth);
    glTexCoord2f(0,1); glVertex3f(-width,0,-depth);
    glTexCoord2f(1,1); glVertex3f(-width,0,depth);
    glTexCoord2f(1,0); glVertex3f(-width,height,depth);
    
    //  right
    glColor3f(0,1,1);
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(width,height,depth);
    glTexCoord2f(0,1); glVertex3f(width,0,depth);
    glTexCoord2f(1,1); glVertex3f(width,0,-depth);
    glTexCoord2f(1,0); glVertex3f(width,height,-depth);
    
  glEnd();
}

 void drawSphere(double r, int lats, int longs) {
	//write code here

  gluSphere(gluNewQuadric(),r,lats,longs);
  return;

  int i,j,k;
  double r1,r2,x1,y1,x2,y2,h1,h2,z1,z2;
  glBegin(GL_QUADS);
  for(i=0;i<lats;i++) { /*  høyde */
    z1=-1+(double)i/(lats/2.0);
    z2=-1+(double)(i+1)/(lats/2.0);
    r1=r*sqrt(1-z1*z1);
    r2=r*sqrt(1-z2*z2);
    h1=(i-lats/2.0)*2*r/lats;
    h2=(i+1-lats/2.0)*2*r/lats;
    for(j=0;j<longs;j++) {  /*  skive */
      k=j+1;
      x1=cos(j*2*PI/longs)-sin(j*2*PI/longs);
      y1=cos(j*2*PI/longs)+sin(j*2*PI/longs);
      x2=cos(k*2*PI/longs)-sin(k*2*PI/longs);
      y2=cos(k*2*PI/longs)+sin(k*2*PI/longs);
      glVertex3f(r1*x1,h1,r1*y1);
      glVertex3f(r2*x1,h2,r2*y1);
      glVertex3f(r2*x2,h2,r2*y2);
      glVertex3f(r1*x2,h1,r1*y2);
    }
  }
  glEnd();
}

int cameramode; /*  1=1st person 2=bird's eye 3=3rd person (panorama) */
double lookatx,lookaty,lookatz;
double angle;
char keys[256];
double EYEDIST=10;
double WALKSPEED=0.02;

void styring() {
  if(keys['a']) angle-=0.01;
  if(keys['d']) angle+=0.01;
  if(keys['w']) {
    lookatx+=cos(angle)*WALKSPEED;
    lookatz+=sin(angle)*WALKSPEED;
  }
  if(keys['s']) {
    lookatx-=cos(angle)*WALKSPEED;
    lookatz-=sin(angle)*WALKSPEED;
  }
}

void display(void) {
double static vinkel=0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

  styring();
  
  if(cameramode==1) {
    gluLookAt(lookatx,lookaty,lookatz,lookatx+cos(angle)*EYEDIST, lookaty, lookatz+sin(angle)*EYEDIST,  0.0, 1.0, 0.0);
  } else if(cameramode==2) {
    gluLookAt(0.0, 20.2, 20.0, lookatx+cos(angle)*EYEDIST, lookaty, lookatz+sin(angle)*EYEDIST, 0.0, 1.0, 0.0);
  } else {
    gluLookAt(0.0, 100.2, 0.0, lookatx+cos(angle)*EYEDIST, lookaty, lookatz+sin(angle)*EYEDIST, 0.0, 1.0, 0.0);
  }
	
	glTranslatef(0.0,0.0,-8.0);
	
	//street
  glBindTexture(GL_TEXTURE_2D,texture[0]);
	glBegin(GL_QUADS);
	glColor3f(0.5f,0.5f,0.5f);
  glNormal3f(0,1,0);
	glTexCoord2f(0,0); glVertex3f( (10),(0), (10));
	glTexCoord2f(0,10); glVertex3f(-(10),(0), (10));
	glTexCoord2f(10,10); glVertex3f(-(10),(0),-(10));
	glTexCoord2f(10,0); glVertex3f( (10),(0),-(10));
	glEnd();
	
	//garden
  glBindTexture(GL_TEXTURE_2D,texture[1]);
	glTranslatef(0.0f,0.01f,0.0f);
	glBegin(GL_QUADS);
	glColor3f(0.2f,2.0f,0.4f);
  glNormal3f(0,1,0);
	glTexCoord2f(0,0); glVertex3f( (3),(0), (3));
	glTexCoord2f(0,1); glVertex3f(-(3),(0), (3));
	glTexCoord2f(1,1); glVertex3f(-(3),(0),-(3));
	glTexCoord2f(1,0); glVertex3f( (3),(0),-(3));
	glEnd();
	glTranslatef(0.0f,-0.01f,0.0f);

  
  //  planet er 20x20 stort og vi står i origo
  //  (x=-10..10, z=-10..10)

  double DIST=6.5;
  for(int i=-1;i<2;i++) for(int j=-1;j<2;j++) if(i||j) {
    glTranslatef(i*DIST,0,j*DIST); drawCube(1.5,4,1.4); glTranslatef(-i*DIST,0,-j*DIST);
    
  }

  glRotatef(vinkel,0,1,0);

  glColor3f(1,0,0);
  glTranslatef(0,1,0);
  drawSphere(1,3,4);
#define SDIST 3
  glColor3f(0,1,0);
  glTranslatef(SDIST,0,0);
  drawSphere(0.44,40,30);
  glTranslatef(-SDIST,0,0);
  
  glColor3f(0,1,1);
  glTranslatef(0,0,SDIST);
  drawSphere(0.3,40,30);
  glTranslatef(0,0,-SDIST);
  
  glColor3f(0,0,1);
  glTranslatef(-SDIST,0,0);
  drawSphere(0.6,40,30);
  glTranslatef(SDIST,0,0);

  glColor3f(1,1,0);
  glTranslatef(0,0,-SDIST);
  drawSphere(0.5,40,30);

	glutSwapBuffers();
  vinkel+=0.1;
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,1000.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
}

void keydown(unsigned char key, int x, int y) {
  switch (key) {
  case '\033':
    exit(0);
    break;
  case 49:
  case 50:
  case 51:
    cameramode=key-48;
    break;
  default:
    keys[key]=1;
    return;
  }
  glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y) {
  keys[key]=0;
  glutPostRedisplay();
}





void init(int argc, char **argv) {
  PI=acos(0)*2;
  cameramode=1;
  lookatx=0.0;
  lookaty=0.2;
  lookatz=-10.0;
  angle=0;
  memset(keys,0,sizeof(keys));


  glutInit(&argc, argv);
  glutInitWindowSize(window_width, window_height);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutCreateWindow("TDT4195");
  
  glClearColor(0, 0, 0, 0);
  
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  LoadGLTextures();
  glEnable(GL_TEXTURE_2D);

  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutKeyboardFunc(keydown);
  glutKeyboardUpFunc(keyup);
  glutReshapeFunc(reshape);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glLightfv(GL_LIGHT1,GL_AMBIENT,ambientlys);
  glLightfv(GL_LIGHT2,GL_DIFFUSE,diffuselys);
  glLightfv(GL_LIGHT1,GL_POSITION,lyspos);
  glLightfv(GL_LIGHT2,GL_POSITION,lyspos);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);

  glutMainLoop();
  
}


// Program entry point
int main(int argc, char **argv) {
	init(argc, argv);
}
