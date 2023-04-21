#include "Engine.hpp"

#pragma region IOCommands

//void ECellEngine::IO::SetFilePathCommand::execute(const std::vector<std::string>& _args)
//{
//	//path = "GibsonAndBruckToyModel.xml";//added during dev time to bypass the time to write the name in the console everytime.
//	//path = "p53_L3V2_mod.xml";//added during dev time to bypass the time to write the name in the console everytime.
//	receiver->setTargetFilePath(_args[1]);
//}
#pragma endregion

#pragma region Engine Commands
//void ECellEngine::IO::AddFileAsSBMLCommand::execute(const std::vector<std::string>& _args)
//{
//	SBMLDocument* out = receiver->getFileIOManager()->tryOpenTargetFileAsSBML();
//	if (out != nullptr)
//	{
//		receiver->addSBMLDocument(out);
//		std::cout << "Open SBML file - SUCCESS." << std::endl;
//	}
//	else
//	{
//		std::cout << "Open SBML file - FAILED." << std::endl;
//	}
//}

//void ECellEngine::IO::AddSimulationTargetCommand::execute(const std::vector<std::string>& _args)
//{
//	receiver->forwardSimulationTarget(std::stoi(_args[1]));
//}

//void ECellEngine::IO::QuitCommand::execute(const std::vector<std::string>& _args)
//{
//	receiver->stop();
//}
#pragma endregion

#pragma region Simulation Loop Commands
//void ECellEngine::IO::DisplayCommand::execute(const std::vector<std::string>& _args)
//{
//	std::cout <<
//		"Simulation state:" << std::endl <<
//
//		"Total elapsed time: " << receiver->simulationTimer.elapsedTime <<
//		", last delta time: " << receiver->simulationTimer.deltaTime << std::endl <<
//		"Simulation Environment: " << *receiver->getSimulationEnvironment();
//}
//
//void ECellEngine::IO::GoBackwardCommand::execute(const std::vector<std::string>& _args)
//{
//	switch (*receiver->getSimulationDirection())
//	{
//	case(-1):
//		std::cout << "The simulation is already going backward." << std::endl;
//		break;
//	case(1):
//		receiver->getSimulationEnvironment()->reverseRNG();
//		receiver->setSimulationDirectionToBackward();
//		std::cout << "The simulation's direction is now set to backward." << std::endl;
//		break;
//	}
//}
//
//void ECellEngine::IO::GoForwardCommand::execute(const std::vector<std::string>& _args)
//{
//	switch (*receiver->getSimulationDirection())
//	{
//	case(1):
//		std::cout << "The simulation is already going forward." << std::endl;
//		break;
//	case(-1):
//		receiver->getSimulationEnvironment()->reverseRNG();
//		receiver->setSimulationDirectionToForward();
//		std::cout << "The simulation's direction is now set to forward." << std::endl;
//		break;
//	}
//}
//
//void ECellEngine::IO::PauseCommand::execute(const std::vector<std::string>& _args)
//{
//	//std::cout << "The PAUSE command was called" << std::endl;
//
//	switch (*receiver->getSimulationState())
//	{
//	case(SimulationState::isPlaying):
//		std::cout << "Pausing the simulation." << std::endl;
//		receiver->SetSimulationState(SimulationState::isPaused);
//		break;
//	case(SimulationState::isPaused):
//		std::cout << "The simulation is already paused." << std::endl;
//		break;
//	case(SimulationState::isStopped):
//		std::cout << "The simulation has not been launched yet." << std::endl;
//		break;
//	default:
//		break;
//	}
//}
//
//void ECellEngine::IO::PlayCommand::execute(const std::vector<std::string>& _args)
//{
//	//std::cout << "The PLAY command was called" << std::endl;
//
//	switch (*receiver->getSimulationState())
//	{
//	case(SimulationState::isPlaying):
//		std::cout << "The simulation is already playing." << std::endl;
//		break;
//	case(SimulationState::isPaused):
//		std::cout << "Resuming the simulation" << std::endl;
//		break;
//	case(SimulationState::isStopped):
//		std::cout << "Launching the simulation" << std::endl;
//
//		//start simulation time
//		receiver->simulationTimer.SetStartTime();
//		break;
//	default:
//		break;
//	}
//	receiver->SetSimulationState(SimulationState::isPlaying);
//}
//
//void ECellEngine::IO::StepBackwardCommand::execute(const std::vector<std::string>& _args)
//{
//	if (*receiver->getSimulationDirection() == 1)
//	{
//		receiver->getSimulationEnvironment()->reverseRNG();
//		receiver->setSimulationDirectionToBackward();
//	}
//	receiver->stepBackward(std::stof(_args[1]));
//}
//
//void ECellEngine::IO::StepForwardCommand::execute(const std::vector<std::string>& _args)
//{
//	if (*receiver->getSimulationDirection() == -1)
//	{
//		receiver->getSimulationEnvironment()->reverseRNG();
//		receiver->setSimulationDirectionToForward();
//	}
//	receiver->stepForward(std::stof(_args[1]));
//}
//
//void ECellEngine::IO::StopCommand::execute(const std::vector<std::string>& _args)
//{
//	//std::cout << "The STOP command was called" << std::endl;
//
//	switch (*receiver->getSimulationState())
//	{
//	case(SimulationState::isPlaying):
//		std::cout << "Stopping the simulation." << std::endl;
//		break;
//	case(SimulationState::isPaused):
//		std::cout << "Stopping the simulation." << std::endl;
//		break;
//	case(SimulationState::isStopped):
//		std::cout << "The simulation is already stopped." << std::endl;
//		break;
//	default:
//		break;
//	}
//
//	receiver->SetSimulationState(SimulationState::isStopped);
//	receiver->simulationTimer.ResetTimes();
//}
#pragma endregion

#pragma region Simulation Commands

bool ECellEngine::IO::AddModuleCommand::execute(const std::vector<std::string>& _args)
{

	if (receiver->CountSimulations() == 0)
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("AddModuleCommand Failed: There is no simulation currently managed so it is impossible to add a module.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver->CountSimulations())
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("AddModuleCommand Failed: Tried to add a module to a simulation that does not exist.");
		return false;
	}

	//TODO: check _args[2]
	receiver->GetSimulation(std::stoi(_args[1]))->AddModule(_args[2]);

	return true;
}

bool ECellEngine::IO::AddSolverCommand::execute(const std::vector<std::string>& _args)
{

	if (receiver->CountSimulations() == 0)
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("AddSolverCommand Failed: There is no simulation currently managed so it is impossible to add a solver.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver->CountSimulations())
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("AddSolverCommand Failed: Tried to add a solver to a simulation that does not exist.");
		return false;
	}

	//TODO: check _args[2]
	receiver->GetSimulation(std::stoi(_args[1]))->AddSolver(_args[2]);
}

bool ECellEngine::IO::PauseSimulationCommand::execute(const std::vector<std::string>& _args)
{
	if (receiver->CountPlayingSimulations() == 0)
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("PauseSimulationCommand Failed: There is no simulation currently playing.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver->CountPlayingSimulations())
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("PauseSimulationCommand Failed: Tried to pause a simulation that is not playing.");
		return false;
	}
	
	else // std::stoi(_args[1]) < receiver->CountPlayingSimulations()
	{
		receiver->PauseSimulation(std::stoi(_args[1]));
		return true;
	}
}

bool ECellEngine::IO::PlaySimulationCommand::execute(const std::vector<std::string>& _args)
{
	if (receiver->CountSimulations() == 0)
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("PlaySimulationCommand Failed: There is no simulation currently managed.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver->CountSimulations())
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("PlaySimulationCommand Failed: Tried to play a simulation that does not exist.");
		return false;
	}

	else //std::stoi(_args[1]) < receiver->CountSimulation()
	{
		receiver->PlaySimulation(std::stoi(_args[1]));
		return true;
	}
}

bool ECellEngine::IO::StepSimulationBackwardCommand::execute(const std::vector<std::string>& _args)
{
	//TODO
	return false;
}

bool ECellEngine::IO::StepSimulationForwardCommand::execute(const std::vector<std::string>& _args)
{
	return false;
	receiver->GetSimulation(std::stoi(_args[1]))->Update(std::stof(_args[2]));
}

bool ECellEngine::IO::StopSimulationCommand::execute(const std::vector<std::string>& _args)
{

	if (receiver->CountPlayingSimulations() == 0)
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("StopSimulationCommand Failed: There is no simulation currently playing.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver->CountPlayingSimulations())
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("StopSimulationCommand Failed: Tried to stop a simulation that is not playing.");
		return false;
	}

	else //std::stoi(_args[1]) < receiver->CountPlayingSimulations()
	{
		receiver->StopSimulation(std::stoi(_args[1]));
		return true;
	}
}

bool ECellEngine::IO::TryAttachSolverToModuleCommand::execute(const std::vector<std::string>& _args)
{
	//TODO: check _args[1], _args[2] & _args[3]

	if (receiver->CountSimulations() == 0)
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("TryAttachSolverToModuleCommand Failed: There is no simulation currently managed.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver->CountPlayingSimulations())
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("TryAttachSolverToModuleCommand Failed: Tried to access a simulation that does not exist.");
		return false;
	}

	std::size_t moduleIdx = receiver->GetSimulation(std::stoi(_args[1]))->FindModuleIdx(_args[3].c_str());
	std::size_t solverIdx = receiver->GetSimulation(std::stoi(_args[1]))->FindSolverIdx(_args[2].c_str());

	if (moduleIdx == SIZE_MAX)
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("TryAttachSolverToModuleCommand Failed: Could not find module with name \"" + _args[3] + "\" in the modules of simulation " + _args[1]);
		return false;
	}
	else if (solverIdx == SIZE_MAX)
	{
		ECellEngine::Logging::Logger::GetSingleton().LogError("TryAttachSolverToModuleCommand Failed: Could not find solver with name \"" + _args[3] + "\" in the solvers of simulation " + _args[1]);
		return false;
	}
	else
	{
		receiver->GetSimulation(std::stoi(_args[1]))->TryAttachSolverToModule(solverIdx, moduleIdx);
		return true;
	}

}

#pragma endregion
