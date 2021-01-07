#include "Simulation.h"


short Simulation::_C_BARTERS = 1;
short Simulation::_C_BLAZE_KILLS = 1;
double Simulation::_C_EP_P = EP_P;
double Simulation::_C_BR_P = BR_P;
unsigned long long Simulation::_C_SIM_COMP = 0;
unsigned long long Simulation::_C_BART_COMP = 0;
unsigned long long Simulation::_C_BR_COMP = 0;
SimSaveFile* Simulation::_CURRENT_SAVE = nullptr;
uint64_t Simulation::seed = time(NULL);
//std::array<ThreadSave, THREADS> Simulation::_t_saves = std::array<ThreadSave, THREADS>();
ThreadSave Simulation::_t_saves[THREADS];
Simulation::Simulation() {
	//check if thread is in array
	
	for (int i = 0; i < THREADS; i++) {
		if (!_t_saves[i].used) {
			_t_saves[i].used = true;
			_t_saves[i].id = std::this_thread::get_id();
			t_index = i;
			break;
		}
	}
	

	//for (i = 0; i < _CURRENT_SAVE->barters; i++) {
	//	if (static_cast<double>(MWC64X()) / UINT32_MAX <= _CURRENT_SAVE->ep_p) barters++;
	//}
	//for (i = 0; i < _CURRENT_SAVE->blaze_kills; i++) {
	//	if (static_cast<double>(MWC64X()) / UINT32_MAX <= _CURRENT_SAVE->br_p) br_received++;
	//}
	//if (br_received > _CURRENT_SAVE->max_res.br_received) {
	//	_CURRENT_SAVE->max_res.br_received = br_received;
	//}
	//if (barters > _CURRENT_SAVE->max_res.barters)_CURRENT_SAVE->max_res.barters = barters;

	//if (br_received > _CURRENT_SAVE->max_res.br_received_comb && barters > _CURRENT_SAVE->max_res.barters_comb) {
	//	_CURRENT_SAVE->max_res.barters_comb = barters;
	//	_CURRENT_SAVE->max_res.br_received_comb = br_received;
	//}

	//_CURRENT_SAVE->sim_comp++;
	//_CURRENT_SAVE->all_barters += barters;
	//_CURRENT_SAVE->all_br_received += br_received;
}



uint32_t Simulation::MWC64X() {
	uint32_t c = (seed) >> 32, x = (seed) & 0xFFFFFFFF;
	seed = x * ((uint64_t)4294883355U) + c;
	return x ^ c;
}

void Simulation::setSimulationPar(SimSaveFile* config)
{
	_CURRENT_SAVE = config;
}
void Simulation::combineResults() {
	for (int i = 0; i < THREADS; i++) {
		if (_t_saves[i].save.max_res.barters > _CURRENT_SAVE->max_res.barters) {
			_CURRENT_SAVE->max_res.barters = _t_saves[i].save.max_res.barters;
		}
		if (_t_saves[i].save.max_res.br_received > _CURRENT_SAVE->max_res.br_received) {
			_CURRENT_SAVE->max_res.br_received = _t_saves[i].save.max_res.br_received;
		}

		if (_t_saves[i].save.max_res.br_received_comb > _CURRENT_SAVE->max_res.br_received_comb && _t_saves[i].save.max_res.barters_comb > _CURRENT_SAVE->max_res.barters_comb) {
			_CURRENT_SAVE->max_res.br_received_comb = _t_saves[i].save.max_res.br_received_comb;
			_CURRENT_SAVE->max_res.barters_comb = _t_saves[i].save.max_res.barters_comb;
		}

		_t_saves[i] = ThreadSave();
	}
	
}

void Simulation::calculate() {
	unsigned short i = 0;
	//for (i = 0; i < _CURRENT_SAVE->barters; i++) {
	//	if (static_cast<double>(MWC64X()) / UINT32_MAX <= _t_saves[t_index].save.ep_p) barters++;
	//}
	//for (i = 0; i < _CURRENT_SAVE->blaze_kills; i++) {
	//	if (static_cast<double>(MWC64X()) / UINT32_MAX <= _t_saves[t_index].save.br_p) br_received++;
	//}
	//if (br_received > _CURRENT_SAVE->max_res.br_received) {
	//	_t_saves[t_index].save.max_res.br_received = br_received;
	//}
	//if (barters > _t_saves[t_index].save.max_res.barters)_t_saves[t_index].save.max_res.barters = barters;

	//if (br_received > _t_saves[t_index].save.max_res.br_received_comb && barters > _t_saves[t_index].save.max_res.barters_comb) {
	//	_t_saves[t_index].save.max_res.barters_comb = barters;
	//	_t_saves[t_index].save.max_res.br_received_comb = br_received;
	//}

	//_t_saves[t_index].save.sim_comp++;
	//_t_saves[t_index].save.all_barters += barters;
	//_t_saves[t_index].save.all_br_received += br_received;
	for (i = 0; i < _CURRENT_SAVE->barters; i++) {
		if (static_cast<double>(MWC64X()) / UINT32_MAX <= _CURRENT_SAVE->ep_p) barters++;
	}
	for (i = 0; i < _CURRENT_SAVE->blaze_kills; i++) {
		if (static_cast<double>(MWC64X()) / UINT32_MAX <= _CURRENT_SAVE->br_p) br_received++;
	}
	if (br_received > _CURRENT_SAVE->max_res.br_received) {
		_CURRENT_SAVE->max_res.br_received = br_received;
	}
	if (barters > _CURRENT_SAVE->max_res.barters)_CURRENT_SAVE->max_res.barters = barters;

	if (br_received > _CURRENT_SAVE->max_res.br_received_comb && barters > _CURRENT_SAVE->max_res.barters_comb) {
		_CURRENT_SAVE->max_res.barters_comb = barters;
		_CURRENT_SAVE->max_res.br_received_comb = br_received;
	}

	_CURRENT_SAVE->sim_comp++;
	_CURRENT_SAVE->all_barters += barters;
	_CURRENT_SAVE->all_br_received += br_received;
}

