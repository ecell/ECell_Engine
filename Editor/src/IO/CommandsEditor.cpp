#include "Util/StrUtil.hpp"
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

bool ECellEngine::Editor::IO::EraseMNBVContextCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("EraseMNBVContextCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	unsigned short contextIdx = 0;
	try
	{
		contextIdx = std::stoul(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("EraseMNBVContextCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a Model Node-Based Viewer (MNBV) Context", _args[1].c_str());
		return false;
	}

	args.contextIdx = contextIdx;

	return true;
}

bool ECellEngine::Editor::IO::EraseMNBVContextCommand::Execute()
{
	if (!receiver.EraseModelNodeBasedViewerContext(args.contextIdx))
	{
		ECellEngine::Logging::Logger::LogError("EraseMNBVContextCommand Failed: Could not erase the Model Node-Based Viewer (MNBV) Context at index %u.", args.contextIdx);
		return false;
	}

	return true;
}

bool ECellEngine::Editor::IO::EraseAllNodesOfTypeCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("EraseAllNodesOfTypeCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	unsigned short contextIdx = 0;
	try
	{
		contextIdx = std::stoul(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("EraseAllNodesOfTypeCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a Model Node-Based Viewer (MNBV) Context", _args[1].c_str());
		return false;
	}

	args.contextIdx = contextIdx;
	Util::StrCopy(args.nodeType, _args[2].c_str(), 32);

	return true;
}

bool ECellEngine::Editor::IO::EraseAllNodesOfTypeCommand::Execute()
{
	if (args.contextIdx >= receiver.CountModelNodeBasedViewerContext())
	{
		ECellEngine::Logging::Logger::LogError("EraseAllNodesOfTypeCommand Failed: The Model Node-Based Viewer (MNBV) Context at index %u does not exist.", args.contextIdx);
		return false;
	}

	if (!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseAllNodesOfType(args.nodeType))
	{
		ECellEngine::Logging::Logger::LogError("EraseAllNodesOfTypeCommand Failed: Could not erase all nodes of type \"%s\" in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeType, args.contextIdx);
		return false;
	}

	return true;
}

bool ECellEngine::Editor::IO::FocusNodeCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("FocusNodeCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	unsigned short contextIdx = 0;
	try
	{
		contextIdx = std::stoul(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("FocusNodeCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a Model Node-Based Viewer (MNBV) Context", _args[1].c_str());
		return false;
	}

	unsigned long long nodeID = 0;
	try
	{
		nodeID = std::stoull(_args[2]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("FocusNodeCommand Failed: Could not convert second argument \"%s\" to an integer to represent the ID of a node", _args[2].c_str());
		return false;
	}

	args.contextIdx = contextIdx;
	args.nodeID = nodeID;

	return true;
}

bool ECellEngine::Editor::IO::FocusNodeCommand::Execute()
{
	//First we check whether a viewer is currently displaying the context
	//at index args.contextIdx
	std::pair<bool, std::vector<unsigned short>::iterator> it = receiver.IsMNBVContextInUse(args.contextIdx);
	if (!it.first)
	{
		ECellEngine::Logging::Logger::LogError("FocusNodeCommand Failed: The Model Node-Based Viewer (MNBV) Context at index %u is used by any MNBV.", args.contextIdx);
	}

	//Then we try to focus on the node with ID args.nodeID in every viewer using the context
	if (!receiver.FocusNode(args.contextIdx, args.nodeID))
	{
		ECellEngine::Logging::Logger::LogError("FocusNodeCommand Failed: Could not focus on node with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
		return false;
	}

	return true;
}