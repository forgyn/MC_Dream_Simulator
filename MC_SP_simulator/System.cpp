#include "System.h"


System::System() { 
	std::cout << "Current seed: " << Simulation::seed << std::endl;
	//_saves.push_back(load("def_sim.dat"));

	if (!loadSaveFile()) {

	}


	//load(); 
}

System::~System() {
	//save();
}

void System::run() {	
	while (_system_running) {
		std::cout << "---------------------------------" << std::endl;
		std::cout << "1.) Simulate (default)" << std::endl;
		std::cout << "2.) Simulate (custom)" << std::endl;
		std::cout << "3.) Print result" << std::endl;
		std::cout << "444.) Clear all data" << std::endl;
		std::cout << "5.) Exit" << std::endl;
		std::cout << "Input number of action: ";
		int res = 0;
		std::cin >> res;

		switch (res) {
		case 0: break;
		case 1: create_simulation(0); break;
		case 2: manage_custom_simulation(); break;
		case 3: print_results(selectSimulation()); break;
		case 444: clearSave(); break;
		case 5: _system_running = false; break;
		default: std::cout << "Wrong code!" << std::endl; break;
		}
	}
}

void System::create_simulation(size_t save_index) {
	unsigned long long sim_num = getInput<unsigned long long>("Enter number of simulations", ULLONG_MAX,1);
	Clock sim_clock;
	Clock status_timer;
	Clock save_timer;

	//set default sim parameters

	Simulation::setSimulationPar(&(_save.sim_save[save_index]));
	//simulate(sim_num);
	//multi thread simulation
	size_t t_num = THREADS;
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

	//Simulation::combineResults();

	//_save.sim_save[0].max_res = Simulation::max_res;

	//simulation stats
	double sim_duration_s = sim_clock.getTimeInS();
	std::cout << "Simulation completed!" << std::endl;
	std::cout << "Simulation time: " << sim_duration_s << " s" << std::endl;
	std::cout << "Avg. simulation speed: " << sim_num / sim_duration_s << " Simulations per Second" << std::endl;
	saveSaveFile();
}

void System::simulate(const unsigned long long& sim_num) {
	Clock status_timer;
	Clock save_timer;
	Simulation sim;
	for (unsigned long i = 0; i < sim_num; i++) {
		sim.calculate();
		/*if (status_timer.getTimeInS() > 1.5) {
			std::cout << "Thread " << std::this_thread::get_id() << " :" << ((double)i / sim_num) * 100 << " %" << std::endl;
			status_timer.reset();
		}*/
	}
}

void System::manage_custom_simulation()
{
	//select simulation save
	while (true) {
		std::cout << "---------------------------------" << std::endl;
		std::cout << "Select simulation save you wish to use" << std::endl;
		std::cout << "1.) Default Simulation" << std::endl;
		for (int i = 1; i < _save.sim_save.size(); i++) {
			std::cout << i + 1 << ".) Custom simulation " << i << std::endl;
		}
		std::cout << _save.sim_save.size() + 1 << ".) Create new simulation " << std::endl;
		std::cout << _save.sim_save.size() + 2 << ".) Back " << std::endl;
		std::cout << "Input number of action: ";
		int res = 0;
		std::cin >> res;
		res--;
		if (res >= 0 && res < _save.sim_save.size()+2) {
			if (res < _save.sim_save.size()) {
				std::stringstream sstream;
				sstream << "---------------------------------" << std::endl;
				sstream << "EP_P: " << _save.sim_save[res].ep_p * 100 << " %" << std::endl;
				sstream << "BR_P: " << _save.sim_save[res].br_p * 100 << " %" << std::endl;
				sstream << "BARTERS: " << _save.sim_save[res].barters << std::endl;
				sstream << "BLAZE KILLS: " << _save.sim_save[res].blaze_kills << std::endl;
				sstream << "!Do you wish to start simulation with this parameters? (0-no/1-yes)";
				bool confirm = getInput<bool>(sstream.str(), 1, 0);
				if (confirm) {
					create_simulation(res);
					return;
				}
			}
			else if (res == _save.sim_save.size()) {
				unsigned short ep_weight = getInput<unsigned short>("Select ender pearl weight (Normal: 20)", UINT16_MAX, 20);
				double br_p = getInput<double>("Select blaze rod drop chance (in %) (Normal: 50%)", 100, 0);
				unsigned short barters = getInput<unsigned short>("Select number of barters per simulation (Dream: " + std::to_string(DREAM_BARTERS) + ")", UINT16_MAX, 1);
				unsigned short blaze_kills = getInput<unsigned short>("Select number of blaze kill per simulation (Dream: " + std::to_string(DREAM_BLAZES_KILLED) + ")", UINT16_MAX, 1);
				SimSaveFile newSimSave;
				newSimSave.barters = barters;
				newSimSave.blaze_kills = blaze_kills;
				newSimSave.ep_p = (ep_weight) / (439.f + ep_weight);
				newSimSave.br_p = br_p/100.f;
				_save.sim_save_num++;
				_save.sim_save.push_back(newSimSave);
				saveSaveFile();
				std::cout << "New simulation save created!" << std::endl;
			}
			else {
				return;
			}
		}
		else {
			std::cout << "Wrong code!" << std::endl;
		}
	}



}

void System::print_results(size_t save_index) {
	SimSaveFile c_save = _save.sim_save[save_index];
	std::cout << "---------------------------------" << std::endl;
	std::cout << "Simulations completed: " << c_save.sim_comp << std::endl;
	std::cout << "EP_P: " << c_save.ep_p * 100 << " %" << std::endl;
	std::cout << "BR_P: " << c_save.br_p * 100 << " %" << std::endl;
	std::cout << "BARTERS: " << c_save.barters << std::endl;
	std::cout << "BLAZE KILLS: " << c_save.blaze_kills << std::endl;
	std::cout << "Max succesfull barters: " << c_save.max_res.barters << " (GOAL: " << DREAM_BARTERS_SUCCES << ")" << std::endl;
	std::cout << "Max blaze rods received: " << c_save.max_res.br_received << " (GOAL: " << DREAM_BR_RECEIVED << ")" << std::endl;
	std::cout << "Max succesfull barters & blaze rods received at once: " << c_save.max_res.barters_comb << " EP / " << c_save.max_res.br_received << " BR";
	std::cout << " (GOAL: " << DREAM_BARTERS_SUCCES << " EP / " << DREAM_BR_RECEIVED << " )" << std::endl;
	if (c_save.sim_comp > 0) {
		std::cout << "Avarage succesfull barters: " << double(c_save.all_barters) / c_save.sim_comp << std::endl;
		std::cout << "Avarage blaze rods received: " << double(c_save.all_br_received) / c_save.sim_comp << std::endl;
	}
}

size_t System::selectSimulation()
{
	while (true) {
		std::cout << "---------------------------------" << std::endl;
		std::cout << "Select simulation save" << std::endl;
		std::cout << "1.) Default Simulation" << std::endl;
		for (int i = 1; i < _save.sim_save.size(); i++) {
			std::cout << i + 1 << ".) Custom simulation " << i << std::endl;
		}
		std::cout << "Input number of action: ";
		int res = 0;
		std::cin >> res;
		res--;

		if (res >= 0 && res < _save.sim_save.size()) {
			return res;
		}
		else {
			std::cout << "Wrong code!" << std::endl;
		}
	}
}


//void System::save(std::string sim_name) {
//	SaveFile save = {Simulation::_max_res,Simulation::sim_completed,Simulation::all_barters,Simulation::all_br_received};
//	std::fstream output(sim_name,std::ios::binary | std::ios::out);
//	output.write((char*)&save, sizeof(SaveFile));
//	output.close();
//	std::cout << "Results saved!" << std::endl;
//}
//
//SaveFile System::load(std::string sim_name) {
//	SaveFile save;
//	std::fstream input(sim_name, std::ios::binary | std::ios::in);
//	input.read((char*)&save,sizeof(SaveFile));
//	/*Simulation::_max_res = save.max_res;
//	Simulation::sim_completed = save.sim_comp;
//	Simulation::all_barters = save.all_barters;
//	Simulation::all_br_received = save.all_br_received;*/
//	input.close();
//	std::cout << "Results from "<<sim_name<<" loaded!" << std::endl;
//	return save;
//}

void System::clearSave()
{
	//clear results
	//Simulation::_max_res = { 0,0,0,0};
	//Simulation::sim_completed = 0;

	//clear save file
	std::fstream save("SaveFile.dat", std::ios::binary | std::ios::out);
	save << 1;
	SimSaveFile def_save;
	save.write((char*)&def_save, sizeof(SimSaveFile));
	save.close();
	_save.sim_save.clear();
	_save.sim_save.push_back(def_save);
}

bool System::loadSaveFile() {
	std::fstream save("SaveFile.dat", std::ios::binary | std::ios::in);
	if (save.is_open()) {
		std::cout << "Loading existing save file!" << std::endl;
		size_t sim_save_size;
		save >> sim_save_size;
		for (size_t i = 0; i < sim_save_size; i++) {
			SimSaveFile tmp_save;
			save.read((char*)&tmp_save, sizeof(SimSaveFile));
			_save.sim_save.push_back(tmp_save);
		}
		std::cout << sim_save_size << " saves loaded!" << std::endl;
		save.close();
		return true;
	}
	else {
		createSaveFile();
		save.close();
		return false;
	}
}

void System::saveSaveFile()
{
	std::fstream save("SaveFile.dat", std::ios::binary | std::ios::out);
	if (save.is_open()) {
		std::cout << "Saving save file!" << std::endl;
		save << _save.sim_save.size();
		for (size_t i = 0; i < _save.sim_save.size(); i++) {
			save.write((char*)&_save.sim_save[i], sizeof(SimSaveFile));
		}
		std::cout << "Save complate!" << std::endl;
		save.close();
	}
	else {
		std::cout << "Error: save file not accesible!" << std::endl;
	}
}

void System::createSaveFile()
{
	std::cout << "Creating new save file!" << std::endl;
	std::fstream save("SaveFile.dat", std::ios::binary | std::ios::out);
	save << 1;
	SimSaveFile def_save;
	save.write((char*)&def_save, sizeof(SimSaveFile));
	save.close();
}

template<class T>
T System::getInput(std::string text, T max, T min)
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
