// LuaTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
using namespace std;
#include "lua.hpp"


#include <sstream>
#include <windows.h>
#include <string>


int pcall_callback_err_fun(lua_State* L)
{
	lua_Debug debug = {};
	int ret = lua_getstack(L, 2, &debug); // 0是pcall_callback_err_fun自己, 1是error函数, 2是真正出错的函数
	lua_getinfo(L, "Slnt", &debug);

	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	std::stringstream msg;
	msg << debug.short_src << ":line " << debug.currentline;
	if (debug.name != 0) {
		msg << "(" << debug.namewhat << " " << debug.name << ")";
	}

	msg << " [" << err << "]";
	lua_pushstring(L, msg.str().c_str());
	return 1;
}
static int traceback(lua_State *L)
{
	const char *msg = lua_tostring(L, -1);
	if (msg)
	{
		/*打印发生错误时的堆栈信息,msg是错误信息,附加到打印的堆栈最前端
		1是指从第一层开始回溯
		这个函数之后,lua_tostring(L,-1)即可获取完整的堆栈和错误信息.
		*/
		//printf(msg);
		luaL_traceback(L, L, msg, 1);
		//printf("\n%s\n", lua_tostring(L, -1));
	}
	else
	{
		lua_pushliteral(L, "no message");
	}
	return 0;
}

int PrintLuaStack(lua_State *L)
{
	lua_State* pLuaState = L;

	int stackTop = lua_gettop(pLuaState);//获取栈顶的索引值
	int nIdx = 0;
	int nType;
	std::string strErro = "[PrintLuaStack]";

	//错误信息
	for (nIdx = stackTop; nIdx > 0; --nIdx)
	{
		nType = lua_type(pLuaState, nIdx);
		strErro += lua_typename(pLuaState, nType);
		strErro += lua_tostring(pLuaState, nIdx);
		strErro += "\n";
	}
	//debug.traceback
	lua_getglobal(pLuaState, "debug");
	lua_getfield(pLuaState, -1, "traceback");
	int iError = lua_pcall(pLuaState,    //VMachine    
		0,    //Argument Count    
		1,    //Return Value Count    
		0);
	const char* sz = lua_tostring(pLuaState, -1);
	strErro += sz;
	printf(strErro.c_str());
	return 0;
}

int main()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	int ret = 0;
	lua_pushcfunction(L, PrintLuaStack);
	int pos_err = lua_gettop(L);
	printf("%d\n", pos_err);
	int iError = luaL_loadfile(L, "main.lua");
	printf("iError is %d\n",iError);
	ret = lua_pcall(L, 0, 0, 0);

	int rst = lua_getglobal(L, "test");
	if (rst != LUA_TFUNCTION) {
		printf("not func");
		system("pause");
		return 1;
	}
	ret = lua_pcall(L, 0, 0, pos_err);
	lua_close(L);
	system("pause");
    return 0;
}

