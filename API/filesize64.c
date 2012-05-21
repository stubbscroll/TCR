/*  return 64-bit filesize.
    different methods for windows and gcc, chosen with #ifdef */
/*  OK 12.07.2010 mingw(32) */
/*  OK 12.07.2010 vc2008(32) */
/*  OK 12.07.2010 vc2008(64) */
#ifdef _MSC_VER
  #include <windows.h>
  #include <winbase.h>
#else
  #include <sys/stat.h>
#endif
#include <stdio.h>

typedef long long ll;

/*  return filesize (supports >4gb), return -1 on error */
ll filesize(const char *s) {
#ifdef _MSC_VER
  HANDLE f=CreateFile(s,FILE_READ_ATTRIBUTES,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  LARGE_INTEGER z;
  if(INVALID_HANDLE_VALUE==f || !GetFileSizeEx(f,&z)) return -1;
  return z.QuadPart;
#else
  FILE *f=fopen64(s,"rb");
  ll l;
  if(!f) return -1;
  fseeko64(f,0,SEEK_END);
  l=ftello64(f);
  fclose(f);
  return l;
/*  the following out-commented method didn't work in mingw: */
/*  TODO try to make it work, since it is shorter and nicer */
/*  TODO eventually make it work under linux with #ifdef */
/*  struct stat64 z;
  if(-1==stat64(s,&z)) return -1;
  return z.st_size;*/
#endif
}

int main() {
  printf("%I64d\n",filesize("T:\\BURN\\fallout3goty\\Fallout 3 GOTYE Expansion disc DoeZ.iso"));
  return 0;
}
