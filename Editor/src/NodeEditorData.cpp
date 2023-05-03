#include "NodeEditorData.hpp"


void ECellEngine::Editor::Utility::AssetNodeData::InputUpdate(std::size_t& _nodeInputPinId, char* _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("AssetNodeData::InputUpdate; data=" + std::string(_data));

	NodeInputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(inputPins, inputPins + std::size(inputPins), _nodeInputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(inputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SolverNodeData::OutputUpdate; idx= " + std::to_string(idx));

	//The node input pin representing the solver attached to this asset.
	if (idx == 0)
	{
		//In this case, the parameter _data is the name of the solver node that is attempting to attach.
		//So, we know that we have the action to perform is to queue the TryAttachSolverToModuleCommand
		//of the engine.
		//This command will be process after the current context has been entirely draw in this frame.
		QueueEngineTASToMCmd(data->GetName(), _data);
	}
}

void ECellEngine::Editor::Utility::AssetNodeData::OutputUpdate(std::size_t& _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("AssetNodeData::OutputUpdate");
	
	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("AssetNodeData::OutputUpdate");
}

void ECellEngine::Editor::Utility::LinePlotNodeData::InputUpdate(std::size_t& _nodeInputId, float _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("LinePlotNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::ComputedParameterNodeData::InputUpdate(std::size_t& _nodeInputPinId, float _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ComputedParameterNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::ComputedParameterNodeData::OutputUpdate(std::size_t& _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ComputedParameterNodeData::OutputUpdate");
	
	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ComputedParameterNodeData::OutputUpdate");
}

void ECellEngine::Editor::Utility::ReactionNodeData::OutputUpdate(std::size_t& _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ReactionNodeData::OutputUpdate");

	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ReactionNodeData::OutputUpdate");
}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::InputUpdate(std::size_t& _nodeInputPinId, float _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SimpleParameterNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::OutputUpdate(std::size_t& _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SimpleParameterNodeData::OutputUpdate");

	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SimpleParameterNodeData::OutputUpdate");
}

void ECellEngine::Editor::Utility::SolverNodeData::OutputUpdate(std::size_t& _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SolverNodeData::OutputUpdate");

	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SolverNodeData::OutputUpdate; idx= " + std::to_string(idx));

	//The node output pin representing the solver.
	//It is used to attach a solver to an asset.
	if (idx == 0)
	{
		//The Solver node send his name to the asset node.
		//It is the Asset node, in its InputUpdate() that will Queue the command to 
		//try to attach the Solver to the Asset.
		it->Broadcast(data->GetName());
	}
}

void ECellEngine::Editor::Utility::SpeciesNodeData::InputUpdate(std::size_t& _nodeInputPinId, float _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SpeciesNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::SpeciesNodeData::OutputUpdate(std::size_t& _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SpeciesNodeData::OutputUpdate");

	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SpeciesNodeData::OutputUpdate");
}