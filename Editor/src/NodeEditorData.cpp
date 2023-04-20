#include "NodeEditorData.hpp"

void ECellEngine::Editor::Utility::AssetNodeData::InputUpdate(std::size_t& _nodeInputPinId, float& _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("AssetNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::AssetNodeData::OutputUpdate(std::size_t& _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("AssetNodeData::OutputUpdate");
	
	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("AssetNodeData::OutputUpdate");
}

void ECellEngine::Editor::Utility::ComputedParameterNodeData::InputUpdate(std::size_t& _nodeInputPinId, float& _data)
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

void ECellEngine::Editor::Utility::ReactionNodeData::InputUpdate(std::size_t& _nodeInputPinId, float& _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ReactionNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::ReactionNodeData::OutputUpdate(std::size_t& _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ReactionNodeData::OutputUpdate");

	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ReactionNodeData::OutputUpdate");
}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::InputUpdate(std::size_t& _nodeInputPinId, float& _data)
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

void ECellEngine::Editor::Utility::SolverNodeData::InputUpdate(std::size_t& _nodeInputPinId, float& _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SolverNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::SolverNodeData::OutputUpdate(std::size_t& _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SolverNodeData::OutputUpdate");

	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SolverNodeData::OutputUpdate; idx= " + std::to_string(idx));
}

void ECellEngine::Editor::Utility::SpeciesNodeData::InputUpdate(std::size_t& _nodeInputPinId, float& _data)
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