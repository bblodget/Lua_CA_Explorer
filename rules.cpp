#include "rules.h"
#include "cam_machine.h"

int rules::life(int neighbors)
{
	g_cam.set_neighborhood(CAM_half::N_MOORE);

	// FIXME: This technique is not portable.
	// But it is convenient. 
	CAM_half::N_Moore* n = (CAM_half::N_Moore*)&neighbors;

	int sum = n->N_WEST + n->NORTH + n->N_EAST +
		n->WEST + n->EAST +
		n->S_WEST + n->SOUTH + n->S_EAST;

	if (n->CENTER == 0) {
		// If Dead
		if (sum == 3) return 1;
		else return 0;
	}
	else {
		// If Alive
		if (sum == 2 || sum == 3) return 1;
		else return 0;
	}
};

int rules::parity(int neighbors)
{
	g_cam.set_neighborhood(CAM_half::N_MOORE);

	// FIXME: This technique is not portable.
	// But it is convenient. 
	CAM_half::N_Moore* n = (CAM_half::N_Moore*)&neighbors;

	// XOR these togehter
	return n->CENTER ^ n->NORTH ^ n->SOUTH ^ n->WEST ^ n->EAST;

};

int rules::echo(int neighbors)
{
	g_cam.set_neighborhood(CAM_half::N_MOORE);

	// FIXME: This technique is not portable.
	// But it is convenient. 
	CAM_half::N_Moore* n = (CAM_half::N_Moore*)&neighbors;

	return n->CENTER;
}

int rules::trace(int neighbors)
{
	g_cam.set_neighborhood(CAM_half::N_MOORE);

	// FIXME: This technique is not portable.
	// But it is convenient. 
	CAM_half::N_Moore* n = (CAM_half::N_Moore*)&neighbors;

	return n->CENTER | n->CENTER_;
}

int rules::off(int neighbors)
{
	g_cam.set_neighborhood(CAM_half::N_MOORE);

	return 0;
}


int rules::hglass(int neighbors)
{
	g_cam.set_neighborhood(CAM_half::N_MOORE);

	// FIXME: This technique is not portable.
	// But it is convenient. 
	CAM_half::N_Moore* n = (CAM_half::N_Moore*)&neighbors;

	int ewsnc = n->CENTER << 0 |
		n->NORTH << 1 |
		n->SOUTH << 2 |
		n->WEST << 3 |
		n->EAST << 4;

	int val = 0;

	switch (ewsnc) {
	case 0: val = 0; break;
	case 1: val = 1; break;
	case 2: val = 1; break;
	case 3: val = 1; break;
	case 4: val = 0; break;
	case 5: val = 0; break;
	case 6: val = 0; break;
	case 7: val = 0; break;

	case 8: val = 0; break;
	case 9: val = 0; break;
	case 10: val = 0; break;
	case 11: val = 1; break;
	case 12: val = 0; break;
	case 13: val = 0; break;
	case 14: val = 0; break;
	case 15: val = 0; break;

	case 16: val = 0; break;
	case 17: val = 0; break;
	case 18: val = 0; break;
	case 19: val = 0; break;
	case 20: val = 0; break;
	case 21: val = 1; break;
	case 22: val = 0; break;
	case 23: val = 0; break;

	case 24: val = 0; break;
	case 25: val = 1; break;
	case 26: val = 0; break;
	case 27: val = 0; break;
	case 28: val = 0; break;
	case 29: val = 1; break;
	case 30: val = 1; break;
	case 31: val = 1; break;
	}

	return val;
}