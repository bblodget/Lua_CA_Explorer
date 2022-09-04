#include "rules.h"
#include "cam_machine.h"

int rules::life(int neighbors)
{
	g_cam.set_neighborhood(CAM_half::N_MOORE);
	g_cam.set_table(CAM_half::PLN0);

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
	g_cam.set_table(CAM_half::PLN0);

	// FIXME: This technique is not portable.
	// But it is convenient. 
	CAM_half::N_Moore* n = (CAM_half::N_Moore*)&neighbors;

	// XOR these togehter
	return n->CENTER ^ n->NORTH ^ n->SOUTH ^ n->WEST ^ n->EAST;

};

int rules::echo(int neighbors)
{
	g_cam.set_neighborhood(CAM_half::N_MOORE);
	g_cam.set_table(CAM_half::PLN1);

	// FIXME: This technique is not portable.
	// But it is convenient. 
	CAM_half::N_Moore* n = (CAM_half::N_Moore*)&neighbors;

	return n->CENTER;
}

int rules::trace(int neighbors)
{
	g_cam.set_neighborhood(CAM_half::N_MOORE);
	g_cam.set_table(CAM_half::PLN1);

	// FIXME: This technique is not portable.
	// But it is convenient. 
	CAM_half::N_Moore* n = (CAM_half::N_Moore*)&neighbors;

	return n->CENTER | n->CENTER_;
}