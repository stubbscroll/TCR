/* load a file from web */

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>

#define BUF 1000000
unsigned char buffer[BUF];
int bptr;

/*  need to collect everything in one huge buffer, as data can be split in the
    middle of a line */
size_t webline(void *ptr,size_t size,size_t n,void *userdata) {
  int i;
  unsigned char *t=(unsigned char *)ptr;
	memcpy(buffer+bptr,t,n);
	bptr+=n;
  return n;
}

void loadwebpage(char *url) {
  CURL *curl=curl_easy_init();
  CURLcode res;
  curl_easy_setopt(curl,CURLOPT_URL,url);
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,webline);
  bptr=0;
  res=curl_easy_perform(curl);
  curl_easy_cleanup(curl);
}

/*  small example showing how to open a webpage */
int main() {
	FILE *f;
  loadwebpage("http://community.topcoder.com/i/header_statistics.gif");
	f=fopen("bilde.gif","wb");
	if(!f) {
		printf("error opening file\n");
		exit(1);
	}
	fwrite(buffer,1,bptr,f);
	if(fclose(f)) {
		printf("error saving and closing file\n");
		exit(1);
	}
  return 0;
}
