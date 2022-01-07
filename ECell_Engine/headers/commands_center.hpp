#include "string.h"
#include "simulation_loop.hpp"
#include "commands.hpp"

struct SimulationLoopCommands
{
public:
	PauseCommand pauseCommand;
	PlayCommand playCommand;
	StopCommand stopCommand;

	SimulationLoopCommands(SimulationLoop* _refSimuLoop) :
		pauseCommand(_refSimuLoop),
		playCommand(_refSimuLoop),
		stopCommand(_refSimuLoop)
	{
	};
};

class CommandsCenter
{
	SimulationLoopCommands simulationLoopCommands;

public:
	void Pointsman(std::string);

	CommandsCenter(SimulationLoop* _refSimuLoop) : simulationLoopCommands(_refSimuLoop) {};
};