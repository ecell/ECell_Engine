#include "NodeEditorData.hpp"


void ECellEngine::Editor::Utility::AssetNodeData::InputUpdate(const NodeInputPinData& _nodeInputPin, char* _data)
{
	//The node input pin representing the solver attached to this asset.
	if (_nodeInputPin == inputPins[AssetNodeData::InputPin_Solver])
	{
		//In this case, the parameter _data is the name of the solver node that is attempting to attach.
		//So, we know that we have the action to perform is to queue the TryAttachSolverToModuleCommand
		//of the engine.
		//This command will be process after the current context has been entirely draw in this frame.
		QueueEngineTASToMCmd(data->GetName(), _data);
	}
}


void ECellEngine::Editor::Utility::ComputedParameterNodeData::OutputConnect(const NodeOutputPinData& _nodeOutputPin)
{
	//Computed parameter operation value
	if (_nodeOutputPin == outputPins[ComputedParameterNodeData::OutputPin_ComputedParameterValue])
	{
		//we set the input pin of the data field collapsing header as the fall back
		GetLinks()->back().OverrideStartFallbackPin(outputPins[ComputedParameterNodeData::CollapsingHeader_EquationOperands].id, 1);
	}
}

void ECellEngine::Editor::Utility::ComputedParameterNodeData::OutputUpdate(const NodeOutputPinData& _nodeOutputPin)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("ComputedParameterNodeData::OutputUpdate");
}

void ECellEngine::Editor::Utility::LinePlotNodeData::InputConnect(const NodeInputPinData& _nodeInputPin)
{
	//X axis input pin
	if (_nodeInputPin == inputPins[1])
	{
		//we set the input pin of the collapsing header as the fallback
		GetLinks()->back().OverrideEndFallbackPin(inputPins[0].id, 1);
	}

	//Y axis input pin
	if (_nodeInputPin == inputPins[2])
	{
		//we set the input pin of the collapsing header as the fallback
		GetLinks()->back().OverrideEndFallbackPin(inputPins[0].id, 1);
	}
}

void ECellEngine::Editor::Utility::LinePlotNodeData::InputUpdate(const NodeInputPinData& _nodeInputPin, float _data)
{
	//The node input pin corresponding to the X Axis data
	if (_nodeInputPin == inputPins[1])
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
	if (_nodeInputPin == inputPins[2])
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

void ECellEngine::Editor::Utility::ReactionNodeData::OutputConnect(const NodeOutputPinData& _nodeOutputPin)
{
	//Reaction kinetic law value
	if (_nodeOutputPin == outputPins[8])
	{
		//we set the input pin of the kinetic law collapsing header as the fall back
		GetLinks()->back().OverrideStartFallbackPin(outputPins[3].id, 1);
	}
}

void ECellEngine::Editor::Utility::ReactionNodeData::OutputUpdate(const NodeOutputPinData& _nodeOutputPin)
{

}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::InputConnect(const NodeInputPinData& _nodeInputPin)
{
	//Simple parameter value
	if (_nodeInputPin == inputPins[5])
	{
		//we set the input pin of the data field collapsing header as the fall back
		GetLinks()->back().OverrideEndFallbackPin(inputPins[4].id, 1);
	}
}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::InputUpdate(const NodeInputPinData& _nodeInputPin, float _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SimpleParameterNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::OutputConnect(const NodeOutputPinData& _nodeOutputPin)
{
	//Simple parameter value
	if (_nodeOutputPin == outputPins[4])
	{
		//we set the output pin of the data field collapsing header as the fall back
		GetLinks()->back().OverrideStartFallbackPin(outputPins[3].id, 1);
	}
}

void ECellEngine::Editor::Utility::SimpleParameterNodeData::OutputUpdate(const NodeOutputPinData& _nodeOutputPin)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SimpleParameterNodeData::OutputUpdate");
}

void ECellEngine::Editor::Utility::SimulationTimeNodeData::OutputUpdate(const NodeOutputPinData& _nodeOutputPin)
{
	//There is only one output pin in the SImulationTimeNodeData so no need to look
	//for a specific one: we know our target is at index 0.
	outputPins[0].Broadcast(simulationTimer->elapsedTime);
}

void ECellEngine::Editor::Utility::SolverNodeData::OutputUpdate(const NodeOutputPinData& _nodeOutputPin)
{
	//The node output pin representing the solver.
	//It is used to attach a solver to an asset.
	if (_nodeOutputPin == outputPins[0])
	{
		//The Solver node send his name to the asset node.
		//It is the Asset node, in its InputUpdate() that will Queue the command to 
		//try to attach the Solver to the Asset.
		outputPins[0].Broadcast(data->GetName());
	}
}

void ECellEngine::Editor::Utility::SpeciesNodeData::InputConnect(const NodeInputPinData& _nodeInputPin)
{
	//Quantity value
	if (_nodeInputPin == inputPins[7])
	{
		//we set the input pin of the data field collapsing header as the fall back
		GetLinks()->back().OverrideEndFallbackPin(inputPins[6].id, 1);
	}
}

void ECellEngine::Editor::Utility::SpeciesNodeData::InputUpdate(const NodeInputPinData& _nodeInputPin, float _data)
{
	ECellEngine::Logging::Logger::GetSingleton().LogDebug("SpeciesNodeData::InputUpdate; data=" + std::to_string(_data));
}

void ECellEngine::Editor::Utility::SpeciesNodeData::OutputConnect(const NodeOutputPinData& _nodeOutputPin)
{
	//Quantity value
	if (_nodeOutputPin == outputPins[6])
	{
		//we set the output pin of the data field collapsing header as the fall back
		GetLinks()->back().OverrideStartFallbackPin(outputPins[5].id, 1);
	}
}

void ECellEngine::Editor::Utility::SpeciesNodeData::OutputUpdate(const NodeOutputPinData& _nodeOutputPin)
{
	NodeOutputPinData* it = ECellEngine::Data::BinaryOperation::LowerBound(outputPins, outputPins + std::size(outputPins), _nodeOutputPin);
	typename std::iterator_traits<NodeOutputPinData*>::difference_type idx;
	idx = std::distance(outputPins, it);

	//The node output pin corresponding to the Quantity
	if (idx == 6)
	{
		it->Broadcast(data->Get());
	}
}

void ECellEngine::Editor::Utility::ValueFloatNodeData::OutputUpdate(const NodeOutputPinData& _nodeOutputPin)
{
	//There is only one output pin in the ValueFloatNodeData so no need to look
	//for a specific one: we know our target is at index 0.
	outputPins[0].Broadcast(value);
}