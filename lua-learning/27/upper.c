#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

static int str_upper(lua_State* L)
{
    size_t l, i;
    luaL_Buffer b;
    const char* s = luaL_checklstring(L, 1, &l);
    luaL_buffinit(L, &b);

    for(i = 0; i < l; ++i)
    {
        luaL_addchar(&b, toupper((unsigned char)(s[i])));
    }
    luaL_pushresult(&b);
    return 1;
}