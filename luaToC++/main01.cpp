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
    lua程序可以有多个，等用于lua_State

    对虚拟栈的操作api练习
*/
int main()
{
    //等同于新建lua程序
    lua_State* L = luaL_newstate();
    //打开lua运行需要的库
    luaL_openlibs(L);
    //输出栈信息
    stackDump(L); 

    //从c++，往虚拟栈中推数据（先进后出）
    lua_pushstring(L, "aaaa");
    lua_pushinteger(L, 22222);
    lua_pushboolean(L, true);
    lua_pushlstring(L, "GGGGGGGGGGGGGG", 2);
    lua_pushstring(L, "cccc");
    stackDump(L);

    //设置栈大小
    //正数表示保留多大，负数表示移除多少 如6表示设置长度为6， -6表示移除栈顶前5个
    lua_settop(L, 6);  
    stackDump(L);
    //获得栈大小
    int num = lua_gettop(L);

    //弹出数据 负数不好理解推荐使用正数 =lua_settop(L, -(n)-1)
    lua_pop(L, 1);
    stackDump(L);

    //移除索引处数据
    //lua_remove(L, 2);
    //stackDump(L);

    //将栈顶元素移动到索引处
    //lua_insert(L, 2);
    //stackDump(L);

    //移位，从索引处。正数向栈顶，负数向栈底（有的低版本没有）
    lua_rotate(L, 1, 2);
    stackDump(L);

    //将索引处元素压入栈顶
    lua_pushvalue(L, -1);
    stackDump(L);

    //将栈顶元素覆盖到索引位，栈顶出栈
    lua_replace(L, 1);
    stackDump(L);

    //获取栈索引位元素类型
    //#define LUA_TNONE		(-1)
    //#define LUA_TNIL		0
    //#define LUA_TBOOLEAN		1
    //#define LUA_TLIGHTUSERDATA	2
    //#define LUA_TNUMBER		3
    //#define LUA_TSTRING		4
    //#define LUA_TTABLE		5
    //#define LUA_TFUNCTION		6
    //#define LUA_TUSERDATA		7
    //#define LUA_TTHREAD		8
    printf("%d\n", lua_type(L, 1));
    printf("%s\n", lua_typename(L, 6));   //1表示枚举1不是栈索引

    lua_close(L);
    return 0;
}