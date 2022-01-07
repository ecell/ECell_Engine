#include <iostream>
#include "simulation_loop.hpp"
#include "command.hpp"

class PauseCommand : Command
{
public:
	virtual void Execute();

	PauseCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop){};

private:
	SimulationLoop* receiver;
};

class PlayCommand : Command
{
public:
	virtual void Execute();

	PlayCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};

class StopCommand : Command
{
public:
	virtual void Execute();

	StopCommand(SimulationLoop* _refSimuLoop) : receiver(_refSimuLoop) {};

private:
	SimulationLoop* receiver;
};