/* another test program that calls functions within a lua script
   that again calls c.
   warning, this page says that "priming runs" (run script itself) before
   calling functions is necessary
   http://www.troubleshooters.com/codecorn/lua/lua_c_calls_lua.htm */

#include <stdio.h>
#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int var1,var2;

/* takes 1 int i, performs var1+=i */
int func1(lua_State *L) {
	/* checks that number of parameters is 1 (or equivalently, number of elements
	   on the stack */
	if(lua_gettop(L)!=1) puts("wrong number of parameters"),exit(1);
	/* warning, stack is 1-indexed */
	var1+=strtol(lua_tostring(L,1),0,10);
	return 0;
}

/* takes 1 int i, performs var2+=i */
int func2(lua_State *L) {
	if(lua_gettop(L)!=1) puts("wrong number of parameters"),exit(1);
	var2+=strtol(lua_tostring(L,1),0,10);
	return 0;
}

int main() {
	lua_State *L;
	L=luaL_newstate();
	luaL_openlibs(L);
	lua_register(L,"func1",func1);
	lua_register(L,"func2",func2);
	if(luaL_loadfile(L,"lua2.lua")) printf("error loading file: %s\n",lua_tostring(L,-1)),exit(1);
	var1=var2=0;
	/* priming call which is apparently crucial */
	if(lua_pcall(L,0,0,0)) printf("error %s\n",lua_tostring(L,-1)),exit(1);
	/* call lua functions */
	lua_getglobal(L,"f1");
	if(lua_pcall(L,0,0,0)) printf("error %s\n",lua_tostring(L,-1)),exit(1);
	lua_getglobal(L,"f2");
	if(lua_pcall(L,0,0,0)) printf("error %s\n",lua_tostring(L,-1)),exit(1);
	printf("var1 %d, var2 %d\n",var1,var2);
	lua_close(L);
	return 0;
}
