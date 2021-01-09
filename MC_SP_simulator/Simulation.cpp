#include "Simulation.h"


//short Simulation::_C_BARTERS = 1;
//short Simulation::_C_BLAZE_KILLS = 1;
//double Simulation::_C_EP_P = EP_P;
//double Simulation::_C_BR_P = BR_P;
//unsigned long long Simulation::_C_SIM_COMP = 0;
//unsigned long long Simulation::_C_BART_COMP = 0;
//unsigned long long Simulation::_C_BR_COMP = 0;
SimSaveFile* Simulation::_CURRENT_SAVE = nullptr;
uint64_t Simulation::seed = time(NULL);
std::mutex Simulation::mutex;
//std::array<ThreadSave, THREADS> Simulation::_t_saves = std::array<ThreadSave, THREADS>();
//ThreadSave Simulation::_t_saves[THREADS];
Simulation::Simulation() {
	_save = *_CURRENT_SAVE;
	_save.sim_comp = 0;
	_save.all_barters = 0;
	_save.all_br_received = 0;
}

Simulation::~Simulation()
{
	combineThreadRes();
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
	/*for (int i = 0; i < THREADS; i++) {
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
	}*/
	
}

void Simulation::calculate() {
	unsigned short i = 0;
	barters = 0;
	br_received = 0;
	for (i = 0; i < _save.barters; i++) {
		if (static_cast<double>(MWC64X()) / UINT32_MAX <= _save.ep_p) barters++;
	}
	for (i = 0; i < _save.blaze_kills; i++) {
		if (static_cast<double>(MWC64X()) / UINT32_MAX <= _save.br_p) br_received++;
	}
	if (br_received > _save.max_res.br_received) {
		_save.max_res.br_received = br_received;
	}
	if (barters > _save.max_res.barters)_save.max_res.barters = barters;

	if (br_received > _save.max_res.br_received_comb && barters > _save.max_res.barters_comb) {
		_save.max_res.barters_comb = barters;
		_save.max_res.br_received_comb = br_received;
	}

	_save.sim_comp++;
	_save.all_barters += barters;
	_save.all_br_received += br_received;
}

void Simulation::combineThreadRes() {
	std::unique_lock<std::mutex> lock(mutex);
	if (_save.max_res.barters > _CURRENT_SAVE->max_res.barters) {
		_CURRENT_SAVE->max_res.barters = _save.max_res.barters;
	}
	if (_save.max_res.br_received > _CURRENT_SAVE->max_res.br_received) {
		_CURRENT_SAVE->max_res.br_received = _save.max_res.br_received;
	}
	if (_save.max_res.br_received_comb > _CURRENT_SAVE->max_res.br_received_comb && _save.max_res.barters_comb > _CURRENT_SAVE->max_res.barters_comb) {
		_CURRENT_SAVE->max_res.br_received_comb = _save.max_res.br_received_comb;
		_CURRENT_SAVE->max_res.barters_comb = _save.max_res.barters_comb;
	}

	_CURRENT_SAVE->sim_comp += _save.sim_comp;
	_CURRENT_SAVE->all_barters += _save.all_barters;
	_CURRENT_SAVE->all_br_received += _save.all_br_received;
}

