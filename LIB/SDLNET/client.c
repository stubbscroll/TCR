#include <stdio.h>
#include <stdlib.h>
#include <SDL/sdl.h>
#include <SDL/SDL_net.h>

void error(char *s) {
  printf("%s",s);
  exit(1);
}

/*  try to connect to server */
int main(int argc,char**argv) {
  IPaddress ip;
  TCPsocket sock;
  char msg[1024];
  int len,res;
  Uint16 port;

  if(SDL_Init(0)==-1) error("sdl init error");
  if(SDLNet_Init()==-1) error("sdlnet init error");
  port=12346;
  if(SDLNet_ResolveHost(&ip,"10.0.0.5",port)==-1) error("sdlnet resolvehost failed");

  sock=SDLNet_TCP_Open(&ip);
  if(!sock) error("tcp open failed");
  strcpy(msg,"dette er en jall");
  len=strlen(msg);
  res=SDLNet_TCP_Send(sock,msg,len+1);
  if(res<len) error("error on send");
	SDLNet_TCP_Close(sock);
	SDLNet_Quit();
	SDL_Quit();
  return 0;
}
