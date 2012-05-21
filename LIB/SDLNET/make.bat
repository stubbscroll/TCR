gcc -o server.exe server.c -O2 -Wall -lmingw32 -lsdlmain -lsdl -lsdl_net
gcc -o client.exe client.c -O2 -Wall -lmingw32 -lsdlmain -lsdl -lsdl_net

@rem main must have defined as int main(int,char **)
