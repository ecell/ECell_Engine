#include "simulation_loop.hpp"
#include "commands_center.hpp"

class KeyboardInput
{
private:
	SimulationLoop* refSimulationLoop;
	std::string command;

public:
	void Start();

	KeyboardInput(SimulationLoop* _refSimuLoop) : refSimulationLoop(_refSimuLoop) {};
};