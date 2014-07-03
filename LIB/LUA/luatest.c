/* small sample program that runs a lua program that calls a C function.
   example stolen from http://csl.name/lua/, converted to C and upgraded to
   lua 5.2 very hastily by me */

#include <stdio.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

int my_function(lua_State *L) {
	int argc=lua_gettop(L),i;
	fprintf(stderr,"-- my_function() called with %d arguments:\n",argc);
	for(i=0;i<argc;i++) fprintf(stderr,"-- argument %d: %s\n",i+1,lua_tostring(L,i));
	lua_pushnumber(L,123); /* return value */
	return 1;              /* number of return values */
}

void report_errors(lua_State *L,int status) {
	if(status) {
		fprintf(stderr,"-- %s\n",lua_tostring(L,-1));
		lua_pop(L,1); /* remove error message */
	}
}

int main(int argc,char **argv) {
	lua_State *L;
	int i,s;
	for(i=1;i<argc;i++) {
		L=luaL_newstate();
		luaL_openlibs(L);
		/* make my_function() available to lua programs */
		lua_register(L,"my_function",my_function);
		s=luaL_loadfile(L,argv[i]);
		if(!s) s=lua_pcall(L,0,LUA_MULTRET,0);
		report_errors(L, s);
		lua_close(L);
	}
	return 0;
}
