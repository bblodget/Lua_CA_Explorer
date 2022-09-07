#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

//#include "zforth.h"
//#include "zforth_cam.h"

#include "cam_machine.h"
#include "rules.h"
#include "cam_examples.h"

extern "C"
{
#include "Lua542/include/lua.h"
#include "Lua542/include/lauxlib.h"
#include "Lua542/include/lualib.h"
}

#ifdef _WIN32
#pragma comment(lib, "lua542/liblua54.a")
#endif

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
		static int line = 1;

		//zf::do_eval("olc", ++line, sText.c_str());

		//if (CheckLua(luaL_dostring(L, sText.c_str())))
		if (CheckLua(luaL_dofile(L, "VideoExample.lua")))
		{
			lua_getglobal(L, "DoAThing");
			if (lua_isfunction(L, -1))
			{
				lua_pushnumber(L, 5.0f);
				lua_pushnumber(L, 6.0f);

				if (CheckLua(lua_pcall(L, 2, 1, 0)))
				{
					float val = (float)lua_tonumber(L, -1);
					std::cout << "Value returned: " << val << std::endl;
				}
			}

			/*lua_getglobal(L, "player");
			if (lua_istable(L, -1))
			{
				lua_pushstring(L, "Name");
				lua_gettable(L, -2);
				player.name = lua_tostring(L, -1);
				lua_pop(L, 1);

				lua_pushstring(L, "Family");
				lua_gettable(L, -2);
				player.family = lua_tostring(L, -1);
				lua_pop(L, 1);

				lua_pushstring(L, "Title");
				lua_gettable(L, -2);
				player.title = lua_tostring(L, -1);
				lua_pop(L, 1);

				lua_pushstring(L, "Level");
				lua_gettable(L, -2);
				player.level = (int)lua_tonumber(L, -1);
				lua_pop(L, 1);
			}
			std::cout << player.title << " " << player.name << " of " << player.family << " [Level: " << player.level << "]" << std::endl;*/
		}


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
