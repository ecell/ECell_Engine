#include <iostream>

#include "simulation_loop_commands.hpp"

PauseCommand::PauseCommand(SimulationLoop* _simulationLoop)
{
	receiver = _simulationLoop;
}

void PauseCommand::Execute()
{
	std::cout << "The PAUSE command was called" << std::endl;

	switch (receiver->GetSimulationState())
	{
	case(SimulationState::isPlaying):
		std::cout << "Pausing the simulation." << std::endl;
		receiver->SetSimulationState(SimulationState::isPaused);
		break;
	case(SimulationState::isPaused):
		std::cout << "The simulation is already paused." << std::endl;
		break;
	case(SimulationState::isStopped):
		std::cout << "The simulation has not been launched yet." << std::endl;
		break;
	default:
		break;
	}
}

PlayCommand::PlayCommand(SimulationLoop* _simulationLoop)
{
	receiver = _simulationLoop;
}

void PlayCommand::Execute()
{
	std::cout << "The PLAY command was called" << std::endl;

	switch (receiver->GetSimulationState())
	{
	case(SimulationState::isPlaying):
		std::cout << "The simulation is already playing." << std::endl;
		break;
	case(SimulationState::isPaused):
		std::cout << "Resuming the simulation" << std::endl;

		break;
	case(SimulationState::isStopped):
		std::cout << "Launching the simulation" << std::endl;

		//start simulation time
		receiver->simulationTimer.SetStartTime();
		break;
	default:
		break;
	}
	receiver->SetSimulationState(SimulationState::isPlaying);
}

StopCommand::StopCommand(SimulationLoop* _simulationLoop)
{
	receiver = _simulationLoop;
}

void StopCommand::Execute()
{
	std::cout << "The STOP command was called" << std::endl;

	switch (receiver->GetSimulationState())
	{
	case(SimulationState::isPlaying):
		std::cout << "Stopping the simulation." << std::endl;
		break;
	case(SimulationState::isPaused):
		std::cout << "Stopping the simulation." << std::endl;
		break;
	case(SimulationState::isStopped):
		std::cout << "The simulation is already stopped." << std::endl;
		break;
	default:
		break;
	}

	receiver->SetSimulationState(SimulationState::isStopped);
}