#include "Widget/ModelExplorerWidget.hpp"
#include "IO/CommandsEditor.hpp"

bool ECellEngine::Editor::IO::AddMNBVContextCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("AddMNBVContextCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	std::size_t simulationID = 0;
	try
	{
		simulationID = std::stoll(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("AddMNBVContextCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a simulation", _args[1].c_str());
		return false;
	}

	args.simulationID = simulationID;

	return true;
}

bool ECellEngine::Editor::IO::AddMNBVContextCommand::Execute()
{
	std::pair<bool, std::vector<std::unique_ptr<Core::Simulation>>::iterator> simuSearch = ECellEngine::Core::SimulationsManager::GetSingleton().FindSimulation(args.simulationID);

	if (!simuSearch.first)
	{
		ECellEngine::Logging::Logger::LogError("PauseSimulationCommand Failed: Could not find simulation with ID \"%llu\".", args.simulationID);
		return false;
	}

	receiver.AddModelNodeBasedViewerContext(simuSearch.second->get());
}