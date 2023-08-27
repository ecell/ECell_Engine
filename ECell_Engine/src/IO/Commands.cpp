#include "Core/Engine.hpp"

#pragma region Simulation Commands

bool ECellEngine::IO::AddModuleCommand::Execute(const std::vector<std::string>& _args)
{
	if (receiver.CountSimulations() == 0)
	{
		ECellEngine::Logging::Logger::LogError("AddModuleCommand Failed: There is no simulation currently managed so it is impossible to add a module.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver.CountSimulations())
	{
		ECellEngine::Logging::Logger::LogError("AddModuleCommand Failed: Tried to add a module to a simulation that does not exist.");
		return false;
	}

	//TODO: check _args[2]
	receiver.GetSimulation(std::stoi(_args[1]))->AddModule(_args[2]);
	receiver.GetSimulation(std::stoi(_args[1]))->RefreshDependenciesDatabase();

	return true;
}

bool ECellEngine::IO::AddSolverCommand::Execute(const std::vector<std::string>& _args)
{

	if (receiver.CountSimulations() == 0)
	{
		ECellEngine::Logging::Logger::LogError("AddSolverCommand Failed: There is no simulation currently managed so it is impossible to add a solver.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver.CountSimulations())
	{
		ECellEngine::Logging::Logger::LogError("AddSolverCommand Failed: Tried to add a solver to a simulation that does not exist.");
		return false;
	}

	//TODO: check _args[2]
	receiver.GetSimulation(std::stoi(_args[1]))->AddSolver(_args[2]);
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

	Core::Simulation* simulation = receiver.FindSimulation(simulationID);

	if (simulation == nullptr)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not find simulation with ID \"%s\".", _args[1].c_str());
		return false;
	}

	std::size_t solverID = 0;
	try
	{
		solverID = std::stoll(_args[2]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not convert second argument \"%s\" to an integer to represent the ID of a solver", _args[2].c_str());
		return false;
	}

	std::size_t moduleID = 0;
	try
	{
		moduleID = std::stoll(_args[3]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not convert third argument \"%s\" to an integer to represent the ID of a module", _args[3].c_str());
		return false;
	}

	simulation->TryModuleSolverLink(solverID, moduleID);
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

	Core::Simulation* simulation = receiver.FindSimulation(simulationID);

	if (simulation == nullptr)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not find simulation with ID \"%s\".", _args[1].c_str());
		return false;
	}

	std::size_t solverID = 0;
	try
	{
		solverID = std::stoll(_args[2]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not convert second argument \"%s\" to an integer to represent the ID of a solver", _args[2].c_str());
		return false;
	}

	std::size_t moduleID = 0;
	try
	{
		moduleID = std::stoll(_args[3]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not convert third argument \"%s\" to an integer to represent the ID of a module", _args[3].c_str());
		return false;
	}

	simulation->RemoveModuleSolverLink(moduleID, solverID);
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
