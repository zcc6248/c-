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

    lua ====》 c++/c
    lua的数据在c++中调用
*/


int main()
{
    //等同于新建lua程序
    lua_State* L = luaL_newstate();
    //打开lua运行需要的库
    luaL_openlibs(L);
    //输出栈信息
    stackDump(L);

    //编译并运行lua文件
    luaL_dofile(L, "main03.lua");

    //从lua，往虚拟栈中推数据（先进后出）
#if 1 //c++端获得全局变量

    //将全局变量推入虚拟栈，第二个参数为lua中全局变量的名称
    lua_getglobal(L, "type");
    lua_getglobal(L, "tab");
    stackDump(L);
#endif

#if 1 //lua端获得表
    lua_pushstring(L, "name");
    lua_pushstring(L, "age");

    //将栈顶元素做为key，去索引所指的table中找到value，复制到栈顶
    lua_gettable(L, -3);
    //stackDump(L);
    printf("%s\n", lua_tostring(L, -1));

    lua_pop(L, 2);
    //stackDump(L);
    //等同于lua_pushstring + lua_gettable
    lua_getfield(L, 2, "name");
    printf("name:%s\n", lua_tostring(L, -1));
    //stackDump(L);

    printf("type:%s\n", lua_toboolean(L, 1) ? "true" : "false");
#endif

#if 1 //lua端获得表中函数
    lua_getfield(L, 2, "fun");
    //作为函数执行的参数
    lua_pushstring(L, "c++");

    //运行虚拟栈上函数，第一个参数为此函数运行的参数个数，如果有则分别从栈顶依次为参数，
    //第二个参数为返回值个数
    lua_pcall(L, 1, 5, 0);
    stackDump(L);

    //将lua全局变量推入虚拟栈   无参无返回值
    lua_getglobal(L, "hello0");
    lua_call(L, 0, 0);
    stackDump(L);

    //将lua全局变量推入虚拟栈   有参无返回值
    lua_getglobal(L, "hello2");
    lua_pushstring(L, "c++");
    lua_call(L, 1, 0);

    //将lua全局变量推入虚拟栈   有参有返回值
    lua_getglobal(L, "hello3");
    //stackDump(L);
    lua_pushstring(L, "c++");
    lua_pushstring(L, "lua");
    //返回值依次入栈
    lua_call(L, 2, 2);
    stackDump(L);
#endif

    lua_close(L);
    return 0;
}