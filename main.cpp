#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "zforth.h"
#include "zforth_cam.h"

// The original CAM6 machine had a resolution of 256x256.
// So we will stick with that for now.
// Makes wrap around easy, we can mask with 0xFF;
static constexpr int screen_width = 256;
static constexpr int screen_height = 256;




// Override base class with your custom functionality
class CAM6 : public olc::PixelGameEngine
{
public:
	CAM6()
	{
		// Name your application
		sAppName = "CAM6";
	}

private:
	int* m_output;
	int* m_state;

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here


		ConsoleCaptureStdOut(true);

		// Zforth setup
		zf_init(0);  // no tracing
		zf_bootstrap();
		zf::include("./forth/core.zf");

		// Cell state arrays, and output to the screen arrays
		m_output = new int[ScreenWidth() * ScreenHeight()];
		m_state = new int[ScreenWidth() * ScreenHeight()];
		memset(m_output, 0, ScreenWidth() * ScreenHeight() * sizeof(int));
		memset(m_state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

		/*
		for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
			m_state[i] = rand() % 2;
		*/

		auto set = [&](int x, int y, std::string s)
		{
			int p = 0;
			for (auto c : s)
			{
				m_state[y * ScreenWidth() + x + p] = (c == '#') ? 1 : 0;
				p++;
			}
		};

		// R-Pentonimo
		set(80, 50, "..##.");
		set(80, 51, ".##..");
		set(80, 52, "..#..");


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame

		//if (!GetKey(olc::Key::SPACE).bHeld)
		//	return true;
		Sleep(10);

		// User Input
		if (GetKey(olc::Key::TAB).bPressed)
			ConsoleShow(olc::Key::TAB, false);

		

		auto cell = [&](int x, int y)
		{
			int yy = y & 0xFF;  
			int xx = x & 0xFF;
			return m_output[yy * ScreenWidth() +xx];
		};

		// Store output state
		for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
			m_output[i] = m_state[i];

		for (int x=0; x<ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
			{
				int nNeighbours = cell(x - 1, y - 1) + cell(x - 0, y - 1) + cell(x + 1, y - 1) +
					cell(x - 1, y + 0) + 0 + cell(x + 1, y + 0) +
					cell(x - 1, y + 1) + cell(x - 0, y + 1) + cell(x + 1, y + 1);

				if (cell(x, y) == 1)
				{
					m_state[y * ScreenWidth() + x] = nNeighbours == 2 || nNeighbours == 3;
				}
				else {
					m_state[y * ScreenWidth() + x] = (nNeighbours == 3);
				}

				if (cell(x, y) == 1)
					Draw(x, y, olc::WHITE);
				else
					Draw(x, y, olc::BLACK);
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

		zf::do_eval("olc", ++line, sText.c_str());

		return true;

	}
};

int main()
{
	CAM6 demo;
	if (demo.Construct(screen_width, screen_height, 3, 3))
		demo.Start();
	return 0;
}
