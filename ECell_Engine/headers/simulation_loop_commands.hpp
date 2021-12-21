#include "command.hpp"
#include "simulation_loop.hpp"

class PauseCommand : Command<SimulationLoop>
{
public:
	virtual void Execute();

	PauseCommand(SimulationLoop*);
};

class PlayCommand : Command<SimulationLoop>
{
public:
	virtual void Execute();

	PlayCommand(SimulationLoop*);
};

class StopCommand : Command<SimulationLoop>
{
public:
	virtual void Execute();

	StopCommand(SimulationLoop*);
};