#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int l_map(lua_State* L)
{
    int i, n;
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    n = lua_objlen(L, 1);

    for(i = 1; i <= n; ++i)
    {
        lua_pushvalue(L, 2);
        lua_rawgeti(L, 1, i);
        lua_call(L, 1, 1);
        lua_rawseti(L, 1, i);
    }

    return 0;
}

