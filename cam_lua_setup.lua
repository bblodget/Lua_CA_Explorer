
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
	for i = 1, 1024 do
		update_rule(bit_plane, i, rule(i))
	end
end

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
