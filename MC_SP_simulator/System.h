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
#include <sstream>





class System {
public:
	System();
	~System();
	void run();
	void create_simulation(size_t save_index);
	void manage_custom_simulation();
	void simulate(const unsigned long long& sim_num);
	void print_results(size_t save_index);
	size_t selectSimulation();
	template<class T>
	T getInput(std::string text, T max, T min = 0);
	bool loadSaveFile();
	void saveSaveFile();
	void createSaveFile();
	void clearSave();

private:
	bool _system_running = true;
	//saves, 1. is always default
	SaveFile _save;
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

