#include "System.h"


System::System() { 
	std::cout << "Current seed: " << Simulation::seed << std::endl;
	load(); 
}

System::~System() {
	save();
}

void System::run() {	
	while (_system_running) {
		std::cout << "---------------------------------" << std::endl;
		std::cout << "1.) Simulate (fast)" << std::endl;
		std::cout << "2.) Simulate (custom) [TODO!]" << std::endl;
		std::cout << "3.) Print results" << std::endl;
		std::cout << "444.) Clear all data" << std::endl;
		std::cout << "5.) Exit" << std::endl;
		std::cout << "Input number of action: ";
		int res = 0;
		std::cin >> res;

		switch (res) {
		case 0: break;
		case 1: create_fast_simulation(); break;
		case 2: create_custom_simulation(); break;
		case 3: print_results(); break;
		case 444: clear_data(); break;
		case 5: _system_running = false; break;
		default: std::cout << "Wrong code!" << std::endl; break;
		}
	}
}

void System::create_fast_simulation() {
	unsigned long long sim_num = getInput<unsigned long long>("Enter number of simulations", ULLONG_MAX,1);
	//_sim_results = std::vector<Res_Table>(sim_num);
	//Clock status_timer;
	//Clock save_timer;
	Clock sim_clock;
	Clock status_timer;
	Clock save_timer;

	size_t t_num = 2;
	std::vector<std::thread> t_vec;
	for (size_t i = 0; i < t_num; i++) {
		if (!(sim_num % t_num)) {
			t_vec.push_back(std::thread(&System::simulate, this, sim_num / t_num));
		}
		else {
			std::cout << "Error!" << std::endl;
			return;
		}
	}
	for (size_t i = 0; i < t_num; i++) {
		t_vec[i].join();
	}

	double sim_duration_s = sim_clock.getTimeInS();
	std::cout << "Simulation completed!" << std::endl;
	std::cout << "Simulations: " << sim_num << std::endl;
	std::cout << "Simulation time: " << sim_duration_s << " s" << std::endl;
	std::cout << "Avg. simulation speed: " << sim_num / sim_duration_s << " Simulations per Second" << std::endl;
	save();
}

void System::simulate(const unsigned long long& sim_num) {
	Clock status_timer;
	Clock save_timer;
	
	for (unsigned long i = 0; i < sim_num; i++) {
		Simulation sim;
		Simulation::updateMaxRes(sim.getResult());
		if (status_timer.getTimeInS() > 1.5) {
			std::cout << "Thread " << std::this_thread::get_id() << " :" << ((double)i / sim_num) * 100 << " %" << std::endl;
			status_timer.reset();
		}
		if (save_timer.getTimeInS() > 15) {
			save();
			save_timer.reset();
		}	
	}
}

void System::create_custom_simulation()
{

}

void System::print_results() {
	std::cout << "---------------------------------" << std::endl;
	std::cout << "Simulations completed: " << Simulation::sim_completed << std::endl;
	std::cout << "Max succesfull barters: " << Simulation::_max_res.barters << " (GOAL: " << Simulation::_DREAM_BARTERS_SUCCES << ")" << std::endl;
	std::cout << "Max blaze rods received: " << Simulation::_max_res.br_received << " (GOAL: " << Simulation::_DREAM_BR_RECEIVED << ")" << std::endl;
	std::cout << "Max succesfull barters & blaze rods received at once: " << Simulation::_max_res.barters_comb << " EP / " << Simulation::_max_res.br_received << " BR";
	std::cout << " (GOAL: " << Simulation::_DREAM_BARTERS_SUCCES << " EP / " << Simulation::_DREAM_BR_RECEIVED << " )" << std::endl;
	if (Simulation::sim_completed > 0) {
		std::cout << "Avarage succesfull barters: " << double(Simulation::all_barters) / Simulation::sim_completed << std::endl;
		std::cout << "Avarage blaze rods received: " << double(Simulation::all_br_received) / Simulation::sim_completed << std::endl;
	}
}

void System::save() {
	SaveFile save = {Simulation::_max_res,Simulation::sim_completed,Simulation::all_barters,Simulation::all_br_received};
	std::fstream output("save_file.dat",std::ios::binary | std::ios::out);
	output.write((char*)&save, sizeof(SaveFile));
	output.close();
	std::cout << "Results saved!" << std::endl;
}

void System::load() {
	SaveFile save;
	std::fstream input("save_file.dat", std::ios::binary | std::ios::in);
	input.read((char*)&save,sizeof(SaveFile));
	Simulation::_max_res = save.max_res;
	Simulation::sim_completed = save.sim_comp;
	Simulation::all_barters = save.all_barters;
	Simulation::all_br_received = save.all_br_received;
	input.close();
	std::cout << "Results loaded!" << std::endl;
}

void System::clear_data()
{
	//clear results
	Simulation::_max_res = { 0,0,0,0};
	Simulation::sim_completed = 0;

	//clear save file
	std::fstream output("save_file.dat", std::ios::binary | std::ios::out);
	output.close();
}

template<class T>
unsigned long System::getInput(std::string text, T max, T min)
{
	unsigned long res = 0;
	while (true) {
		std::cout << "---------------------------------" << std::endl;
		std::cout << text << " (in interval " << min << "/" << max << "):";
		std::cin >> res;

		if (res >= min && res <= max) break;
		else {
			std::cout << "Input can be only in interval (" << min << "," << max << "): ";
		}
	}
	return res;
}
