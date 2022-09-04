#include "Commands.hpp"
#include "Engine.hpp"
#include "SimulationLoop.hpp"


#pragma region IOCommands

void SetFilePathCommand::execute(const std::vector<std::string>& _args)
{
	//path = "GibsonAndBruckToyModel.xml";//added during dev time to bypass the time to write the name in the console everytime.
	//path = "p53_L3V2_mod.xml";//added during dev time to bypass the time to write the name in the console everytime.
	receiver->setTargetFilePath(_args[1]);
}

void AddFileAsSBMLCommand::execute(const std::vector<std::string>& _args)
{
	SBMLDocument* out = receiver->getFileIOManager()->tryOpenTargetFileAsSBML();
	if (out != nullptr)
	{
		receiver->addSBMLDocument(out);
		std::cout << "Open SBML file - SUCCESS." << std::endl;
	}
	else
	{
		std::cout << "Open SBML file - FAILED." << std::endl;
	}
}
#pragma endregion

#pragma region Engine Commands
void QuitCommand::execute(const std::vector<std::string>& _args)
{
	receiver->stop();
}
#pragma endregion

#pragma region Simulation Loop Commands
void DisplayCommand::execute(const std::vector<std::string>& _args)
{
	std::cout <<
		"Simulation state:" << std::endl <<

		"Total elapsed time: " << receiver->simulationTimer.elapsedTime <<
		", last delta time: " << receiver->simulationTimer.deltaTime << std::endl <<
		"Simulation Environment: " << receiver->GetSimulationEnvironment();
}

void GoForwardCommand::execute(const std::vector<std::string>& _args)
{
	switch (receiver->getSimulationDirection())
	{
	case(1):
		std::cout << "The simulation is already going forward." << std::endl;
		break;
	case(-1):
		receiver->setSimulationDirectionToForward();
		std::cout << "The simulation's direction is now set to forward." << std::endl;
		break;
	}
}

void GoBackwardCommand::execute(const std::vector<std::string>& _args)
{
	switch (receiver->getSimulationDirection())
	{
	case(-1):
		std::cout << "The simulation is already going backward." << std::endl;
		break;
	case(1):
		receiver->setSimulationDirectionToBackward();
		std::cout << "The simulation's direction is now set to backward." << std::endl;
		break;
	}
}

void PauseCommand::execute(const std::vector<std::string>& _args)
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

void PlayCommand::execute(const std::vector<std::string>& _args)
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

void StopCommand::execute(const std::vector<std::string>& _args)
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