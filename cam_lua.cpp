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
#include "main.h"
#include <iostream>


/*********************** Function Definitions ************************/

// Open the CAM library
// Store a pointer to the Pixel Game Engine instance
int luaopen_libLuaCAM(lua_State* L) 
{
	lua_register(L, "print", lua_print);
	lua_register(L, "set_color_table", lua_set_color_table);
	lua_register(L, "update_rule", lua_update_rule);
	lua_register(L, "step", lua_step);
	lua_register(L, "run", lua_run);
	lua_register(L, "stop", lua_stop);
	lua_register(L, "home", lua_home);
	lua_register(L, "clear_state", lua_clear_state);
	lua_register(L, "clear_out", lua_clear_out);
	lua_register(L, "set_state", lua_set_state);
	lua_register(L, "set_fps", lua_set_fps);
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

int lua_step(lua_State* L)
{
	int num_steps = 0;

	// Get the number of args
	int n = lua_gettop(L);

	// discard any extra arguemnts pass in
	if (n > 1) {
		lua_settop(L, 1);
	}

	if (n > 0) {
		// Use the one argument
		num_steps = luaL_checkinteger(L, 1);
	}

	g_cam.set_steps(num_steps);

	return 0;
}

int lua_run(lua_State* L)
{
	// discard all arguments
	lua_settop(L, 0);

	g_cam.run();

	return 0;
}

int lua_stop(lua_State* L)
{
	lua_settop(L, 0); // discard all arguments
	g_cam.stop();
	return 0;
}

// Clears the console output
int lua_home(lua_State* L)
{
	// discard all arguments
	lua_settop(L, 0);
	pge_ConsoleClear();
	return 0;
}

int lua_clear_state(lua_State* L)
{
	lua_settop(L, 1); // Only one arguement, discard others
	CAM_half::Plane bit_plane = (CAM_half::Plane)luaL_checkinteger(L, 1);
	g_cam.clear_state(bit_plane);
	return 0;
}

int lua_clear_out(lua_State* L)
{
	lua_settop(L, 1); // Only one arguement, discard others
	CAM_half::Plane bit_plane = (CAM_half::Plane)luaL_checkinteger(L, 1);
	g_cam.clear_out(bit_plane);
	return 0;
}

// Sets states in the bitplane
// Expected params
// int x - x pos
// int y - y pos
// string s - Incoded string '#' means 1, anything else means 0.
// Plane p (opt) - The plane to write to PLN0 or PLN1. PLN0 default.
int lua_set_state(lua_State* L)
{
	CAM_half::Plane plane = CAM_half::PLN0;

	// Get the number of args
	int n = lua_gettop(L);

	// discard any extra arguemnts pass in
	if (n > 4) {
		lua_settop(L, 4);
	}

	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	std::string s = luaL_checkstring(L, 3);

	if (n > 3) {
		// Optional Plane argument
		int p = luaL_checkinteger(L, 4);
		if (p == 1) {
			plane = CAM_half::PLN1;
		}
	}

	g_cam.set_state(plane, x, y, s);

	return 0;
}

int lua_set_fps(lua_State* L)
{
	lua_settop(L, 1); // Only one arguement, discard others
	float fps = luaL_checknumber(L, 1);
	g_cam.set_fps(fps);
	return 0;
}

