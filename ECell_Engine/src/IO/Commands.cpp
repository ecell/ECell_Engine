#include "Core/Engine.hpp"

#pragma region Simulation Commands

bool ECellEngine::IO::AddModuleCommand::Execute(const std::vector<std::string>& _args)
{
	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("AddModuleCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", _args[1].c_str());
		return false;
	}

	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("AddModuleCommand Failed: Could not find simulation with ID \"%s\".", _args[1].c_str());
		return false;
	}

	if ((*simuSearch.second)->AddModule(_args[2]) == nullptr)
	{
		ECellEngine::Logging::Logger::LogError("AddModuleCommand Failed: Could not import module \"%s\"", _args[2].c_str());
		return false;
	}

	(*simuSearch.second)->RefreshDependenciesDatabase();
	(*simuSearch.second)->GetModules().back()->SetName(const_cast<char*>(_args[3].c_str()));
	return true;
}

bool ECellEngine::IO::AddSolverCommand::Execute(const std::vector<std::string>& _args)
{
	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("AddSolverCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", _args[1].c_str());
		return false;
	}

	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("AddSolverCommand Failed: Could not find simulation with ID \"%s\".", _args[1].c_str());
		return false;
	}

	if ((*simuSearch.second)->AddSolver(_args[2]) == nullptr)
	{
		ECellEngine::Logging::Logger::LogError("AddSolverCommand Failed: Could not import solver \"%s\"", _args[2].c_str());
		return false;
	}

	return true;
}

bool ECellEngine::IO::ModuleSolverConnectionCommand::Execute(const std::vector<std::string>& _args)
{
	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", _args[1].c_str());
		return false;
	}

	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not find simulation with ID \"%s\".", _args[1].c_str());
		return false;
	}

	std::size_t moduleID = 0;
	try
	{
		moduleID = std::stoll(_args[2]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not convert second argument \"%s\" to an integer to represent the ID of a module", _args[2].c_str());
		return false;
	}

	std::size_t solverID = 0;
	try
	{
		solverID = std::stoll(_args[3]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not convert third argument \"%s\" to an integer to represent the ID of a solver", _args[3].c_str());
		return false;
	}

	if ((*simuSearch.second)->TryModuleSolverLink(moduleID, solverID))
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed : Could not link module \"%s\" and solver \"%s\".", _args[2].c_str(), _args[3].c_str());
		return false;
	}
	return true;
}

bool ECellEngine::IO::ModuleSolverDisconnectionCommand::Execute(const std::vector<std::string>& _args)
{
	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", _args[1].c_str());
		return false;
	}

	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not find simulation with ID \"%s\".", _args[1].c_str());
		return false;
	}

	std::size_t moduleID = 0;
	try
	{
		moduleID = std::stoll(_args[2]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not convert second argument \"%s\" to an integer to represent the ID of a module", _args[2].c_str());
		return false;
	}

	std::size_t solverID = 0;
	try
	{
		solverID = std::stoll(_args[3]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not convert third argument \"%s\" to an integer to represent the ID of a solver", _args[3].c_str());
		return false;
	}

	if ((*simuSearch.second)->RemoveModuleSolverLink(moduleID, solverID))
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed : Could not unlink module \"%s\" and solver \"%s\".", _args[2].c_str(), _args[3].c_str());
		return false;
	}
	return true;
}

bool ECellEngine::IO::PauseSimulationCommand::Execute(const std::vector<std::string>& _args)
{
	if (receiver.CountPlayingSimulations() == 0)
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: There is no simulation currently playing.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver.CountPlayingSimulations())
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Tried to pause a simulation that is not playing.");
		return false;
	}
	
	// std::stoi(_args[1]) < receiver.CountPlayingSimulations()
	receiver.PauseSimulation(std::stoi(_args[1]));
	return true;
}

bool ECellEngine::IO::PlaySimulationCommand::Execute(const std::vector<std::string>& _args)
{
	if (receiver.CountSimulations() == 0)
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: There is no simulation currently managed.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver.CountSimulations())
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: Tried to play a simulation that does not exist.");
		return false;
	}

	//std::stoi(_args[1]) < receiver.CountSimulation()
	receiver.PlaySimulation(std::stoi(_args[1]));
	return true;
}

bool ECellEngine::IO::StepSimulationBackwardCommand::Execute(const std::vector<std::string>& _args)
{
	//TODO
	return false;
}

bool ECellEngine::IO::StepSimulationForwardCommand::Execute(const std::vector<std::string>& _args)
{
	if (receiver.CountSimulations() == 0)
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: There is no simulation currently managed.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver.CountSimulations())
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: Tried to play a simulation that does not exist.");
		return false;
	}

	//std::stoi(_args[1]) < receiver.CountSimulation()
	receiver.GetSimulation(std::stoi(_args[1]))->Update(std::stof(_args[2]));
	return true;

	return false;
}

bool ECellEngine::IO::StopSimulationCommand::Execute(const std::vector<std::string>& _args)
{

	if (receiver.CountPlayingSimulations() == 0)
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: There is no simulation currently playing.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver.CountPlayingSimulations())
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: Tried to stop a simulation that is not playing.");
		return false;
	}

	//std::stoi(_args[1]) < receiver.CountPlayingSimulations()
	receiver.StopSimulation(std::stoi(_args[1]));
	return true;
}

#pragma endregion
