/* given a directory path, read all file entries
   (including file name, date, other flags).
   this mess is supposed to work on most platforms.
   TODO support file date (meh, is troublesome on linux, need to call
	   stat on every file or something)
   TODO support file length on unix (meh, see above)
   TODO check on mac, dos
*/
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <dirent.h>
#endif

typedef long long ll;
typedef unsigned long long ull;

typedef struct {
#ifdef _WIN32
	HANDLE hfind;
	WIN32_FIND_DATA f;
#else
	DIR *d;
	struct dirent *f;
#endif
	int dir;	/* 1: dir, 0: no dir, -1: not supported */
	ull len;
	int nolen;	/* 1 if no support for filelen */
	char *s;
} dir_t;

#ifdef _WIN32
int dirwin(dir_t *h) {
	h->dir=(h->f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)?1:0;
	h->s=h->f.cFileName;
	h->len=h->f.nFileSizeHigh*(1ULL<<32)+h->f.nFileSizeLow;
	h->nolen=0;
	return 1;
}
#else
int dirunix(dir_t *h) {
	h->f=readdir(h->d);
	if(!h->f) return 0;
	h->s=h->f->d_name;
#ifdef _DIRENT_HAVE_D_TYPE
	h->dir=h->f->d_type==DT_DIR;
#else
	h->dir=-1;
#endif
	h->nolen=1;
	return 1;
}
#endif
/* return 1 if ok, 0 if error */
int findfirst(char *s,dir_t *h) {
#ifdef _WIN32
	h->hfind=FindFirstFile(s,&h->f);
	if(INVALID_HANDLE_VALUE==h->hfind) return 0;
	return dirwin(h);
#else
	h->d=opendir(s);
	if(!h->d) return 0;
	return dirunix(h);
#endif
}

/* return 0 if no more files in directory */
int findnext(dir_t *h) {
#ifdef _WIN32
	if(!FindNextFile(h->hfind,&h->f)) return 0;
	return dirwin(h);
#else
	return dirunix(h);
#endif
}

void findclose(dir_t *h) {
#ifdef _WIN32
	FindClose(h->hfind);
#else
	closedir(h->d);
#endif
}

int main(int argc,char **argv) {
	dir_t t;
#ifdef _WIN32
	if(!findfirst("*",&t)) { puts("no files"); return 0; }
#else
	if(!findfirst(".",&t)) { puts("no files"); return 0; }
#endif
	do {
		if(t.dir==0) printf("     ");
		else if(t.dir==1) printf("<dir>");
		else printf(" <?> ");
		#ifdef _WIN32
			if(t.nolen) printf(" %s\n",t.s);
			else printf(" %s %I64d\n",t.s,t.len);
		#else
			if(t.nolen) printf(" %s\n",t.s);
			else printf(" %s %lld\n",t.s,t.len);
		#endif
	} while(findnext(&t));
	findclose(&t);
  return 0;
}
