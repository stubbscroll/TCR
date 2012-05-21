#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int main(int argc,char **argv) {
  SDL_Event event;
  SDL_Surface *window,*bilde;
  SDL_Rect dest={0,0,0,0};
  if(SDL_Init(SDL_INIT_VIDEO)<0) return 0;
  window=SDL_SetVideoMode(800,600,32,SDL_SWSURFACE);
  bilde=IMG_Load("bilde.jpg");
  if(!bilde) return 0;
  SDL_BlitSurface(bilde,NULL,window,&dest);
  SDL_UpdateRect(window,0,0,bilde->w,bilde->h);
  SDL_FreeSurface(bilde);
  while(SDL_WaitEvent(&event)>=0) if(event.type==SDL_QUIT) break;
  SDL_Quit();
  return 0;
}
