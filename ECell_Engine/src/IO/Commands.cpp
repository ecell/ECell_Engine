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
		ECellEngine::Logging::Logger::LogError("AddModuleCommand Failed: Could not find simulation with ID \"%llu\".", simulationID);
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
		ECellEngine::Logging::Logger::LogError("AddSolverCommand Failed: Could not find simulation with ID \"%llu\".", simulationID);
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
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not find simulation with ID \"%llu\".", simulationID);
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

	//Search for the module in the ::modules list
	std::pair<bool, std::vector<std::shared_ptr<Data::Module>>::iterator> moduleSearch = (*simuSearch.second)->FindModule(moduleID);
	if (!moduleSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not find module with ID \"%llu\".", moduleID);
		return false;
	}

	//Search for the solver in the ::solvers list
	std::pair<bool, std::vector<std::shared_ptr<Solvers::Solver>>::iterator> solverSearch = (*simuSearch.second)->FindSolver(solverID);
	if (!solverSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not find solver with ID \"%llu\".", solverID);
		return false;
	}

	//Check if the solver is compatible with the module
	if (!(*moduleSearch.second)->IsValidSolverType(solverSearch.second->get()))
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Solver \"%llu\" is not compatible with module \"%llu\".", solverID, moduleID);
		return false;
	}

	//We search for the lower bound where to insert the pair <moduleIdx, solverIdx> in ::moduleSolverLinks
	std::size_t moduleIdx = std::distance((*simuSearch.second)->GetModules().begin(), moduleSearch.second);
	std::size_t solverIdx = std::distance((*simuSearch.second)->GetSolvers().begin(), solverSearch.second);
	std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator> linkSearch = (*simuSearch.second)->FindModuleSolverLink(moduleIdx, solverIdx);
	
	//Check if the link already exists
	if (linkSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Link between module \"%llu\" and solver \"%llu\" already exists.", moduleID, solverID);
		return false;
	}

	(*simuSearch.second)->InsertModuleSolverLink(linkSearch.second, std::pair<std::size_t, std::size_t>(moduleIdx, solverIdx));
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
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not find simulation with ID \"%llu\".", simulationID);
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

	//Search for the module in the ::modules list
	std::pair<bool, std::vector<std::shared_ptr<Data::Module>>::iterator> moduleSearch = (*simuSearch.second)->FindModule(moduleID);
	if (!moduleSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not find module with ID \"%llu\".", moduleID);
		return false;
	}

	//Search for the solver in the ::solvers list
	std::pair<bool, std::vector<std::shared_ptr<Solvers::Solver>>::iterator> solverSearch = (*simuSearch.second)->FindSolver(solverID);
	if (!solverSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not find solver with ID \"%llu\".", solverID);
		return false;
	}

	//Search for the link in the ::moduleSolverLinks list
	std::size_t moduleIdx = std::distance((*simuSearch.second)->GetModules().begin(), moduleSearch.second);
	std::size_t solverIdx = std::distance((*simuSearch.second)->GetSolvers().begin(), solverSearch.second);
	std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator> linkSearch = (*simuSearch.second)->FindModuleSolverLink(moduleIdx, solverIdx);
	
	//Check if the link exists
	if (!linkSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not find link between module \"%llu\" and solver \"%llu\".", moduleID, solverID);
		return false;
	}

	(*simuSearch.second)->RemoveModuleSolverLink(linkSearch.second);
	return true;
}

bool ECellEngine::IO::PauseSimulationCommand::Execute(const std::vector<std::string>& _args)
{
	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", simulationID);
		return false;
	}

	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Could not find simulation with ID \"%llu\".", simulationID);
		return false;
	}

	//Search for playing simulation with ID simulationID
	std::pair<bool, std::vector<Core::Simulation*>::iterator> playingSimuSearch = receiver.FindPlayingSimulation(simulationID);
	if (!playingSimuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Could not find a playing simulation with ID \"%llu\".", simulationID);
		return false;
	}

	if (!receiver.PauseSimulation(playingSimuSearch.second))
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Could not pause simulation with ID \"%llu\".", simulationID);
		return false;
	}

	return true;
}

bool ECellEngine::IO::PlaySimulationCommand::Execute(const std::vector<std::string>& _args)
{
	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", simulationID);
		return false;
	}

	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: Could not find simulation with ID \"%llu\".", simulationID);
		return false;
	}

	if (!receiver.PlaySimulation(simuSearch.second))
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: Could not play simulation with ID \"%llu\".", simulationID);
		return false;
	}
	return true;
}

bool ECellEngine::IO::StepSimulationBackwardCommand::Execute(const std::vector<std::string>& _args)
{
	//TODO
	return false;
}

bool ECellEngine::IO::StepSimulationForwardCommand::Execute(const std::vector<std::string>& _args)
{
	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("StepSimulationForwardCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", simulationID);
		return false;
	}

	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("StepSimulationForwardCommand Failed: Could not find simulation with ID \"%llu\".", simulationID);
		return false;
	}

	float deltaTime = 0.f;
	try
	{
		deltaTime = std::stof(_args[2]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("StepSimulationForwardCommand Failed: Could not convert second argument \"%s\" to a float to represent the delta time of the update loop", deltaTime);
		return false;
	}

	(*simuSearch.second)->Update(deltaTime);
	return true;
}

bool ECellEngine::IO::StopSimulationCommand::Execute(const std::vector<std::string>& _args)
{

	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", simulationID);
		return false;
	}

	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: Could not find simulation with ID \"%llu\".", simulationID);
		return false;
	}

	//Search for playing simulation with ID simulationID
	std::pair<bool, std::vector<Core::Simulation*>::iterator> playingSimuSearch = receiver.FindPlayingSimulation(simulationID);
	if (!playingSimuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: Could not find a playing simulation with ID \"%llu\".", simulationID);
		return false;
	}

	if (!receiver.StopSimulation(playingSimuSearch.second))
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: Could not stop simulation with ID \"%llu\".", simulationID);
		return false;
	}
	return true;
}

#pragma endregion
