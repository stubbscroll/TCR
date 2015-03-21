/* lazyfoo sdl tutorial 51: sdl and modern opengl */

#include <SDL2/SDL.h>
#include <GL/glew.h>         /* include before gl or it will complain */
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>

const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;

SDL_Window *window;
SDL_GLContext *context;

int renderquad=1;             /* render flag */

GLuint programid=0;           /* graphics program */
GLint vertexpos2Dlocation=-1;
GLuint VBO=0;
GLuint IBO=0;

void error(char *s,...) {
	static char t[999];
	va_list argptr;
	va_start(argptr,s);
	vsnprintf(t,999,s,argptr);
	va_end(argptr);
	puts(t);
	exit(1);
}

void printprogramlog(GLuint program) {
	/* make sure name is a program */
	if(glIsProgram(program)) {
		int infologlength=0;
		int maxlength;
		char *infolog;
		glGetProgramiv(program,GL_INFO_LOG_LENGTH,&maxlength); /* get info string length */
		infolog=malloc(maxlength);                             /* allocate string */
		glGetProgramInfoLog(program,maxlength,&infologlength,infolog); /* get info log */
		if(infologlength>0) printf("%s\n",infolog);
		free(infolog);                                         /* deallocate string */
	} else printf("name %d is not a program\n",program);
}

void printshaderlog(GLuint shader) {
	/* make sure name is a shader */
	if(glIsShader(shader)) {
		int infologlength = 0;
		int maxlength;
		char *infolog;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&maxlength ); /* get info string length */
		infolog=malloc(maxlength);                            /* allocate string */
		glGetShaderInfoLog(shader,maxlength,&infologlength,infolog); /* get info log */
		if(infologlength>0) printf("%s\n",infolog);
		free(infolog);                                        /* deallocate string */
	} else printf("name %d is not a shader\n",shader);
}

void initGL() {
	GLuint vertexshader;
	GLuint fragmentshader;
	GLint shadercompiled=GL_FALSE,programsuccess=GL_TRUE;
	const GLchar *vertexshadersource[]={
		"#version 140\n"
		"in vec2 LVertexPos2D;"
		"void main() {"
		"  gl_Position=vec4(LVertexPos2D.x,LVertexPos2D.y,0,1);"
		"}"
	};
	const GLchar* fragmentshadersource[]={
		"#version 140\n"
		"out vec4 LFragment;"
		"void main() {"
		"  LFragment=vec4(1.0,1.0,1.0,1.0);"
		"}"
	};	
	GLfloat vertexdata[]={-0.5f,-0.5f,0.5f,-0.5f,0.5f,0.5f,-0.5f,0.5f};
	GLuint indexdata[]={0,1,2,3};


	programid=glCreateProgram();                            /* generate program */
	vertexshader=glCreateShader(GL_VERTEX_SHADER);          /* create vertex shader */
	glShaderSource(vertexshader,1,vertexshadersource,NULL); /* set vertex source */
	glCompileShader(vertexshader);                          /* compile vertex source */
	/* check vertex shader for errors */
	glGetShaderiv(vertexshader,GL_COMPILE_STATUS,&shadercompiled);
	if(shadercompiled!=GL_TRUE) {
		printshaderlog(vertexshader);
		error("unable to compile vertex shader %d",vertexshader);
	}
	glAttachShader(programid,vertexshader);                 /* attach vertex shader to program */
	fragmentshader=glCreateShader(GL_FRAGMENT_SHADER);      /* create fragment shader */
	glShaderSource(fragmentshader,1,fragmentshadersource,NULL); /* set fragment source */
	glCompileShader(fragmentshader);                        /* compile fragment source */
	/* check fragment shader for errors */
	shadercompiled=GL_FALSE;
	glGetShaderiv(fragmentshader,GL_COMPILE_STATUS,&shadercompiled);
	if(shadercompiled!=GL_TRUE) {
		printshaderlog(fragmentshader);
		error("unable to compile fragment shader %d",fragmentshader);
	}
	glAttachShader(programid,fragmentshader );              /* attach fragment shader to program */
	glLinkProgram(programid);                               /* link program */
	/* check for errors */
	glGetProgramiv(programid,GL_LINK_STATUS,&programsuccess);
	if(programsuccess!=GL_TRUE) {
		printprogramlog(programid);
		error("error linking program %d",programid);
	}
	vertexpos2Dlocation=glGetAttribLocation(programid,"LVertexPos2D"); /* get vertex attribute location */
	if(vertexpos2Dlocation==-1) error("LVertexPos2D is not a valid glsl program variable");
	glClearColor(0.f,0.f,0.f,1.f);
	/* create VBO */
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,2*4*sizeof(GLfloat),vertexdata,GL_STATIC_DRAW);
	/* create IBO */
	glGenBuffers(1,&IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,4*sizeof(GLuint),indexdata,GL_STATIC_DRAW);
}

void init() {
	if(SDL_Init(SDL_INIT_VIDEO)<0) error("error sdl init: %s",SDL_GetError());
	/* use opengl 3.1 core */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	window=SDL_CreateWindow("SDL Tutorial",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
	                        SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	if(!window) error("couldn't open window: %s",SDL_GetError());
	context=SDL_GL_CreateContext(window);
	if(!context) error("couldn't open context: %s",SDL_GetError());
	/* initialize glew */
	glewExperimental=GL_TRUE;
	GLenum glewError=glewInit();
	if(glewError!=GLEW_OK) error("error glew init: %s",glewGetErrorString(glewError));
	if(SDL_GL_SetSwapInterval(1)<0) error("unable to set vsync: %s",SDL_GetError());
	initGL();
}

void close() {
	glDeleteProgram(programid);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void handlekeys(unsigned char key) {
	/* toggle quad */
	if(key=='q') renderquad^=1;
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);         /* clear colour buffer */
	/* render quad */
	if(renderquad ) {
		glUseProgram(programid);            /* bind program */
		glEnableVertexAttribArray(vertexpos2Dlocation); /* enable vertex position */
		/* set vertex data */
		glBindBuffer(GL_ARRAY_BUFFER,VBO );
		glVertexAttribPointer(vertexpos2Dlocation,2,GL_FLOAT,GL_FALSE,2*sizeof(GLfloat),NULL);
		/* set index data and render */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
		glDrawElements(GL_TRIANGLE_FAN,4,GL_UNSIGNED_INT,NULL);
		glDisableVertexAttribArray(vertexpos2Dlocation ); /* disable vertex position */
		glUseProgram(0);                    /* unbind program */
	}
}

int main(int argc,char **argv) {
	SDL_Event e;
	int quit=0;
	init();
	SDL_StartTextInput();             /* enable text input */
	while(!quit) {
		while(SDL_PollEvent(&e)) {
			if(e.type==SDL_QUIT) quit=1;
			else if(e.type==SDL_TEXTINPUT) handlekeys(e.text.text[0]);
		}
		render();
		SDL_GL_SwapWindow(window);      /* update screen */
	}
	SDL_StopTextInput();
	close();
	return 0;
}
