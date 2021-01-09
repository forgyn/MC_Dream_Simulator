#pragma once
#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <vector>
#include <mutex>
//dream barters & blaze kill
const short DREAM_BARTERS = 262;
const short DREAM_BARTERS_SUCCES = 42;
const short DREAM_BLAZES_KILLED = 305;
const short DREAM_BR_RECEIVED = 211;

//default minecraft prop
const double EP_P = 20.f / 459.f;
const double BR_P = 0.5f;

//generates random number
static uint32_t MWC64X(uint64_t* state)
{
	uint32_t c = (*state) >> 32, x = (*state) & 0xFFFFFFFF;
	*state = x * ((uint64_t)4294883355U) + c;
	return x ^ c;
}

//maximal results from multiple simulations
struct Max_Sim_Res {
	unsigned barters = 0;
	unsigned br_received = 0;
	unsigned barters_comb = 0;
	unsigned br_received_comb = 0;
};

struct SimSaveFile {
	Max_Sim_Res max_res;
	unsigned long long sim_comp = 0;
	unsigned long long all_barters = 0;
	unsigned long long all_br_received = 0;
	double ep_p = EP_P;
	double br_p = BR_P;
	short barters = DREAM_BARTERS;
	short blaze_kills = DREAM_BLAZES_KILLED;
};

struct SaveFile {
	size_t sim_save_num;
	std::vector<SimSaveFile> sim_save;
};

class Simulation {
public:
	Simulation();
	~Simulation();
	void run();
	//returns current simulation result
	//Res_Table getResult();
	//update results
	//static void updateMaxRes(const Res_Table& results);
	static uint32_t MWC64X();
	static void setSimulationPar(SimSaveFile* config);
	void combineThreadRes();
public:
	//results
	static Max_Sim_Res max_res;

	//seed
	static uint64_t seed;
private:
	//ender pearls from barter
	

	//blare rods from blaze kills
	

	//custom barters & blaze kills
	static short _C_BARTERS;
	static short _C_BLAZE_KILLS;
	static double _C_EP_P;
	static double _C_BR_P;
	static unsigned long long _C_SIM_COMP;
	static unsigned long long _C_BART_COMP;
	static unsigned long long _C_BR_COMP;

	static SimSaveFile* _CURRENT_SAVE;
	SimSaveFile* _save = nullptr;
	static std::mutex _mutex;
};

