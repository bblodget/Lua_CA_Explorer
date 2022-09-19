
-- Color Constants
GREY				= {192, 192, 192}
DARK_GREY			= {128, 128, 128}
VERY_DARK_GREY		= {64, 64, 64}
RED					= {255, 0, 0}
DARK_RED			= {128, 0, 0}
VERY_DARK_RED		= {64, 0, 0}
YELLOW				= {255, 255, 0}
DARK_YELLOW			= {128, 128, 0}
VERY_DARK_YELLOW	= {64, 64, 0}
GREEN				= {0, 255, 0}
DARK_GREEN			= {0, 128, 0}
VERY_DARK_GREEN		= {0, 64, 0}
CYAN				= {0, 255, 255}
DARK_CYAN			= {0, 128, 128}
VERY_DARK_CYAN		= {0, 64, 64}
BLUE				= {0, 0, 255}
DARK_BLUE			= {0, 0, 128} 
VERY_DARK_BLUE		= {0, 0, 64,}
MAGENTA				= {255, 0, 255}
DARK_MAGENTA		= {128, 0, 128}
VERY_DARK_MAGENTA	= {64, 0, 64}
WHITE				= {255, 255, 255} 
BLACK				= {0, 0, 0}
BLANK				= {0, 0, 0, 0}

-- Plane Constants
PLN0	= 0
PLN1	= 1

-- Screen Constants
SCREEN_WIDTH	= 256
SCREEN_HEIGHT	= 256

function n_moore(index)
	nh = {}
	nh.CENTER	= (index >> 0) & 0x01
	nh.CENTER_	= (index >> 1) & 0x01
	nh.S_EAST	= (index >> 2) & 0x01
	nh.S_WEST	= (index >> 3) & 0x01
	nh.N_EAST	= (index >> 4) & 0x01
	nh.N_WEST	= (index >> 5) & 0x01
	nh.EAST		= (index >> 6) & 0x01
	nh.WEST		= (index >> 7) & 0x01
	nh.SOUTH	= (index >> 8) & 0x01
	nh.NORTH	= (index >> 9) & 0x01
	return nh
end

function make_table(rule, bit_plane)
	bit_plane = bit_plane or 0 -- 0 is the default plane for optional param
	for i = 0, 1023 do
		update_rule(bit_plane, i, rule(i))
	end
end

-- Start Rules

function life(index)
	nh = n_moore(index)

	sum = nh.N_WEST + nh.NORTH + nh.N_EAST +
		nh.WEST + nh.EAST +
		nh.S_WEST + nh.SOUTH + nh.S_EAST

	if nh.CENTER == 0 then
		-- If this cell is DEAD
		if sum == 3 then 
			return 1
		else
			return 0
		end
	else
		-- If this is is ALIVE
		if sum == 2 or sum == 3 then
			return 1
		else 
			return 0
		end
	end
end

function hglass(index)
	nh = n_moore(index)

	ewsnc = nh.CENTER << 0 |
		nh.NORTH << 1 |
		nh.SOUTH << 2 |
		nh.WEST << 3 |
		nh.EAST << 4

	if	ewsnc == 1 or ewsnc == 2 or ewsnc == 3 or ewsnc == 11 or
		ewsnc == 21 or ewsnc == 25 or ewsnc == 29 or ewsnc == 30 or ewsnc == 31 then
		return 1
	else
		return 0
	end
end

function echo(index)
	nh = n_moore(index)

	return nh.CENTER
end


function trace(index)
	n = n_moore(index)
	return n.CENTER | n.CENTER_
end

function off(index)
	return 0
end

function parity(index)
	n = n_moore(index)
	return n.CENTER ~ n.NORTH ~ n.SOUTH ~ n.EAST ~ n.WEST
end

-- End Rules

function clear()
	clear_state(PLN0)
	clear_state(PLN1)
	clear_out(PLN0)
	clear_out(PLN1)
end


function draw_r_pentonimo(x, y)
	set_state(x, y+0, "..##.")
	set_state(x, y+1, ".##..")
	set_state(x, y+2, "..#..")
end

function fill_rect(x, y, w, h)
	for yy=y,y+h do
		for xx=x,x+w do
			set_state(xx,yy,"#")
		end
	end
end

function draw_inf_growth(x, y)
	set_state(x, y, "########.#####...###......#######.#####")
end

-- Specify the percent of random ones
function rand(ones_percent)
	local r = math.random(100)
	if (r>ones_percent) then
		return 0
	else
		return 1
	end
end

function rand_plane(ones_percent, plane)
	local r=0
	ones_percent = ones_percent or 50	-- 50% default for opt param
	plane = plane or PLN0 -- default for optional param
	for y=1,SCREEN_HEIGHT do
		for x=1,SCREEN_WIDTH do
			r = rand(ones_percent)
			if r==0 then
				set_state(x,y,".")
			else
				set_state(x,y,"#")
			end
		end
	end
end

function cs()
	home()
	clear()
end


-- demos

function life_inf_growth()
	clear()
	make_table(life, PLN0)
	make_table(echo, PLN1)
	draw_inf_growth(100, 128)
	set_fps(10)
	run()
end

function life_random()
	clear()
	set_color_table(BLACK, WHITE, RED, GREEN)
	make_table(life, PLN0)
	make_table(echo, PLN1)
	rand_plane(15)
	set_fps(10)
	run()
end

function life_r_pentonimo()
	clear()
	set_color_table(BLACK, WHITE, RED, GREEN)
	make_table(life, PLN0)
	make_table(echo, PLN1)
	draw_r_pentonimo(128,128)
	set_fps(10)
	run()
end

function demo_parity()
	clear()
	make_table(parity, PLN0)
	make_table(echo, PLN1)
	fill_rect(112, 112, 32, 32)
	set_fps(10)
	run()
end

function demo_hglass()
	clear()
	make_table(hglass, PLN0)
	make_table(echo, PLN1)
	set_color_table(BLACK, CYAN, VERY_DARK_CYAN, BLUE)
	rand_plane(10)
	set_fps(0)
	run()
end

-- end demos

function startup()
	print("Welcome to Lua CA Explorer.")
	print("Type TAB to toggle between console mode and normal mode.")
	print("Type demos() to see a list of demos.")
	print("")
end

function demos()
	print("")
	print("Remember to press TAB to toggle to normal mode and back.")
	print("Here are some demos you can try:")
	print("   life_inf_growth()")
	print("   life_random()")
	print("   life_r_pentonimo()")
	print("   demo_parity()")
	print("   demo_hglass()")
	print("")
	print("Other commands to try")
	print("   step()     : step one generation")
	print("   step(x)    : step x generations")
	print("   run()      : run at set frame rate")
	print("   set_fps(0) : Max frame rate")
	print("   set_fps(x) : Set frame rate to x fps")
	print("")
end

