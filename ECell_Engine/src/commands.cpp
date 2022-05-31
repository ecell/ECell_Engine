#include "commands.hpp"
#include "ECell_Engine.hpp"
#include "simulation_loop.hpp"


#pragma region IOCommands
void OpenCommand::AskUser()
{
	std::cout << std::endl << "Give file name: ";
	std::cin >> answer;
	//answer = "GibsonAndBruckToyModel.xml";//added during dev time to bypass the time to write the name in the console everytime.
	answer = "p53_L3V2_mod.xml";//added during dev time to bypass the time to write the name in the console everytime.
}

void OpenCommand::Execute()
{
	AskUser();
	receiver->OpenFile(&answer);
}
#pragma endregion

#pragma region Engine Commands
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
		", last delta time: " << receiver->simulationTimer.deltaTime << std::endl <<
		"Simulation Environment: " << receiver->GetSimulationEnvironment();

}

void LoadCommand::Execute()
{
	std::cout << "Loading active SBML document... ";
	receiver->SetSimulationEnvironment();
	std::cout << "Done" << std::endl;
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