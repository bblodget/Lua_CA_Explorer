#define OLC_PGE_APPLICATION
#define OLC_KEYBOARD_US

#include "olcPixelGameEngine.h"

//#include "zforth.h"
//#include "zforth_cam.h"

#include "cam_machine.h"
#include "rules.h"
#include "cam_examples.h"

extern "C"
{
#include "lua-5.4.4/src/lua.h"
#include "lua-5.4.4/src/lauxlib.h"
#include "lua-5.4.4/src/lualib.h"
}

/* mark in error messages for incomplete statements */
#define EOFMARK		"<eof>"

// Create global CAM object
CAM_half g_cam;

int lua_HostFunction(lua_State* L)
{
	int stack_size = lua_gettop(L);
	float a = (float)lua_tonumber(L, 1);
	float b = (float)lua_tonumber(L, 2);

	std::cout << "[C++] HostFunction(" << a << ", " << b << ") called, stack_size: " << stack_size << std::endl;

	float c = a * b;
	lua_pushnumber(L, c);

	return 1;  // return 1 value back to lua
}

int lua_print(lua_State* L) {
	const char* str = luaL_checkstring(L, 1);

	std::cout << str << std::endl;

	return 0;
}

// Override base class with your custom functionality
class CAM6 : public olc::PixelGameEngine
{
private:
	lua_State* L;

	struct Player
	{
		std::string title;
		std::string name;
		std::string family;
		int level;
	} player;

public:
	CAM6()
	{
		// Name your application
		sAppName = "CAM6";
	}

public:
	bool OnUserCreate() override
	{
		
		// Called once at the start, so create things here

		// Init random generator
		srand(time(NULL));

		ConsoleCaptureStdOut(true);

		// Zforth setup
		//zf_init(0);  // no tracing
		//zf_bootstrap();
		//zf::include("./forth/core.zf");

		// Lua setup
		L = luaL_newstate();
		luaL_openlibs(L);
		lua_register(L, "HostFunction", lua_HostFunction);
		lua_register(L, "print", lua_print);


		// Init CAM machine
		//cam_example::life_r_pentonimo();
		//cam_example::life_inf_growth();
		//cam_example::life_random();
		//cam_example::parity();
		cam_example::hglass();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame

		//if (!GetKey(olc::Key::SPACE).bHeld)
		//	return true;
		//Sleep(10);
		//if (!GetKey(olc::Key::SPACE).bPressed)
		//	return true;

		// User Input
		if (GetKey(olc::Key::TAB).bPressed)
			ConsoleShow(olc::Key::TAB, false);

	

		// Copy the current state to the output
		g_cam.cp_state_to_output();

		// Go through every pixel and update for next state
		// Display this state.
		
		for (int y = 0; y < ScreenHeight(); y++)
			for (int x = 0; x < ScreenWidth(); x++)
			{	
				g_cam.update_state(x, y);
				Draw(x, y, g_cam.get_out_pixel(x, y));
			}

		return true;
	}

	// Override this to receive "event" when ENTER is pressed in "TEXT ENTRY" mode
	//void OnTextEntryComplete(const std::string& sText) override
	//{
	//	std::cout << "Text Entry: " + sText << std::endl;
	//}

	bool OnConsoleCommand(const std::string& sText) override
	{
		//static int line = 1;
		static std::string chunk;
		static bool in_chunk = false;
		int status;

		if (in_chunk) 
		{
			// continuation of chunk
			// Append the new line to the old
			chunk.append(std::string("\n") + sText);
		}
		else
		{
			// New chunk
			chunk.clear();
			chunk.append(sText);
		}

		//Check if we can just run the new line
		status = luaL_dostring(L, chunk.c_str());
		if (status != LUA_OK) 
		{
			std::string errormsg = lua_tostring(L, -1);
			if (sText.rfind(EOFMARK) == std::string::npos) {
				lua_pop(L, 1);
				std::cout << "... ";
				std::cout.flush();
				in_chunk = true;
			}
			else
			{
				std::cout << errormsg << std::endl;
				in_chunk = false;
				chunk.clear();
			}
		}
		else
		{
			// Chunk ran ok
			in_chunk = false;
			chunk.clear();
		}




		//Push the new line onto the Lua stack
		//lua_pushlstring(L, sText.c_str(), strlen(sText.c_str()));





		//zf::do_eval("olc", ++line, sText.c_str());
		//std::string buff = sText + '\n';

		//std::cout << "[C++] " << cmd << "bla" << std::endl;

		//CheckLua(luaL_dostring(L, cmd.c_str()));

		//CheckLua(luaL_loadbuffer(L, buff.c_str(), strlen(buff.c_str()), "line") ||
		//	lua_pcall(L, 0, 0, 0));

		//CheckLua(luaL_dostring(L, "print(\"[LUA]  Hello World\")"));

		//if (CheckLua(luaL_dofile(L, "VideoExample.lua")))
		//{

		//}
		//{
		//	lua_getglobal(L, "DoAThing");
		//	if (lua_isfunction(L, -1))
		//	{
		//		lua_pushnumber(L, 5.0f);
		//		lua_pushnumber(L, 6.0f);

		//		if (CheckLua(lua_pcall(L, 2, 1, 0)))
		//		{
		//			float val = (float)lua_tonumber(L, -1);
		//			std::cout << "Value returned: " << val << std::endl;
		//		}
		//	}

		//	/*lua_getglobal(L, "player");
		//	if (lua_istable(L, -1))
		//	{
		//		lua_pushstring(L, "Name");
		//		lua_gettable(L, -2);
		//		player.name = lua_tostring(L, -1);
		//		lua_pop(L, 1);

		//		lua_pushstring(L, "Family");
		//		lua_gettable(L, -2);
		//		player.family = lua_tostring(L, -1);
		//		lua_pop(L, 1);

		//		lua_pushstring(L, "Title");
		//		lua_gettable(L, -2);
		//		player.title = lua_tostring(L, -1);
		//		lua_pop(L, 1);

		//		lua_pushstring(L, "Level");
		//		lua_gettable(L, -2);
		//		player.level = (int)lua_tonumber(L, -1);
		//		lua_pop(L, 1);
		//	}
		//	std::cout << player.title << " " << player.name << " of " << player.family << " [Level: " << player.level << "]" << std::endl;*/
		//}


		return true;

	}

	bool CheckLua(int r)
	{
		if (r != LUA_OK)
		{
			std::string errormsg = lua_tostring(L, -1);
			std::cout << errormsg << std::endl;
			return false;
		}
		return true;
	}
};



int main()
{
	CAM6 demo;
	if (demo.Construct(g_cam.get_screen_width(), g_cam.get_screen_height(), 3, 3, false))
		demo.Start();
	return 0;
}
