#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>

#define BUF 1000000
char buffer[BUF];
int bptr;

/* need to collect everything in one huge buffer, as data can be split in the
   middle of a line */
/* chunk of data is in ptr, has n elements each of size size */
size_t webline(void *ptr,size_t size,size_t n,void *userdata) {
	int i;
	char *t=(char *)ptr;
	for(i=0;i<n;i++) buffer[bptr++]=t[i];
	buffer[bptr]=0;
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

/* small example showing how to open a webpage */
int main() {
	loadwebpage("http://www.pvv.org/~spaans/");
	puts(buffer);
	return 0;
}
