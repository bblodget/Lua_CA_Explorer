/**
 * File:  ca_lua.cpp
 *
 * This file defines custom lua functions
 * That interface with the cam_machine.
 * 
 * Author: Brandon Blodget
 * 
 */

/************************** Include Files *****************************/


#include "cam_lua.h"
#include "cam_machine.h"
#include "olcPixelGameEngine.h"
#include <iostream>

/*********************** Function Definitions ************************/

// Open the CAM library
int luaopen_libLuaCAM(lua_State* L) 
{
	lua_register(L, "print", lua_print);
	lua_register(L, "set_color_table", lua_set_color_table);
	lua_register(L, "update_rule", lua_update_rule);
	return 1;
}

/*
 * This version of the print statement uses std::cout
 * so it can be capture to the OLC console output.
 */
int lua_print(lua_State* L) 
{
	int n = lua_gettop(L);  /* number of arguments */
	int i;
	for (i = 1; i <= n; i++) {  /* for each argument */
		size_t l;
		const char* s = luaL_tolstring(L, i, &l);  /* convert it to string */
		if (i > 1)  /* not the first element? */
			std::cout << " ";
		std::cout << s;
		lua_pop(L, 1);  /* pop result */
	}
	std::cout << std::endl;
	return 0; // return 0 items on the stack
}

/**
 * Sets the 4 color values in the cam_machine's
 * color table.  
 */
int lua_set_color_table(lua_State* L) 
{
	// discard any extra arguemnts pass in
	lua_settop(L, 4);

	// Check 4 table params
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	luaL_checktype(L, 3, LUA_TTABLE);
	luaL_checktype(L, 4, LUA_TTABLE);

	olc::Pixel color[4];

	for (int i = 0; i < 4; i++)
	{
		// Push param1 table values onto the stack
		lua_geti(L, i+1, 1);
		lua_geti(L, i+1, 2);
		lua_geti(L, i+1, 3);
		// Now stack has
		// 1 -- color1 {r1, g1, b1}
		// 2 -- color2 {r2, g2, b2}
		// 3 -- color3 {r3, g3, b3}
		// 4 -- color4 {r4, g4, b4}
		// -3 -- r1
		// -2 -- g1
		// -1 -- b1
		color[i].r = luaL_checkinteger(L, -3);
		color[i].g = luaL_checkinteger(L, -2);
		color[i].b = luaL_checkinteger(L, -1);

		lua_pop(L, 3);
	}

	g_cam.set_color_table(color[0], color[1], color[2], color[3]);

	return 0; // return 0 items on the stack
}

int lua_update_rule(lua_State* L)
{
	// discard any extra arguemnts pass in
	lua_settop(L, 3);

	// Get the params
	int bit_plane = luaL_checkinteger(L, 1);
	int index = luaL_checkinteger(L, 2);
	int value = luaL_checkinteger(L, 3);

	g_cam.update_rule(bit_plane, index, value);

	return 0;	// return 0 items on the stack
}

