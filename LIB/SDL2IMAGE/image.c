/* example: load image from file and display it */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

int main(int argc,char **argv) {
	SDL_Event event;
	if(argc<2) return 0; /* need filename */
	SDL_Init(SDL_INIT_VIDEO);
	/* initialise with jpg/png/tif so that stuff doesn't take long time later */
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF);
	window=SDL_CreateWindow("Font",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
	                        640,400,0);
	renderer=SDL_CreateRenderer(window,-1,0);
	texture=IMG_LoadTexture(renderer,argv[1]);
	if(!texture) return 1;
	while(1) {
		while(!SDL_PollEvent(&event)) SDL_Delay(10);
		if(event.type==SDL_QUIT) break;
		if(event.type==SDL_KEYDOWN) break;
		if(event.type==SDL_MOUSEBUTTONDOWN) break;
		SDL_RenderClear(renderer);
		/* just scale it to fill window */
		SDL_RenderCopy(renderer,texture,0,0);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
