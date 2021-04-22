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

    lua ====�� c++/c
    lua��������c++�е���
*/


int main()
{
    //��ͬ���½�lua����
    lua_State* L = luaL_newstate();
    //��lua������Ҫ�Ŀ�
    luaL_openlibs(L);
    //���ջ��Ϣ
    stackDump(L);

    //���벢����lua�ļ�
    luaL_dofile(L, "main03.lua");

    //��lua��������ջ�������ݣ��Ƚ������
#if 1 //c++�˻��ȫ�ֱ���

    //��ȫ�ֱ�����������ջ���ڶ�������Ϊlua��ȫ�ֱ���������
    lua_getglobal(L, "type");
    lua_getglobal(L, "tab");
    stackDump(L);
#endif

#if 1 //lua�˻�ñ�
    lua_pushstring(L, "name");
    lua_pushstring(L, "age");

    //��ջ��Ԫ����Ϊkey��ȥ������ָ��table���ҵ�value�����Ƶ�ջ��
    lua_gettable(L, -3);
    //stackDump(L);
    printf("%s\n", lua_tostring(L, -1));

    lua_pop(L, 2);
    //stackDump(L);
    //��ͬ��lua_pushstring + lua_gettable
    lua_getfield(L, 2, "name");
    printf("name:%s\n", lua_tostring(L, -1));
    //stackDump(L);

    printf("type:%s\n", lua_toboolean(L, 1) ? "true" : "false");
#endif

#if 1 //lua�˻�ñ��к���
    lua_getfield(L, 2, "fun");
    //��Ϊ����ִ�еĲ���
    lua_pushstring(L, "c++");

    //��������ջ�Ϻ�������һ������Ϊ�˺������еĲ����������������ֱ��ջ������Ϊ������
    //�ڶ�������Ϊ����ֵ����
    lua_pcall(L, 1, 5, 0);
    stackDump(L);

    //��luaȫ�ֱ�����������ջ   �޲��޷���ֵ
    lua_getglobal(L, "hello0");
    lua_call(L, 0, 0);
    stackDump(L);

    //��luaȫ�ֱ�����������ջ   �в��޷���ֵ
    lua_getglobal(L, "hello2");
    lua_pushstring(L, "c++");
    lua_call(L, 1, 0);

    //��luaȫ�ֱ�����������ջ   �в��з���ֵ
    lua_getglobal(L, "hello3");
    //stackDump(L);
    lua_pushstring(L, "c++");
    lua_pushstring(L, "lua");
    //����ֵ������ջ
    lua_call(L, 2, 2);
    stackDump(L);
#endif

    lua_close(L);
    return 0;
}