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
    lua��������ж����������lua_State

    ������ջ�Ĳ���api��ϰ
*/
int main()
{
    //��ͬ���½�lua����
    lua_State* L = luaL_newstate();
    //��lua������Ҫ�Ŀ�
    luaL_openlibs(L);
    //���ջ��Ϣ
    stackDump(L); 

    //��c++��������ջ�������ݣ��Ƚ������
    lua_pushstring(L, "aaaa");
    lua_pushinteger(L, 22222);
    lua_pushboolean(L, true);
    lua_pushlstring(L, "GGGGGGGGGGGGGG", 2);
    lua_pushstring(L, "cccc");
    stackDump(L);

    //����ջ��С
    //������ʾ������󣬸�����ʾ�Ƴ����� ��6��ʾ���ó���Ϊ6�� -6��ʾ�Ƴ�ջ��ǰ5��
    lua_settop(L, 6);  
    stackDump(L);
    //���ջ��С
    int num = lua_gettop(L);

    //�������� ������������Ƽ�ʹ������ =lua_settop(L, -(n)-1)
    lua_pop(L, 1);
    stackDump(L);

    //�Ƴ�����������
    //lua_remove(L, 2);
    //stackDump(L);

    //��ջ��Ԫ���ƶ���������
    //lua_insert(L, 2);
    //stackDump(L);

    //��λ������������������ջ����������ջ�ף��еĵͰ汾û�У�
    lua_rotate(L, 1, 2);
    stackDump(L);

    //��������Ԫ��ѹ��ջ��
    lua_pushvalue(L, -1);
    stackDump(L);

    //��ջ��Ԫ�ظ��ǵ�����λ��ջ����ջ
    lua_replace(L, 1);
    stackDump(L);

    //��ȡջ����λԪ������
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
    printf("%s\n", lua_typename(L, 6));   //1��ʾö��1����ջ����

    lua_close(L);
    return 0;
}