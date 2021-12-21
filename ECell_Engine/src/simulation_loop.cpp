#include <iostream>

#include "simulation_loop.hpp"
#include "simulation_loop_commands.hpp"

// Declaring accessors of SimulationLoop
SimulationState SimulationLoop::GetSimulationState()
{
	return this->simulationState;
}

// Declaring logic of SimulationLoop
void SimulationLoop::LoopCommandCenter(std::string _command)
{
	if (_command == "pause")
	{
		PauseCommand pauseCommand(this);
		pauseCommand.Execute();
	}

	else if (_command == "play")
	{
		PlayCommand playCommand(this);
		playCommand.Execute();
	}

	else if (_command == "stop")
	{
		StopCommand stopCommand(this);
		stopCommand.Execute();
	}

	else
	{
		std::cout << "The command:" << _command << " was not recognized" << std::endl;
	}
}

//Declaring mutators of SimulationLoop
void SimulationLoop::SetSimulationState(SimulationState _simulationState)
{
	this->simulationState = _simulationState;
}
