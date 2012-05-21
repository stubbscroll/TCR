#include <stdio.h>
#include <SDL/SDL_net.h>

void error(char *s) {
  printf("%s",s);
  exit(1);
}

/*  wait for incoming connection */
int main(int argc,char**argv) {
  IPaddress ip,*remoteip;
  TCPsocket server,client;
  Uint32 ipaddr;
  Uint16 port;
  int len;
  char msg[1024];

  if(SDL_Init(0)==-1) error("error sdl init");
  if(SDLNet_Init()==-1) error("error sdlnet init");

  port=12346;
  if(SDLNet_ResolveHost(&ip,NULL,port)==-1) error("resolvehost error");

  /*  open server socket */
  if(!(server=SDLNet_TCP_Open(&ip))) error("couldn't open tcp");
  
  while(1) {
    client=SDLNet_TCP_Accept(server);
    if(!client) {
      printf("waiting for connection\n");
      SDL_Delay(200);
      continue;
    }
    remoteip=SDLNet_TCP_GetPeerAddress(client);
    if(!remoteip) error("error getting peer");
    
    ipaddr=SDL_SwapBE32(remoteip->host);
    printf("accepted connection from %d.%d.%d.%d port %u\n",
      ipaddr>>24,(ipaddr>>16)&255,(ipaddr>>8)&255,ipaddr&255,remoteip->port);
    
    len=SDLNet_TCP_Recv(client,msg,1024);
    SDLNet_TCP_Close(client);
    if(!len) error("didn't receive msg");
    printf("got: %s\n",msg);
    break;
  }
  SDLNet_Quit();
  SDL_Quit();
  return 0;
}
