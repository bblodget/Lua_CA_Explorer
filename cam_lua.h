#pragma once

/**
 * File:  ca_lua.h
 *
 * This header declares custom lua functions
 * That interface with the cam_machine.
 *
 * Author: Brandon Blodget
 *
 */


/*************************** Include Files ****************************/

extern "C"
{
#include "lua-5.4.4/src/lua.h"
#include "lua-5.4.4/src/lauxlib.h"
#include "lua-5.4.4/src/lualib.h"
}

/*********************** Function Declarations ************************/


int luaopen_libLuaCAM(lua_State* L);
int lua_print(lua_State* L);
int lua_set_color_table(lua_State* L);
int lua_update_rule(lua_State* L);
