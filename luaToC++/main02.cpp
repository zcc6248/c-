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

    c++/c ====》 lua
    c++的数据在lua中调用
*/

/*  有无参数都必须使用这种结构，因为typedef int (*lua_CFunction) (lua_State *L);
每次调用都会新建立一个lua_State，只保留此方法中的参数和返回值等。return0表示没有返回值，栈中数据都为脏数据
int hello(lua_State* L)
{
    return 0;
}
*/

int hello0(lua_State* L)
{
    printf("hello0 no param\n");
    return 0;
}

int hello2(lua_State *L) 
{
    printf("hello2 two param\n");
    printf("%s\n", lua_tostring(L, -1));
    printf("%d\n", lua_tointeger(L, -2));
    return 0;
}

int hello3(lua_State* L)
{
    printf("hello2 two param\n");
    const char* str = lua_tostring(L, -1);
    int num =  lua_tointeger(L, -2);
    lua_pushstring(L, str);
    lua_pushinteger(L, num + 100);
    stackDump(L);
    return 2;
}

int main()
{
    //等同于新建lua程序
    lua_State* L = luaL_newstate();
    //打开lua运行需要的库
    luaL_openlibs(L);
    //输出栈信息
    stackDump(L);


    luaL_newmetatable(L, "aaaaaa");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    lua_newuserdata(L, 10);
    luaL_getmetatable(L, "aaaaaa");
    lua_setmetatable(L, -2);

    stackDump(L);
    //从c++，往虚拟栈中推数据（先进后出）
#if 0 //lua端获得全局变量
    lua_pushstring(L, "aaaa");
    lua_pushinteger(L, 22222);
    lua_pushboolean(L, true);
    lua_pushlstring(L, "GGGGGGGGGGGGGG", 2);
    lua_pushstring(L, "cccc");
    stackDump(L);

    //从栈顶将数据推入lua全局变量并出栈（第二个参数为变量名）
    lua_setglobal(L, "str1");
    lua_setglobal(L, "str2");
    lua_setglobal(L, "bool");
    lua_setglobal(L, "num");
    lua_setglobal(L, "str3");
    stackDump(L);

    //编译并运行lua文件
    //luaL_dofile(L, "main02.lua");
#endif

#if 0 //lua端获得表
    lua_newtable(L);
    stackDump(L);

    lua_pushstring(L, "name");
    lua_pushstring(L, "张三");

    //把栈顶元素做为value，下一个元素做为key,设置到索引指向的table,最后两个全部出栈
    lua_settable(L, 1);

    lua_pushboolean(L, false);  
    //将栈顶元素做为value。等同于lua_pushstring + lua_settable
    lua_setfield(L, 1, "type");
    stackDump(L);

    //从栈顶将数据推入lua全局变量并出栈（第二个参数为变量名）
    lua_setglobal(L, "tab");

    //编译并运行lua文件
    //luaL_dofile(L, "main02.lua");
#endif
   
#if 0 //lua端获得无参无返回值函数
    lua_pushcfunction(L, hello0);
    stackDump(L);

    //从栈顶将数据推入lua全局变量并出栈（第二个参数为变量名）
    lua_setglobal(L, "fun");
    stackDump(L);

    //lua端获得有参无返回值函数
    lua_register(L, "fun2", hello2);

    //lua端获得有参有返回值函数
    lua_register(L, "fun3", hello3);

    //编译并运行lua文件
    luaL_dofile(L, "main02.lua");
    stackDump(L);
    /*
    说明不同lua_State虚拟栈是不同的， lua_State中的虚拟栈，其只是对全局栈中某一段栈的索引，所以lua_State就相当于一个对象。不同对象有自己的数据。
    可以看出上面输出的栈信息为空，但在hello3中输出的栈信息有4个，其分别表示在此lua_State对象中，从lua端传入的两个参数和在c++端push进去的两个参数
    */
#endif


    lua_close(L);
    return 0;
}