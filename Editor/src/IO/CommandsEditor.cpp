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

	if (!strcmp(args.nodeType, "Equation"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::EquationNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "Parameter"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::ParameterNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "Reaction"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::ReactionNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "Species"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::SpeciesNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "Arithmetic"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::ArithmeticOperationNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "Asset"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::AssetNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "LinePlot"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::LinePlotNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "Logic"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::LogicOperationNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "ModifyDataStateValueEvent"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::ModifyDataStateValueEventNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "Solver"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::SolverNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "Time"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::TimeNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "Trigger"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::TriggerNodeData>().clear();
	}
	else if (!strcmp(args.nodeType, "ValueFloat"))
	{
		receiver.GetModelNodeBasedViewerContext(args.contextIdx).GetNodesOfType<Utility::MNBV::ValueFloatNodeData>().clear();
	}
	else
	{
		ECellEngine::Logging::Logger::LogError("EraseAllNodesOfTypeCommand Failed: Invalid node type \"%s\".", args.nodeType);
		return false;
	}

	return true;
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

bool ECellEngine::Editor::IO::EraseNodeCommand::DecodeParameters(const std::vector<std::string>& _args)
{
	if ((unsigned char)_args.size() != nbArgs)
	{
		ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Wrong number of arguments. Expected %llu, got %u.", nbArgs, _args.size());
		return false;
	}

	unsigned short contextIdx = 0;
	try
	{
		contextIdx = std::stoul(_args[1]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not convert first argument \"%s\" to an integer to represent the ID of a Model Node-Based Viewer (MNBV) Context", _args[1].c_str());
		return false;
	}

	unsigned long long nodeID = 0;
	try
	{
		nodeID = std::stoull(_args[3]);
	}
	catch (const std::invalid_argument& _e)
	{
		ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not convert third argument \"%s\" to an integer to represent the ID of a node", _args[3].c_str());
		return false;
	}

	args.contextIdx = contextIdx;
	Util::StrCopy(args.nodeType, _args[2].c_str(), 32);
	args.nodeID = nodeID;

	return true;
}

bool ECellEngine::Editor::IO::EraseNodeCommand::Execute()
{
	if (args.contextIdx >= receiver.CountModelNodeBasedViewerContext())
	{
		ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: The Model Node-Based Viewer (MNBV) Context at index %u does not exist.", args.contextIdx);
		return false;
	}

	if (!strcmp(args.nodeType, "Equation"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::EquationNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"Equation\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false;
		}
	}
	else if (!strcmp(args.nodeType, "Parameter"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::ParameterNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"Parameter\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false;
		}
	}
	else if (!strcmp(args.nodeType, "Reaction"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::ReactionNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"Reaction\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false;
		}
	}
	else if (!strcmp(args.nodeType, "Species"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::SpeciesNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"Species\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false;
		}
	}
	else if (!strcmp(args.nodeType, "Arithmetic"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::ArithmeticOperationNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"Arithmetic\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false;
		}
	}
	else if (!strcmp(args.nodeType, "Asset"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::AssetNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"Asset\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false;
		}
	}
	else if (!strcmp(args.nodeType, "LinePlot"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::LinePlotNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"LinePlot\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false;
		}
	}
	else if (!strcmp(args.nodeType, "Logic"))
	{
		if (!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::LogicOperationNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"Logic\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false;
		}
	}
	else if (!strcmp(args.nodeType, "ModifyDataStateValueEvent"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::ModifyDataStateValueEventNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"ModifyDataStateValueEvent\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false;
		}
	}
	else if (!strcmp(args.nodeType, "Solver"))
	{
		if (!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::SolverNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"Solver\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false;
		}
	}
	else if (!strcmp(args.nodeType, "Time"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::TimeNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"Time\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false; 
		}
	}
	else if (!strcmp(args.nodeType, "Trigger"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::TriggerNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"Trigger\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false; 
		}
	}
	else if (!strcmp(args.nodeType, "ValueFloat"))
	{
		if(!receiver.GetModelNodeBasedViewerContext(args.contextIdx).EraseNodeOfType<Utility::MNBV::ValueFloatNodeData>(args.nodeID))
		{
			ECellEngine::Logging::Logger::LogError("EraseNodeCommand Failed: Could not erase node of type \"ValueFloat\" with ID %llu in the Model Node-Based Viewer (MNBV) Context at index %u.", args.nodeID, args.contextIdx);
			return false; 
		}
	}
	else
	{
		ECellEngine::Logging::Logger::LogError("EraseAllNodesOfTypeCommand Failed: Invalid node type \"%s\".", args.nodeType);
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