#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define MAX_COLOR 255

typedef struct color
{
    char name[256];
    unsigned char red, green, blue;
}color;

void load_size(lua_State* L, const char* fname, int* w, int* h)
{
    if(luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
    {
        error(L, "cannot run config. file: %s", lua_tostring(L, -1));
    }
    lua_getglobal(L, "width");
    lua_getglobal(L, "height");
    if(!lua_isnumber(L, -2))
    {
        error(L, "width should be a number\n");
    }
    if(!lua_isnumber(L, -1))
    {
        error(L, "height should be a number\n");
    }
    *w = lua_tointeger(L, -2);
    *h = lua_tointeger(L, -1);
}

int getfield(lua_State* L, const char* key)
{
    int res;
    /*
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    */
    lua_getfield(L, -1, key);
    if(!lua_isnumber(L, -1))
    {
        error(L, "invalid component in background color");
    }
    res = (int)lua_tonumber(L, -1) * MAX_COLOR;
    lua_pop(L, 1);
    return res;
}

int setfield(lua_State* L, const char* index, int value)
{
    lua_pushnumber(L, (double)value/MAX_COLOR);
    lua_setfield(L, -2, index);
}

void load_color(lua_State* L, const char* fname, color* c)
{
    if(luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
    {
        error(L, "cannot run config. file: %s", lua_tostring(L, -1));
    }
    lua_getglobal(L, "background");
    if(!lua_istable(L, -1))
    {
        error(L, "background is not a table");
    }

    strcpy(c->name, "background");
    c->red = getfield(L, "r");
    c->green = getfield(L, "g");
    c->blue = getfield(L, "b");
}

void set_color(lua_State* L, color* c)
{
    lua_newtable(L);
    setfield(L, "r", c->red);
    setfield(L, "g", c->green);
    setfield(L, "b", c->blue);
    lua_setglobal(L, c->name);
}

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    int w, h;
    load_size(L, "size.txt", &w, &h);

    color c;
    load_color(L, "color.txt", &c);

    printf("w: %d\nh: %d\n", w, h);
    printf("r: %d\tg: %d\tb: %d\n", c.red, c.green, c.blue);
}