#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

static int counter(lua_State* L)
{
    int val = lua_tointeger(L, lua_upvalueindex(1));
    lua_pushinteger(L, ++val);
    lua_pushvalue(L, -1);
    lua_replace(L, lua_upvalueindex(1));
    return 1;
}

int newCounter(lua_State* L)
{
    lua_pushinteger(L, 0);
    lua_pushcclosure(L, &counter, 1);
    return 1;
}

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    newCounter(L);

    counter(L);
}