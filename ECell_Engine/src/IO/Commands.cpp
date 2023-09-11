#include "Core/Engine.hpp"

#pragma region Simulation Commands

bool ECellEngine::IO::AddModuleCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("AddModuleCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

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

	args.simulationID = simulationID;
	args.modulePath = const_cast<char*>(_args[2].c_str());
	args.moduleName = const_cast<char*>(_args[3].c_str());
	return true;
}

bool ECellEngine::IO::AddModuleCommand::Execute()
{
	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(args.simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("AddModuleCommand Failed: Could not find simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	if ((*simuSearch.second)->AddModule(args.modulePath) == nullptr)
	{
		ECellEngine::Logging::Logger::LogError("AddModuleCommand Failed: Could not import module \"%s\"", args.modulePath);
		return false;
	}

	(*simuSearch.second)->RefreshDependenciesDatabase();
	(*simuSearch.second)->GetModules().back()->SetName(args.moduleName);
	return true;
}

bool ECellEngine::IO::AddSimulationCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("AddSimulationCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	return true;
}

bool ECellEngine::IO::AddSimulationCommand::Execute()
{
	if (receiver.AddSimulation() == nullptr)
	{
		return false;
	}
	
	return true;
}

bool ECellEngine::IO::AddSolverCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("AddSolverCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

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

	args.simulationID = simulationID;
	args.solverName = const_cast<char*>(_args[2].c_str());

	return true;
}

bool ECellEngine::IO::AddSolverCommand::Execute()
{
	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(args.simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("AddSolverCommand Failed: Could not find simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	if ((*simuSearch.second)->AddSolver(args.solverName) == nullptr)
	{
		ECellEngine::Logging::Logger::LogError("AddSolverCommand Failed: Could not import solver \"%s\"", args.solverName);
		return false;
	}

	return true;
}

bool ECellEngine::IO::ModuleSolverConnectionCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

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

	args.simulationID = simulationID;
	args.moduleID = moduleID;
	args.solverID = solverID;

	return true;
}

bool ECellEngine::IO::ModuleSolverConnectionCommand::Execute()
{
	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(args.simulationID);

	//Search for the module in the ::modules list
	std::pair<bool, std::vector<std::shared_ptr<Data::Module>>::iterator> moduleSearch = (*simuSearch.second)->FindModule(args.moduleID);
	if (!moduleSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not find module with ID \"%llu\".", args.moduleID);
		return false;
	}

	//Search for the solver in the ::solvers list
	std::pair<bool, std::vector<std::shared_ptr<Solvers::Solver>>::iterator> solverSearch = (*simuSearch.second)->FindSolver(args.solverID);
	if (!solverSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Could not find solver with ID \"%llu\".", args.solverID);
		return false;
	}

	//Check if the solver is compatible with the module
	if (!(*moduleSearch.second)->IsValidSolverType(solverSearch.second->get()))
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Solver \"%llu\" is not compatible with module \"%llu\".", args.solverID, args.moduleID);
		return false;
	}

	//We search for the lower bound where to insert the pair <moduleIdx, solverIdx> in ::moduleSolverLinks
	std::size_t moduleIdx = std::distance((*simuSearch.second)->GetModules().begin(), moduleSearch.second);
	std::size_t solverIdx = std::distance((*simuSearch.second)->GetSolvers().begin(), solverSearch.second);
	std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator> linkSearch = (*simuSearch.second)->FindModuleSolverLink(moduleIdx, solverIdx);
	
	//Check if the link already exists
	if (linkSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverConnectionCommand Failed: Link between module \"%llu\" and solver \"%llu\" already exists.", args.moduleID, args.solverID);
		return false;
	}

	(*simuSearch.second)->InsertModuleSolverLink(linkSearch.second, std::pair<std::size_t, std::size_t>(moduleIdx, solverIdx));
	return true;
}

bool ECellEngine::IO::ModuleSolverDisconnectionCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

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

	args.simulationID = simulationID;
	args.moduleID = moduleID;
	args.solverID = solverID;

	return true;
}

bool ECellEngine::IO::ModuleSolverDisconnectionCommand::Execute()
{
	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(args.simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not find simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	//Search for the module in the ::modules list
	std::pair<bool, std::vector<std::shared_ptr<Data::Module>>::iterator> moduleSearch = (*simuSearch.second)->FindModule(args.moduleID);
	if (!moduleSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not find module with ID \"%llu\".", args.moduleID);
		return false;
	}

	//Search for the solver in the ::solvers list
	std::pair<bool, std::vector<std::shared_ptr<Solvers::Solver>>::iterator> solverSearch = (*simuSearch.second)->FindSolver(args.solverID);
	if (!solverSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not find solver with ID \"%llu\".", args.solverID);
		return false;
	}

	//Search for the link in the ::moduleSolverLinks list
	std::size_t moduleIdx = std::distance((*simuSearch.second)->GetModules().begin(), moduleSearch.second);
	std::size_t solverIdx = std::distance((*simuSearch.second)->GetSolvers().begin(), solverSearch.second);
	std::pair<bool, std::vector<std::pair<std::size_t, std::size_t>>::iterator> linkSearch = (*simuSearch.second)->FindModuleSolverLink(moduleIdx, solverIdx);
	
	//Check if the link exists
	if (!linkSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("ModuleSolverDisconnectionCommand Failed: Could not find link between module \"%llu\" and solver \"%llu\".", args.moduleID, args.solverID);
		return false;
	}

	(*simuSearch.second)->RemoveModuleSolverLink(linkSearch.second);
	return true;
}

bool ECellEngine::IO::PauseSimulationCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", _args[1].c_str());
		return false;
	}

	args.simulationID = simulationID;

	return true;
}

bool ECellEngine::IO::PauseSimulationCommand::Execute()
{
	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(args.simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Could not find simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	//Search for playing simulation with ID simulationID
	std::pair<bool, std::vector<Core::Simulation*>::iterator> playingSimuSearch = receiver.FindPlayingSimulation(args.simulationID);
	if (!playingSimuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Could not find a playing simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	if (!receiver.PauseSimulation(playingSimuSearch.second))
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Could not pause simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	return true;
}

bool ECellEngine::IO::PlaySimulationCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", _args[1].c_str());
		return false;
	}

	args.simulationID = simulationID;

	return true;
}

bool ECellEngine::IO::PlaySimulationCommand::Execute()
{
	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(args.simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: Could not find simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	if (!receiver.PlaySimulation(simuSearch.second))
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulationCommand Failed: Could not play simulation with ID \"%llu\".", args.simulationID);
		return false;
	}
	return true;
}

bool ECellEngine::IO::StepSimulationBackwardCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("StepSimulationBackwardCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("StepSimulationBackwardCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", _args[1].c_str());
		return false;
	}

	float deltaTime = 0.f;
	try
	{
		deltaTime = std::stof(_args[2]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("StepSimulationBackwardCommand Failed: Could not convert second argument \"%s\" to a float to represent the delta time of the update loop", _args[2].c_str());
		return false;
	}

	args.simulationID = simulationID;
	args.deltaTime = deltaTime;

	return false;
}

bool ECellEngine::IO::StepSimulationBackwardCommand::Execute()
{
	//TODO
	return false;
}

bool ECellEngine::IO::StepSimulationForwardCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("StepSimulationForwardCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("StepSimulationForwardCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", _args[1].c_str());
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

	args.simulationID = simulationID;
	args.deltaTime = deltaTime;

	return true;
}

bool ECellEngine::IO::StepSimulationForwardCommand::Execute()
{
	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(args.simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("StepSimulationForwardCommand Failed: Could not find simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	(*simuSearch.second)->Update(args.deltaTime);
	return true;
}

bool ECellEngine::IO::StopSimulationCommand::DecodeParameters(const std::vector<std::string>& _args)
{

	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", _args[1].c_str());
		return false;
	}

	args.simulationID = simulationID;

	return true;
}

bool ECellEngine::IO::StopSimulationCommand::Execute()
{
	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = receiver.FindSimulation(args.simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: Could not find simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	//Search for playing simulation with ID simulationID
	std::pair<bool, std::vector<Core::Simulation*>::iterator> playingSimuSearch = receiver.FindPlayingSimulation(args.simulationID);
	if (!playingSimuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: Could not find a playing simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	if (!receiver.StopSimulation(playingSimuSearch.second))
	{
		ECellEngine::Logging::Logger::LogError("StopSimulationCommand Failed: Could not stop simulation with ID \"%llu\".", args.simulationID);
		return false;
	}
	return true;
}

#pragma endregion
