#include "NodeEditorData.hpp"


void ECellEngine::Editor::Utility::AssetNodeData::InputUpdate(std::size_t _nodeInputPinId, char* _data)
{
	NodeInputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(inputPins, inputPins + std::size(inputPins), _nodeInputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(inputPins, it);

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


void ECellEngine::Editor::Utility::ComputedParameterNodeData::OutputConnect(std::size_t _nodeOutputPinId)
{
	//Computed parameter operation value
	if (_nodeOutputPinId == (std::size_t)outputPins[8].id)
	{
		//we set the input pin of the data field collapsing header as the fall back
		GetLinks()->back().OverrideStartFallbackPin(outputPins[3].id, 1);
	}
}

void ECellEngine::Editor::Utility::ComputedParameterNodeData::OutputUpdate(std::size_t _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ComputedParameterNodeData::OutputUpdate");
	
	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ComputedParameterNodeData::OutputUpdate");
}

void ECellEngine::Editor::Utility::LinePlotNodeData::InputConnect(std::size_t _nodeInputPinId)
{
	//X axis input pin
	if (_nodeInputPinId == (std::size_t)inputPins[1].id)
	{
		//we set the input pin of the collapsing header as the fallback
		GetLinks()->back().OverrideEndFallbackPin(inputPins[0].id, 1);
	}

	//Y axis input pin
	if (_nodeInputPinId == (std::size_t)inputPins[2].id)
	{
		//we set the input pin of the collapsing header as the fallback
		GetLinks()->back().OverrideEndFallbackPin(inputPins[0].id, 1);
	}
}

void ECellEngine::Editor::Utility::LinePlotNodeData::InputUpdate(std::size_t _nodeInputPinId, float _data)
{
	NodeInputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(inputPins, inputPins + std::size(inputPins), _nodeInputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(inputPins, it);

	//The node input pin corresponding to the X Axis data
	if (idx == 1)
	{
		newPointBuffer[0] = _data;

		//If both the x and y values of a new data point have been updated this frame.
		if (++newPointConstructionCounter == 2)
		{
			//We add the new point
			dataPoints.AddPoint(newPointBuffer[0], newPointBuffer[1]);
		}
	}

	//The node input pin corresponding to the Y Axis data
	if (idx == 2)
	{
		newPointBuffer[1] = _data;

		//If both the x and y values of a new data point have been updated this frame.
		if (++newPointConstructionCounter == 2)
		{
			//We add the new point
			dataPoints.AddPoint(newPointBuffer[0], newPointBuffer[1]);
		}
	}
}

void ECellEngine::Editor::Utility::ReactionNodeData::OutputConnect(std::size_t _nodeOutputPinId)
{
	//Reaction kinetic law value
	if (_nodeOutputPinId == (std::size_t)outputPins[8].id)
	{
		//we set the input pin of the kinetic law collapsing header as the fall back
		GetLinks()->back().OverrideStartFallbackPin(outputPins[3].id, 1);
	}
}

void ECellEngine::Editor::Utility::ReactionNodeData::OutputUpdate(std::size_t _nodeOutputPinId)
{
	
}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::InputConnect(std::size_t _nodeInputPinId)
{
	//Simple parameter value
	if (_nodeInputPinId == (std::size_t)inputPins[5].id)
	{
		//we set the input pin of the data field collapsing header as the fall back
		GetLinks()->back().OverrideEndFallbackPin(inputPins[4].id, 1);
	}
}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::InputUpdate(std::size_t _nodeInputPinId, float _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SimpleParameterNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::OutputConnect(std::size_t _nodeOutputPinId)
{
	//Simple parameter value
	if (_nodeOutputPinId == (std::size_t)outputPins[4].id)
	{
		//we set the output pin of the data field collapsing header as the fall back
		GetLinks()->back().OverrideStartFallbackPin(outputPins[3].id, 1);
	}
}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::OutputUpdate(std::size_t _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SimpleParameterNodeData::OutputUpdate");

	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SimpleParameterNodeData::OutputUpdate");
}

void ECellEngine::Editor::Utility::SimulationTimeNodeData::OutputUpdate(std::size_t _nodeOutputPinId)
{
	//There is only one output pin in the SImulationTimeNodeData so no need to look
	//for a specific one: we know our target is at index 0.
	outputPins[0].Broadcast(simulationTimer->elapsedTime);
}

void ECellEngine::Editor::Utility::SolverNodeData::OutputUpdate(std::size_t _nodeOutputPinId)
{
	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

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

void ECellEngine::Editor::Utility::SpeciesNodeData::InputConnect(std::size_t _nodeInputPinId)
{
	//Quantity value
	if (_nodeInputPinId == (std::size_t)inputPins[7].id)
	{
		//we set the input pin of the data field collapsing header as the fall back
		GetLinks()->back().OverrideEndFallbackPin(inputPins[6].id, 1);
	}
}

void ECellEngine::Editor::Utility::SpeciesNodeData::InputUpdate(std::size_t _nodeInputPinId, float _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SpeciesNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::SpeciesNodeData::OutputConnect(std::size_t _nodeOutputPinId)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SpeciesNodeData::OutputConnect");
	//Quantity value
	if (_nodeOutputPinId == (std::size_t)outputPins[6].id)
	{
		ECellEngine::Logging::Logger::GetSingleton().LogDebug("fall back of quantity to collapsing header");
		//we set the output pin of the data field collapsing header as the fall back
		GetLinks()->back().OverrideStartFallbackPin(outputPins[5].id, 1);
	}
}

void ECellEngine::Editor::Utility::SpeciesNodeData::OutputUpdate(std::size_t _nodeOutputPinId)
{
	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPinId);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	//The node output pin corresponding to the Quantity
	if (idx == 6)
	{
		it->Broadcast(data->Get());
	}
}

void ECellEngine::Editor::Utility::ValueFloatNodeData::OutputUpdate(std::size_t _nodeOutputPinId)
{
	//There is only one output pin in the ValueFloatNodeData so no need to look
	//for a specific one: we know our target is at index 0.
	outputPins[0].Broadcast(value);
}