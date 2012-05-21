/*  given a directory path, read all file entries
    (including file name, date, other flags)
    sadly, it is apparently impossible to do this in a portable way
*/
#include <stdio.h>
#include <stdlib.h>

/*  the windows way */

#ifdef _WIN32
#include <windows.h>
void readdir(char *s) {
  HANDLE hfind;
  WIN32_FIND_DATA f;
  char t[1024];
  strcpy(t,s);
  strcat(t,"*");
  hfind=FindFirstFile(t,&f);
  if(INVALID_HANDLE_VALUE==hfind) {
    puts("error reading dir");
    exit(1);
  }
  do {
    if(f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
      printf("<DIR> %s\n",f.cFileName);
    } else {
      printf("      %s %I64d\n",f.cFileName,(long long)f.nFileSizeHigh*(1LL<<32)+f.nFileSizeLow);
    }
  } while(FindNextFile(hfind,&f));
  printf("error %u\n",(unsigned)GetLastError());
  FindClose(hfind);
}
#else
  /*  TODO linuks */
#endif

/*  the linux way (let's pray it works on mac as well) */
/*  TODO */

int main(int argc,char **argv) {
readdir("C:\\Windows\\*.exe");
  if(argc<2) puts("usage: readdir path (end with \\)\n");
  else readdir(argv[1]);
  return 0;
}
