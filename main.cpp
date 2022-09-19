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

#include "cam_lua.h"

/* mark in error messages for incomplete statements */
#define EOFMARK		"<eof>"

// Create global CAM object
CAM_half g_cam;

// Override base class with your custom functionality
class LuaCAM : public olc::PixelGameEngine
{
private:
	lua_State* L;
	float		fFrameTimer = 0.0f;

public:
	LuaCAM()
	{
		// Name your application
		sAppName = "Lua CA Explorer";
	}

public:
	bool OnUserCreate() override
	{
		
		// Called once at the start, so create things here

		// Init random generator
		srand(time(NULL));

		ConsoleCaptureStdOut(true);

		// Default to Console mode
		ConsoleShow(olc::Key::TAB, false);

		// Lua setup
		L = luaL_newstate();
		luaL_openlibs(L);
		luaopen_libLuaCAM(L);
		


		// Init CAM machine
		//cam_example::life_r_pentonimo();
		cam_example::life_inf_growth();
		//cam_example::life_random();
		//cam_example::parity();
		//cam_example::hglass();
		//cam_example::start();

		// Lua setup script
		CheckLua(luaL_dofile(L, "./assets/cam_lua_setup.lua"));

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

		bool do_update = false;
		fFrameTimer += fElapsedTime;
		if (fFrameTimer > g_cam.get_delay()) {
			do_update = true;
			fFrameTimer = 0.0;
		}

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
				if (g_cam.get_run_state() == CAM_half::STEP || (do_update && g_cam.get_run_state() == CAM_half::RUN)) {
					g_cam.update_state(x, y);
				}
				Draw(x, y, g_cam.get_out_pixel(x, y));
			}

		g_cam.dec_steps();

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

		// Check if we can run as an expression
		// Which will print the value when it gets pushed on the stack
		std::string ret_chunk = std::string("return " + chunk);
		status = luaL_dostring(L, ret_chunk.c_str());

		if (status != LUA_OK) {
			lua_pop(L, 1); // remove the error message from stack

			//Check if we can just run the new line
			status = luaL_dostring(L, chunk.c_str());
		}

		if (status != LUA_OK) 
		{
			std::string errormsg = lua_tostring(L, -1);
			if (errormsg.rfind(EOFMARK) != std::string::npos) {
				lua_pop(L, 1);  // pop the error msg
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

			// Print anything on the stack
			l_print();
		}


		return true;

	}

	bool CheckLua(int r)
	{
		if (r != LUA_OK)
		{
			std::string errormsg = lua_tostring(L, -1);
			std::cout << errormsg << std::endl;
			lua_pop(L, 1);  // pop the error msg
			return false;
		}
		return true;
	}

	void l_print()
	{
		int n = lua_gettop(L);
		if (n > 0) {  /* any result to be printed? */
			luaL_checkstack(L, LUA_MINSTACK, "too many results to print");
			lua_getglobal(L, "print");
			lua_insert(L, 1);
			CheckLua(lua_pcall(L, n, 0, 0));
		}
	}


};

// Give the cam_pge file scope
static LuaCAM cam_pge;

int main()
{
	if (cam_pge.Construct(g_cam.get_screen_width(), g_cam.get_screen_height(), 3, 3, false))
		cam_pge.Start();
	return 0;
}

void pge_ConsoleClear()
{
	cam_pge.ConsoleClear();
}

void pge_ConsoleHide()
{
	// TODO: Implement this function

}
