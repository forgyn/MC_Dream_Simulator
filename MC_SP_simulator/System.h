#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <limits>
#include "Simulation.h"
#include <chrono>
#include <thread>

struct SaveFile {
	Max_Sim_Res max_res;
	unsigned long long sim_comp = 0;
	unsigned long long all_barters = 0;
	unsigned long long all_br_received = 0;
};

class System {
public:
	System();
	~System();
	void run();
	void create_fast_simulation();
	void simulate(const unsigned long long& sim_num);
	void create_custom_simulation();
	void print_results();
	void save();
	void load();
	void clear_data();
	template<class T>
	unsigned long getInput(std::string text, T max, T min = 0);

private:
	bool _system_running = true;

};

class Clock {
public:
	typedef std::chrono::high_resolution_clock hclock;
	Clock() { start = hclock::now(); }
	void reset() { start = hclock::now(); }
	double getTimeInS() { return std::chrono::duration<double>(hclock::now() - start).count(); }
private:
	std::chrono::high_resolution_clock::time_point start = hclock::now();
};

