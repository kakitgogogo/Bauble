#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

static int l_split(lua_State* L)
{
    const char* s = luaL_checkstring(L, 1);
    const char* sep = luaL_checkstring(L, 2);
    const char* e;
    int i = 1;

    lua_newtable(L);

    while((e = strchr(s, *sep)) != NULL)
    {
        lua_pushlstring(L, s, s-e);
        lua_rawseti(L, -2, i++);
        s = e + 1;
    }

    lua_pushstring(L, s);
    lua_rawseti(L, -2, i);

    return 1;
}