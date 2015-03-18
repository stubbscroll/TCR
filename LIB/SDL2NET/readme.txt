examples from http://www.libsdl.org/projects/SDL_net/docs/demos/

did some cleanup: added stdio to prevent a barrage of compiler warnings about
implicit declaration of printf and changed sdl #includes to fit my install.

i couldn't compile tcpmulticlient and tcpmulticlientthreaded on mingw64, gcc
says "undefined reference to __imp_select and __WSAFDIsSet", which seem to be
linux kernel functions after a sloppy internet search.
