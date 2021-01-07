#include "Simulation.h"


//chance of gainging pearls from bartering
const double Simulation::ep_p = 20.f / 459.f;
//blaze rods chance of droping from blaze
const double Simulation::br_p = 0.5;

short Simulation::_BARTERS = 1;
short Simulation::_BLAZE_KILLS = 1;
unsigned long long Simulation::sim_completed = 0;
unsigned long long Simulation::all_barters = 0;
unsigned long long Simulation::all_br_received = 0;
uint64_t Simulation::seed = time(NULL);
Max_Sim_Res Simulation::_max_res = Max_Sim_Res{ 0,0,0,0 };


Simulation::Simulation() {
	static unsigned short i = 0;
	for (i = 0; i < _DREAM_BARTERS; i++) {
		if (static_cast<double>(MWC64X()) / UINT16_MAX <= ep_p)barters++;
		//if (0.f + 1.f * UNI <= ep_p)barters++;
		//if ((0 + MWC % 100) / 100.f <= ep_p)barters++;
	}
	for (i = 0; i < _DREAM_BLAZES_KILLED; i++) {
		//if ((0 + MWC % 100) / 100.f <= br_p)br_received++;
		//if (0.f + 1.f * UNI <= br_p)br_received++;
		if (static_cast<double>(MWC64X()) / UINT16_MAX <= br_p)br_received++;
	}
	sim_completed++;
	all_barters += barters;
	all_br_received += br_received;
	//settable(12345, 65435, 34221, 12345, 9983651, 95746118);
}

Res_Table Simulation::getResult() {
	return {ep_received,barters,br_received,blazes_killed};
}

void Simulation::updateMaxRes(const Res_Table& res) {
		if (res.br_received > _max_res.br_received)_max_res.br_received = res.br_received;

		if (res.barters > _max_res.barters)_max_res.barters = res.barters;

		if (res.br_received > _max_res.br_received_comb && res.barters > _max_res.barters_comb) {
			_max_res.barters_comb = res.barters;
			_max_res.br_received_comb = res.br_received;
		}		
}

uint32_t Simulation::MWC64X() {
	uint32_t c = (seed) >> 32, x = (seed) & 0xFFFFFFFF;
	seed = x * ((uint64_t)4294883355U) + c;
	return x ^ c;
}
