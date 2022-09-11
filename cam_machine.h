#pragma once
#include <string>
#include "olcPixelGameEngine.h"
#include <assert.h>

// The original CAM6 machine had a resolution of 256x256.
// So we will stick with that for now.
// Makes wrap around easy, we can mask with 0xFF;
static constexpr int screen_width = 256;
static constexpr int screen_height = 256;

// A lookup table for the various bit planes (pln0, pln1, aux0, aux1)
// Currently only supporting Major Neighborhoods (10 bits - 1024)
// Once we support Minor Neighborhoods too, will go up to 12 bits - 4096.
static constexpr int table_size = 1024;


class CAM_half
{
public:
	enum State
	{
		STOP=0, RUN, STEP
	};

	enum Plane
	{
		PLN0=0, PLN1
	};

	enum Neighborhood
	{
		N_MOORE = 0, N_VONN
	};

	// Moore Neighborhood
	struct N_Moore
	{
		unsigned int CENTER		: 1;
		unsigned int CENTER_	: 1;
		unsigned int S_EAST		: 1;
		unsigned int S_WEST		: 1;
		unsigned int N_EAST		: 1;
		unsigned int N_WEST		: 1;
		unsigned int EAST		: 1;
		unsigned int WEST		: 1;
		unsigned int SOUTH		: 1;
		unsigned int NORTH		: 1;
	};

	// Von Neumann Neighborhood
	struct N_Vonn
	{
		unsigned int CENTER : 1;
		unsigned int CENTER_ : 1;
		unsigned int EAST_ : 1;
		unsigned int WEST_ : 1;
		unsigned int SOUTH_ : 1;
		unsigned int NORTH_ : 1;
		unsigned int EAST : 1;
		unsigned int WEST : 1;
		unsigned int SOUTH : 1;
		unsigned int NORTH : 1;
	};

	// Bit plane output values
	int m_pln0_out[screen_width * screen_height] = { 0 };
	int m_pln1_out[screen_width * screen_height] = { 0 };

	// Next state values
	int m_pln0_state[screen_width * screen_height] = { 0 };
	int m_pln1_state[screen_width * screen_height] = { 0 };

	// Rules for each of the planes
	int m_pln0_rules[table_size] = { 0 };
	int m_pln1_rules[table_size] = { 0 };

	// Running state
	State m_run_state = STOP;
	int m_steps = 0;

	// Color table.  Map the two planes to color values
	olc::Pixel m_color_table[4] = {
		olc::BLACK,		// 00
		olc::WHITE,		// 01
		olc::RED,		// 10
		olc::GREEN		// 11
	};

	// Current neighborhood type
	Neighborhood m_neighborhood = N_MOORE;

	// Get the CAM state, PAUSE, RUN, or STEP
	State get_run_state()
	{
		return m_run_state;
	}

	void dec_steps()
	{
		if (m_run_state == STEP) {
			m_steps--;
			if (m_steps <= 0) {
				m_run_state = STOP;
				m_steps = 0;
			}
		}
	}

	void set_steps(int steps)
	{
		m_steps = steps;
		m_run_state = STEP;
	}

	void run()
	{
		m_run_state = RUN;
	}

	void stop()
	{
		m_run_state = STOP;
	}

	// Set the current type of neighborhood
	void set_neighborhood(Neighborhood hood)
	{
		m_neighborhood = hood;
	}

	void set_color_table(olc::Pixel color0, olc::Pixel color1,
		olc::Pixel color2, olc::Pixel color3)
	{
		m_color_table[0] = color0;
		m_color_table[1] = color1;
		m_color_table[2] = color2;
		m_color_table[3] = color3;
	}

	// Use a string to initialize state
	void set_state(Plane plane, int x, int y, std::string s)
	{
		int p = 0;
		for (auto c : s)
		{
			switch (plane) {
			case PLN0:
				m_pln0_state[y * screen_width + x + p] = (c == '#') ? 1 : 0;
				break;
			case PLN1:
				m_pln1_state[y * screen_width + x + p] = (c == '#') ? 1 : 0;
				break;
			}
			p++;
		}
	}

	// Clear the state in specified plane
	void clear_state(Plane plane)
	{
		for (int i = 0; i < (screen_width * screen_height); i++)
		{
			switch (plane)
			{
			case PLN0: m_pln0_state[i] = 0;
			case PLN1: m_pln1_state[i] = 0;
			}
		}
	}

	// Clear the state in specified plane
	void clear_out(Plane plane)
	{
		for (int i = 0; i < (screen_width * screen_height); i++)
		{
			switch (plane)
			{
			case PLN0: m_pln0_out[i] = 0;
			case PLN1: m_pln1_out[i] = 0;
			}
		}
	}

	void make_random_state(Plane plane)
	{
		for (int i = 0; i < (screen_width * screen_height); i++)
		{
			//int r = i % 16 == 0 ? rand() % 2 : 0;
			int r = rand() & 7;
			r = r == 1 ? 1 : 0;
			switch (plane)
			{
			case PLN0: m_pln0_state[i] = r;
			case PLN1: m_pln1_state[i] = r;
			}
		}
	}

	void update_rule(int bit_plane, int index, int value)
	{
		switch (bit_plane) {
		case PLN0:
			m_pln0_rules[index] = value;
			break;
		case PLN1:
			m_pln1_rules[index] = value;
			break;
		}
	}

	// Use the rule() function to update the current table.
	void make_table(int(*rule)(int neighbors), Plane bit_plane)
	{
		int value = 0;
		for (int i = 0; i < table_size; i++) {
			switch (bit_plane) {
			case PLN0 :
				m_pln0_rules[i] = rule(i);
				break;
			case PLN1 :
				m_pln1_rules[i] = rule(i);
			}
		}
	}

	// Copies the current state to the output arrays.
	void cp_state_to_output()
	{
		for (int i = 0; i < screen_width * screen_height; i++) {
			m_pln0_out[i] = m_pln0_state[i];
			m_pln1_out[i] = m_pln1_state[i];
		}
	}

	int calc_index(int x, int y) {
		return (y & 0xFF) * screen_width + (x & 0xFF);
	}

	// Use rules to update cells
	void update_state(int x, int y)
	{
		int s_index = calc_index(x, y);
		int r_index = 0;

		if (m_neighborhood == N_MOORE) {
			r_index |= m_pln0_out[s_index] << 0;	// CENTER
			r_index |= m_pln1_out[s_index] << 1;	// CENTER_
			r_index |= m_pln0_out[calc_index(x + 1, y + 1)] << 2;	// S_EAST
			r_index |= m_pln0_out[calc_index(x - 1, y + 1)] << 3;	// S_WEST
			r_index |= m_pln0_out[calc_index(x + 1, y - 1)] << 4;	// N_EAST
			r_index |= m_pln0_out[calc_index(x - 1, y - 1)] << 5;	// N_WEST
			r_index |= m_pln0_out[calc_index(x + 1, y + 0)] << 6;	// EAST
			r_index |= m_pln0_out[calc_index(x - 1, y + 0)] << 7;	// WEST
			r_index |= m_pln0_out[calc_index(x + 0, y + 1)] << 8;	// SOUTH
			r_index |= m_pln0_out[calc_index(x + 0, y - 1)] << 9;	// NORTH

			m_pln0_state[s_index] = m_pln0_rules[r_index];
			m_pln1_state[s_index] = m_pln1_rules[r_index];
			return;
		}

		if (m_neighborhood == N_VONN) {
			r_index =
				m_pln0_out[s_index] << 0 |	// CENTER
				m_pln1_out[s_index] << 1 |	// CENTER_
				m_pln1_out[calc_index(x + 1, y + 0)] << 2 |	// EAST_
				m_pln1_out[calc_index(x - 1, y + 0)] << 3 |	// WEST_
				m_pln1_out[calc_index(x + 0, y + 1)] << 4 |	// SOUTH_
				m_pln1_out[calc_index(x + 0, y - 1)] << 5 |	// NORTH_
				m_pln0_out[calc_index(x + 1, y + 0)] << 6 |	// EAST
				m_pln0_out[calc_index(x - 1, y + 0)] << 7 |	// WEST
				m_pln0_out[calc_index(x + 0, y + 1)] << 8 |	// SOUTH
				m_pln0_out[calc_index(x + 0, y - 1)] << 9;	// NORTH

			m_pln0_state[s_index] = m_pln0_rules[r_index];
			m_pln1_state[s_index] = m_pln1_rules[r_index];
			return;
		}
		
	}

	olc::Pixel get_out_pixel(int x, int y)
	{
		int index = y * screen_width + x;
		int color = (m_pln1_out[index]<<1) | m_pln0_out[index];
		return m_color_table[color];
	}


	// Return screen width
	int get_screen_width() { return screen_width; }

	// Return screen height
	int get_screen_height() { return screen_height; }
};

// Global cam object. That can shared.
// TODO:  make this a singleton.
// created in main.cpp
extern CAM_half g_cam;

//class CAM_machine
//{
//};

