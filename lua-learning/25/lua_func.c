#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

double f(lua_State* L, double x, double y)
{
    double z;

    lua_getglobal(L, "f");
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);

    if(lua_pcall(L, 2, 1, 0) != 0)
    {
        error(L, "error running function 'f': %s", lua_tostring(L, -1));
    }

    if(!lua_isnumber(L, -1))
    {
        error(L, "function 'f' must return a number");
        z = lua_tonumber(L, -1);
        lua_pop(L, 1);
        return z;
    }
}

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if(luaL_loadfile(L, "func.lua") || lua_pcall(L, 0, 0, 0))
    {
        error(L, "cannot run config. file: %s", lua_tostring(L, -1));
    }

    double z = f(L, 0, 1);
    printf("z: %f\n", z);

    return 0;
}