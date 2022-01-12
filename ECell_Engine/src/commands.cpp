#include "commands.hpp"
#include "ECell_Engine.hpp"
#include "simulation_loop.hpp"


#pragma region Main Application Commands
void QuitCommand::Execute()
{
	receiver->isRunning = false;
}
#pragma endregion

#pragma region Simulation Loop Commands
void DisplayCommand::Execute()
{
	std::cout <<
		"Simulation state:" << std::endl <<

		"Total elapsed time: "<< receiver->simulationTimer.elapsedTime <<
		", last delta time: " << receiver->simulationTimer.deltaTime << std::endl;

}

void PauseCommand::Execute()
{
	//std::cout << "The PAUSE command was called" << std::endl;

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

void PlayCommand::Execute()
{
	//std::cout << "The PLAY command was called" << std::endl;

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

void StopCommand::Execute()
{
	//std::cout << "The STOP command was called" << std::endl;

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
	receiver->simulationTimer.ResetTimes();
}
#pragma endregion