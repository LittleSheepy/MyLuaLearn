// LuaTest.cpp : �������̨Ӧ�ó������ڵ㡣
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
	int ret = lua_getstack(L, 2, &debug); // 0��pcall_callback_err_fun�Լ�, 1��error����, 2����������ĺ���
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
		/*��ӡ��������ʱ�Ķ�ջ��Ϣ,msg�Ǵ�����Ϣ,���ӵ���ӡ�Ķ�ջ��ǰ��
		1��ָ�ӵ�һ�㿪ʼ����
		�������֮��,lua_tostring(L,-1)���ɻ�ȡ�����Ķ�ջ�ʹ�����Ϣ.
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

	int stackTop = lua_gettop(pLuaState);//��ȡջ��������ֵ
	int nIdx = 0;
	int nType;
	std::string strErro = "[PrintLuaStack]";

	//������Ϣ
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

