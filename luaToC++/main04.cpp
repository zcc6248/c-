#include "iostream"
#include "lua.hpp"
#include "tool.h"

/*lua和c++通信关系，具体如luaAndC++.png
				   lua虚拟机
				 -----------
				|  lua程序  |
	lua         |  虚拟栈   |      c++/c
				| 全局队列  |
				 -----------
	lua和c++通信，通过lua中的虚拟栈进行，设计者是站在c++位置进行开发的所以
	c++端获得lua中的数据，首先将lua中的数据使用get系列函数从全局队列放入虚拟栈中，再通过is/to获得
	lua端获得c++中的数据，也是站在c++端看，在c++代码中，使用push系列函数将数据放入虚拟栈中，再使用set系列函数将其放入全局队列
	lua程序可以有多个，等同于lua_State

	c++函数批量注册
*/

/*  有无参数都必须使用这种结构，因为typedef int (*lua_CFunction) (lua_State *L);
每次调用都会新建立一个lua_State，只保留此方法中的参数和返回值等。return0表示没有返回值，栈中数据都为脏数据
int hello(lua_State* L)
{
	return 0;
}
*/


//要想注册进 lua, 
//函数的定义为 typedef int (*lua_CFunction)(lua_State* L) 
int printHello(lua_State* l) {
	lua_pushstring(l, "hello lua");
	//返回值代表向栈内压入的元素个数 
	return 1;
}

int foo(lua_State* l)
{ 
	//获得 Lua 传递过来的参数个数 
	int n = lua_gettop(l);
	if (n != 0)
	{
		//获得第一个参数 
		long i = lua_tonumber(l, 1) + 1;
		//将传递过来的参数加一以后最为返回值传递回去 
		lua_pushnumber(l, int(i));
		return 1;
	}
	return 0;
}

//相加 
int add(lua_State* l)
{
	int n = lua_gettop(l);
	int sum = 0;
	for (int i = 0; i < n; i++) 
	{
		sum += lua_tonumber(l, i + 1);
	}
	if (n != 0) 
	{ 
		lua_pushnumber(l, sum); 
		return 1; 
	}
	return 0;
}

//把需要用到的函数都放到注册表中, 统一进行注册 
const luaL_Reg lib[] = 
{ 
	{"printHello", printHello}, 
	{"foo", foo}, 
	{"add", add}, 
	{NULL, NULL} 
};

extern "C" {
	int luaopen_mylib(lua_State* L) {
		luaL_newlib(L, lib);
		return 1;
	}
}