#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <limits.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define BITS_PER_WORD (sizeof(unsigned int))
#define I_WORD ((unsigned int)(i) / BITS_PER_WORD)
#define I_BIT (1 << (unsigned int)(i) % BITS_PER_WORD)

typedef struct NumArray
{
    int size;
    unsigned int values;
}NumArray;

static int newarray(lua_State* L)
{
    int i, n;
    size_t nbytes;
    NumArray* a;

    n = luaL_checkint(L, 1);
    luaL_argcheck(L, n >= 1, 1, "")；
    nbytes = sizeof(NumArray) + I_WORD(n-1)*sizeof(unsigned int);
}