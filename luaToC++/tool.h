#pragma once
#include "lua.hpp"


static void stackDump(lua_State* L)
{
	static int count = 0;
	printf("==============begin================%d\n", count);
	int i = 0;
	int top = lua_gettop(L);
	for (i = top; i > 0; --i) // for (i = top; i >= 1; --i)
	{
		int t = lua_type(L, i);
		switch (t)
		{
		case LUA_TSTRING:
			printf("%d, \"%s\" ", i, lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			printf("%d, %s", i, lua_toboolean(L, i) ? "true " : "false ");
			break;
		case LUA_TNUMBER:
			printf("%d, %g ", i, lua_tonumber(L, i));
			break;
		default:
			printf("%d, %s ",i, lua_typename(L, t));
			break;
		}
		finally:
			putchar(10);
	}
	printf("===============end=================%d\n", count++);
}

