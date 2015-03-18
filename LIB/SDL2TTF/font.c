#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
TTF_Font *font;

SDL_Texture *rendertext(char *s,SDL_Color col) {
	SDL_Surface *surface=TTF_RenderText_Blended(font,s,col);
	SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,surface);
	SDL_FreeSurface(surface);
	return texture;
}

int main(int argc,char **argv) {
	SDL_Color white={255,255,255,255};
	SDL_Event event;
	SDL_Rect dest;
	int height,width;
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	window=SDL_CreateWindow("Font",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
	                        640,400,0);
	renderer=SDL_CreateRenderer(window,-1,0);
	if(!(font=TTF_OpenFont("c:/windows/fonts/times.ttf",40))) return 1;
	SDL_Texture *texture=rendertext("Whee, hello world!",white);
	SDL_QueryTexture(texture,NULL,NULL,&width,&height);
	while(1) {
		while(!SDL_PollEvent(&event)) SDL_Delay(10);
		if(event.type==SDL_QUIT) break;
		if(event.type==SDL_KEYDOWN) break;
		if(event.type==SDL_MOUSEBUTTONDOWN) break;
		SDL_RenderClear(renderer);
		dest.x=(640-width)/2;
		dest.y=(400-height)/2;
		dest.w=width;
		dest.h=height;
		SDL_RenderCopy(renderer,texture,0,&dest);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
