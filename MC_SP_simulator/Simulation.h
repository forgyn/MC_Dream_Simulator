#pragma once
#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <vector>
#include "simdxorshift128plus.h"

//generates random number
static uint32_t MWC64X(uint64_t* state)
{
	uint32_t c = (*state) >> 32, x = (*state) & 0xFFFFFFFF;
	*state = x * ((uint64_t)4294883355U) + c;
	return x ^ c;
}

//result table
struct Res_Table {
	unsigned ep_received;
	unsigned barters;
	unsigned br_received;
	unsigned blaze_kills;
};

//maximal results from multiple simulations
struct Max_Sim_Res {
	unsigned barters = 0;
	unsigned br_received = 0;
	unsigned barters_comb = 0;
	unsigned br_received_comb = 0;
};

class Simulation {
public:
	Simulation();
	//returns current simulation result
	Res_Table getResult();
	//update results
	static void updateMaxRes(const Res_Table& results);
	static uint32_t MWC64X();
public:
	//results
	static Max_Sim_Res _max_res;
	static unsigned long long sim_completed;
	static unsigned long long all_barters;
	static unsigned long long all_br_received;
	//dream barters & blaze kills
	static const short _DREAM_BARTERS = 262;
	static const short _DREAM_BARTERS_SUCCES = 42;
	static const short _DREAM_BLAZES_KILLED = 305;
	static const short _DREAM_BR_RECEIVED = 211;

	//custom barters & blaze kills
	static short _BARTERS;
	static short _BLAZE_KILLS;

	//seed
	static uint64_t seed;
private:
	//ender pearls from barter
	unsigned ep_received = 0;
	unsigned barters = 0;

	//blare rods from blaze kills
	unsigned br_received = 0;
	unsigned blazes_killed = 0;

	//simulation variables
	static const double ep_p;
	static const double br_p;
};

