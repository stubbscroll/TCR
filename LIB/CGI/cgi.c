#include <stdio.h>
#include <stdlib.h>

int main() {
	char *data=getenv("QUERY_STRING");
	puts("Content-type: text/html\n");
	puts("<html><body><h1>good evening</h1>");
	printf("you entered '%s'\n",data);
	puts("</body></html>");
	return 0;
}
