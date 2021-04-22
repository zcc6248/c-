#include "iostream"
#include "lua.hpp"
#include "tool.h"

/*lua��c++ͨ�Ź�ϵ��������luaAndC++.png
                   lua�����
                 -----------
                |  lua����  |
    lua         |  ����ջ   |      c++/c
                | ȫ�ֶ���  |
                 -----------
    lua��c++ͨ�ţ�ͨ��lua�е�����ջ���У��������վ��c++λ�ý��п���������
    c++�˻��lua�е����ݣ����Ƚ�lua�е�����ʹ��getϵ�к�����ȫ�ֶ��з�������ջ�У���ͨ��is/to���
    lua�˻��c++�е����ݣ�Ҳ��վ��c++�˿�����c++�����У�ʹ��pushϵ�к��������ݷ�������ջ�У���ʹ��setϵ�к����������ȫ�ֶ���
    lua��������ж������ͬ��lua_State

    c++/c ====�� lua
    c++��������lua�е���
*/

/*  ���޲���������ʹ�����ֽṹ����Ϊtypedef int (*lua_CFunction) (lua_State *L);
ÿ�ε��ö����½���һ��lua_State��ֻ�����˷����еĲ����ͷ���ֵ�ȡ�return0��ʾû�з���ֵ��ջ�����ݶ�Ϊ������
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
    //��ͬ���½�lua����
    lua_State* L = luaL_newstate();
    //��lua������Ҫ�Ŀ�
    luaL_openlibs(L);
    //���ջ��Ϣ
    stackDump(L);

    //��c++��������ջ�������ݣ��Ƚ������
#if 0 //lua�˻��ȫ�ֱ���
    lua_pushstring(L, "aaaa");
    lua_pushinteger(L, 22222);
    lua_pushboolean(L, true);
    lua_pushlstring(L, "GGGGGGGGGGGGGG", 2);
    lua_pushstring(L, "cccc");
    stackDump(L);

    //��ջ������������luaȫ�ֱ�������ջ���ڶ�������Ϊ��������
    lua_setglobal(L, "str1");
    lua_setglobal(L, "str2");
    lua_setglobal(L, "bool");
    lua_setglobal(L, "num");
    lua_setglobal(L, "str3");
    stackDump(L);

    //���벢����lua�ļ�
    //luaL_dofile(L, "main02.lua");
#endif

#if 1 //lua�˻�ñ�
    lua_newtable(L);
    stackDump(L);

    lua_pushstring(L, "name");
    lua_pushstring(L, "����");
    //��ջ��Ԫ����Ϊvalue����һ��Ԫ����Ϊkey,���õ�����ָ���table,�������ȫ����ջ
    lua_settable(L, 1);

    lua_pushboolean(L, false);  
    //��ջ��Ԫ����Ϊvalue����ͬ��lua_pushstring + lua_settable
    lua_setfield(L, 1, "type");
    stackDump(L);

    //��ջ������������luaȫ�ֱ�������ջ���ڶ�������Ϊ��������
    lua_setglobal(L, "tab");

    //���벢����lua�ļ�
    //luaL_dofile(L, "main02.lua");
#endif
   
#if 1 //lua�˻���޲��޷���ֵ����
    lua_pushcfunction(L, hello0);
    stackDump(L);

    //��ջ������������luaȫ�ֱ�������ջ���ڶ�������Ϊ��������
    lua_setglobal(L, "fun");
    stackDump(L);

    //lua�˻���в��޷���ֵ����
    lua_register(L, "fun2", hello2);

    //lua�˻���в��з���ֵ����
    lua_register(L, "fun3", hello3);

    //���벢����lua�ļ�
    luaL_dofile(L, "main02.lua");
    stackDump(L);
    /*
    ˵����ͬlua_State����ջ�ǲ�ͬ�ģ� lua_State�е�����ջ����ֻ�Ƕ�ȫ��ջ��ĳһ��ջ������������lua_State���൱��һ�����󡣲�ͬ�������Լ������ݡ�
    ���Կ������������ջ��ϢΪ�գ�����hello3�������ջ��Ϣ��4������ֱ��ʾ�ڴ�lua_State�����У���lua�˴����������������c++��push��ȥ����������
    */
#endif


    lua_close(L);
    return 0;
}