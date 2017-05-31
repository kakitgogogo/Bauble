#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

void call_va(lua_State* L, const char* func, const char* sig, ...)
{
    va_list vl;
    int narg, nres;

    va_start(vl, sig);
    lua_getglobal(L, func);

    for(narg = 0; *sig; ++narg)
    {
        luaL_checkstack(L, 1, "too many argument");

        switch(*sig++)
        {
        case 'd':
            lua_pushnumber(L, va_arg(vl, double));
            break;
        case 'i':
            lua_pushnumber(L, va_arg(vl, int));
            break;
        case 's':
            lua_pushstring(L, va_arg(vl, char*));
        case '>':
            goto endargs;
        default:
            error(L, "invalid option(%c)", *(sig - 1));
        }
    }

endargs:

    nres = strlen(sig);

    if(lua_pcall(L, narg, nres, 0) != 0)
    {
        error(L, "error calling '%s': %s", func, lua_tostring(L, -1));
    }

    nres = -nres;

    while(*sig)
    {
        switch(*sig++)
        {
        case 'd':
            if(!lua_isnumber(L, nres))
            {
                error(L, "wrong result type");
            }
            *va_arg(vl, double*) = lua_tonumber(L, nres);
            break;
        case 'i':
            if(!lua_isnumber(L, nres))
            {
                error(L, "wrong result type");
            }
            *va_arg(vl, int*) = lua_tointeger(L, nres);
            break;
        case 's':
            if(!lua_isstring(L, nres))
            {
                error(L, "wrong result type");
            }
            *va_arg(vl, const char**) = lua_tostring(L, nres);
            break;
        default:
            error(L, "invalid option(%c)", *(sig-1));
        }
        ++nres;
    }

    va_end(vl);
}

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if(luaL_loadfile(L, "func.lua") || lua_pcall(L, 0, 0, 0))
    {
        error(L, "cannot run config. file: %s", lua_tostring(L, -1));
    }

    double z;

    call_va(L, "f", "dd>d", 1, 0, &z);

    printf("z: %f\n", z);

    return 0;
}