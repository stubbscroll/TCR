#include "SDL/SDL.h"
#include <stdio.h>

int main(int argc,char **argv) {
  SDL_Event event;
  if(SDL_Init(SDL_INIT_VIDEO)<0) exit(1);
  if(!SDL_SetVideoMode(512,512,8,SDL_SWSURFACE | SDL_ANYFORMAT)) exit(1);
  while(SDL_WaitEvent(&event)) {
    if(event.type==SDL_MOUSEBUTTONDOWN) {
      printf("mouse button: %d\n",event.button.button);
    } else if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE) break;
  }
  
  SDL_Quit();
  return 0;
}
