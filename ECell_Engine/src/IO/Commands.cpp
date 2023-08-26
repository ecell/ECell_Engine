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

bool ECellEngine::IO::TryModuleSolverLinkCommand::Execute(const std::vector<std::string>& _args)
{
	//TODO: check _args[1], _args[2] & _args[3]

	if (receiver.CountSimulations() == 0)
	{
		ECellEngine::Logging::Logger::LogError("TryModuleSolverLinkCommand Failed: There is no simulation currently managed.");
		return false;
	}

	if (std::stoi(_args[1]) >= receiver.CountSimulations())
	{
		ECellEngine::Logging::Logger::LogError("TryModuleSolverLinkCommand Failed: Tried to access a simulation that does not exist.");
		return false;
	}

	/*std::size_t moduleIdx = receiver.GetSimulation(std::stoi(_args[1]))->FindModuleIdx(std::stoi(_args[3]));
	std::size_t solverIdx = receiver.GetSimulation(std::stoi(_args[1]))->FindSolverIdx(std::stoi(_args[2]));

	if (moduleIdx == SIZE_MAX)
	{
		ECellEngine::Logging::Logger::LogError("TryModuleSolverLinkCommand Failed: Could not find module with ID \"%s\" in the modules of simulation %s", _args[3].c_str(), _args[1].c_str());
		return false;
	}

	if (solverIdx == SIZE_MAX)
	{
		ECellEngine::Logging::Logger::LogError("TryModuleSolverLinkCommand Failed: Could not find solver with name \"%s\" in the solvers of simulation %s", _args[3].c_str(), _args[1].c_str());
		return false;
	}*/

	receiver.GetSimulation(std::stoi(_args[1]))->TryModuleSolverLink(std::stoi(_args[2]), std::stoi(_args[3]));
	return true;

}

#pragma endregion
