#include "cam_machine.h"
#include "rules.h"

namespace cam_example {

	void life_r_pentonimo()
	{
		// Make the PLN0 rules
		g_cam.make_table(rules::life);

		// Make the PLN1 rules
		g_cam.make_table(rules::echo);
		//g_cam.make_table(rules::trace);

		// Initialize PLN0 state with R-Pentonimo
		g_cam.set_state(CAM_half::PLN0, 125, 127, "..##.");
		g_cam.set_state(CAM_half::PLN0, 125, 128, ".##..");
		g_cam.set_state(CAM_half::PLN0, 125, 129, "..#..");
	}

	void life_inf_growth()
	{
		// Make the PLN0 rules
		g_cam.make_table(rules::life);

		// Make the PLN1 rules
		g_cam.make_table(rules::echo);
		//g_cam.make_table(rules::trace);

		// Infinite Growth
		g_cam.set_state(CAM_half::PLN0, 20, 50, "########.#####...###......#######.#####");
	}

	void parity()
	{
		// Make the PLN0 rules
		g_cam.make_table(rules::parity);

		// Make the PLN1 rules
		g_cam.make_table(rules::echo);
		//g_cam.make_table(rules::trace);

		//Init PLN0 with small square.
		g_cam.set_state(CAM_half::PLN0, 128, 126, "####");
		g_cam.set_state(CAM_half::PLN0, 128, 127, "####");
		g_cam.set_state(CAM_half::PLN0, 128, 128, "####");
		g_cam.set_state(CAM_half::PLN0, 128, 129, "####");
	}


}