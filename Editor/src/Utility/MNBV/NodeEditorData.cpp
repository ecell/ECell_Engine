#include "Utility/MNBV/NodeEditorData.hpp"

#pragma region NodeListBoxStringData<DataType>

const char* ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData<std::string>::At(std::size_t _idx) const noexcept
{
	return data->at(_idx).c_str();
}

const char* ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData<std::weak_ptr<ECellEngine::Maths::Equation>>::At(std::size_t _idx) const noexcept
{
	return data->at(_idx).lock()->GetOperand()->name.c_str();
}

const char* ECellEngine::Editor::Utility::MNBV::NodeListBoxStringData<std::weak_ptr<ECellEngine::Data::Reaction>>::At(std::size_t _idx) const noexcept
{
	return data->at(_idx).lock()->name.c_str();
}

#pragma endregion

void ECellEngine::Editor::Utility::MNBV::AssetNodeData::InputConnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//The node input pin representing the solver attached to this asset.
	if (_nodeInputPin->id == inputPins[AssetNodeData::InputPin_Solver].id)
	{
		//In this case, the parameter _data is the name of the solver node that is attempting to attach.
		//So, we know that the action to perform is to queue the TryAttachSolverToModuleCommand
		//of the engine.
		//This command will be processed after the current context has been entirely draw in this frame.
		Widget::MNBV::QueueEngineTASToMCmd(data->GetName(), (char*)_data);
	}
}

void ECellEngine::Editor::Utility::MNBV::AssetNodeData::InputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//TODO: remove the link between the solver and the asset data in the simulation
	//		by calling the appropriate command in the engine (also TODO)
	//		This can be done here or in SolverNodeData::OutputDisconnect()
}

void ECellEngine::Editor::Utility::MNBV::AssetNodeData::ResetNLBSDUtilityStates() noexcept
{
	nlbsData[NodeListBoxString_Species].ResetUtilityState();
	nlbsData[NodeListBoxString_Parameters].ResetUtilityState();
	nlbsData[NodeListBoxString_Equations].ResetUtilityState();
	nlbsData[NodeListBoxString_Reactions].ResetUtilityState();
}

void ECellEngine::Editor::Utility::MNBV::EquationNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//Equation operation value
	if (_nodeOutputPin->id == outputPins[EquationNodeData::OutputPin_EquationValue].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPin, &lhsValueBuffer);

		//we set the input pin of the data field collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[EquationNodeData::CollapsingHeader_EquationOperands].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::EquationNodeData::Update() noexcept
{
	lhsValueBuffer = data->Get();
}

void ECellEngine::Editor::Utility::MNBV::EquationNodeData::ResetNLBSDUtilityStates() noexcept
{
	nlbsDataEqDep.ResetUtilityState();
	nlbsDataRKLDep.ResetUtilityState();
	nlbsData[NodeListBoxString_EquationOperands].ResetUtilityState();
	nlbsData[NodeListBoxString_ParameterOperands].ResetUtilityState();
	nlbsData[NodeListBoxString_SpeciesOperands].ResetUtilityState();
}

void ECellEngine::Editor::Utility::MNBV::LinePlotNodeData::InputConnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//X axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_XAxis].id)
	{
		ptrX = (float*)_data;

		//we set the input pin of the collapsing header as the fallback
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[LinePlotNodeData::InputPin_CollHdrPlot].id, 1);
	}

	//Y axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_YAxis].id)
	{
		ptrY = (float*)_data;

		//we set the input pin of the collapsing header as the fallback
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[LinePlotNodeData::InputPin_CollHdrPlot].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::LinePlotNodeData::InputDisconnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData)
{
	//X axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_XAxis].id)
	{
		ptrX = nullptr;

		//we set the input pin of the collapsing header as the fallback
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[LinePlotNodeData::InputPin_CollHdrPlot].id, 1);
	}

	//Y axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_YAxis].id)
	{
		ptrY = nullptr;

		//we set the input pin of the collapsing header as the fallback
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[LinePlotNodeData::InputPin_CollHdrPlot].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::LinePlotNodeData::Update() noexcept
{
	if (ptrX && ptrY)
	{
		dataPoints.AddPoint(*ptrX, *ptrY);
	}
}

void ECellEngine::Editor::Utility::MNBV::ModifyDataStateValueEventNodeData::InputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	if (_nodeInputPinData->id == inputPins[ModifyDataStateValueEventNodeData::InputPin_NewFloatValue].id)
	{
		data->newValue = (float*)_data;
	}

	if (_nodeInputPinData->id == inputPins[ModifyDataStateValueEventNodeData::InputPin_Watchers].id)
	{
		WatcherNodeData* watcherNodeData = dynamic_cast<WatcherNodeData*>(_nodeOutputPinData->node);
		watcherNodeData->data->AddEvent(data);
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[ModifyDataStateValueEventNodeData::InputPin_CollHdrExecution].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::ModifyDataStateValueEventNodeData::InputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	if (_nodeInputPinData->id == inputPins[ModifyDataStateValueEventNodeData::InputPin_NewFloatValue].id)
	{
		newValue = *data->newValue;
		data->newValue = &newValue;
	}
}

void ECellEngine::Editor::Utility::MNBV::ModifyDataStateValueEventNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin (Modify Pin), so we don't need to check the id of the output pin

	SpeciesNodeData* speciesNodeData = dynamic_cast<SpeciesNodeData*>(_nodeInputPinData->node);
	if (speciesNodeData != nullptr)
	{
		data->dataStateValueId = speciesNodeData->data.get()->name;
		data->valueType = ECellEngine::Core::Events::ModifyDataStateValueEvent::DataStateValueType::Species;
	}

	ParameterNodeData* parameterNodeData = dynamic_cast<ParameterNodeData*>(_nodeInputPinData->node);
	if (parameterNodeData != nullptr)
	{
		data->dataStateValueId = parameterNodeData->data.get()->name;
		data->valueType = ECellEngine::Core::Events::ModifyDataStateValueEvent::DataStateValueType::Parameter;
	}

	Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[ModifyDataStateValueEventNodeData::OutputPin_CollHdrExecution].id, 1);
}

void ECellEngine::Editor::Utility::MNBV::ModifyDataStateValueEventNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin (Modify Pin), so we don't need to check the id of the output pin

	//Reset the data state value id
	data->dataStateValueId = "";
}

void ECellEngine::Editor::Utility::MNBV::ReactionNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//Reaction kinetic law value
	if (_nodeOutputPin->id == outputPins[ReactionNodeData::OutputPin_KineticLawValue].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPin, &kineticLawValueBuffer);

		//we set the input pin of the kinetic law collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[OutputPin_CollHdrKineticLaw].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::ReactionNodeData::ResetNLBSDUtilityStates() noexcept
{
	nlbsData[NodeListBoxString_Products].ResetUtilityState();
	nlbsData[NodeListBoxString_Reactants].ResetUtilityState();
	nlbsData[NodeListBoxString_EquationOperands].ResetUtilityState();
	nlbsData[NodeListBoxString_ParameterOperands].ResetUtilityState();
	nlbsData[NodeListBoxString_SpeciesOperands].ResetUtilityState();
}

void ECellEngine::Editor::Utility::MNBV::ReactionNodeData::Update() noexcept
{
	kineticLawValueBuffer = data->GetKineticLawValue();
}

void ECellEngine::Editor::Utility::MNBV::ParameterNodeData::ResetNLBSDUtilityStates() noexcept
{
	nlbsDataEqDep.ResetUtilityState();
	nlbsDataRKLDep.ResetUtilityState();
}

void ECellEngine::Editor::Utility::MNBV::ParameterNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//Parameter value
	if (_nodeOutputPin->id == outputPins[ParameterNodeData::OutputPin_ParameterValue].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPin, &parameterValueBuffer);

		//we set the output pin of the data field collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[ParameterNodeData::OutputPin_CollHdrDataFields].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::ParameterNodeData::Update() noexcept
{
	parameterValueBuffer = data->Get();
}

void ECellEngine::Editor::Utility::MNBV::SimulationTimeNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the SimulationTimeNodeData

	_nodeInputPinData->OnConnect(_nodeOutputPinData, &(simulationTimer->elapsedTime));
}

void ECellEngine::Editor::Utility::MNBV::SolverNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the SolverNodeData

	_nodeInputPinData->OnConnect(_nodeOutputPinData, data->GetName());
}

void ECellEngine::Editor::Utility::MNBV::SolverNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//TODO: remove the link between the solver and the asset adata in the simulation
	//		by calling the appropriate command in the engine (also TODO)
	//		This can be done here or in AssetNodeData::InputDisconnect()
}

void ECellEngine::Editor::Utility::MNBV::SpeciesNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//Quantity value
	if (_nodeOutputPin->id == outputPins[SpeciesNodeData::OutputPin_Quantity].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPin, &speciesQuantityBuffer);

		//we set the output pin of the data field collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[SpeciesNodeData::OutputPin_CollHdrDataFields].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::SpeciesNodeData::Update() noexcept
{
	speciesQuantityBuffer = data->Get();
}

void ECellEngine::Editor::Utility::MNBV::SpeciesNodeData::ResetNLBSDUtilityStates() noexcept
{
	nlbsDataEqDep.ResetUtilityState();
	nlbsDataRRDep.ResetUtilityState();
	nlbsDataRPDep.ResetUtilityState();
	nlbsDataRKLDep.ResetUtilityState();
}

void ECellEngine::Editor::Utility::MNBV::ValueFloatNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the ValueFloatNodeData

	_nodeInputPinData->OnConnect(_nodeOutputPinData, &value);
}

void ECellEngine::Editor::Utility::MNBV::WatcherNodeData::InputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//The LHS input pin
	if (_nodeInputPinData->id == inputPins[WatcherNodeData::InputPin_LHS].id)
	{
		data->SetLHS((float*)_data);
	}
	
	//The RHS input pin
	if (_nodeInputPinData->id == inputPins[WatcherNodeData::InputPin_RHS].id)
	{
		data->SetRHS((float*)_data);
	}
}

void ECellEngine::Editor::Utility::MNBV::WatcherNodeData::InputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//The LHS input pin
	if (_nodeInputPinData->id == inputPins[WatcherNodeData::InputPin_LHS].id)
	{
		//We start by setting the local lhs value to the previous value of the lhs in the watcher.
		lhs = *data->GetLHS();

		//We then set the lhs of the watcher to the local lhs value.
		data->SetLHS(&lhs);
	}

	//The RHS input pin
	if (_nodeInputPinData->id == inputPins[WatcherNodeData::InputPin_RHS].id)
	{
		//We start by setting the local rhs value to the previous value of the rhs in the watcher.
		rhs = *data->GetRHS();

		//We then set the rhs of the watcher to the local rhs value.
		data->SetRHS(&rhs);
	}
}

void ECellEngine::Editor::Utility::MNBV::WatcherNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the WatcherNodeData and it is the output pin of the events to trigger.

	//We defer the subscription of the event to the watcher, to the event node.
	_nodeInputPinData->OnConnect(_nodeOutputPinData, nullptr);
}

void ECellEngine::Editor::Utility::MNBV::WatcherNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the WatcherNodeData and it is the output pin of the events to trigger.

	//TODO: Handle disconnection to an Event node
	//		In particular, remove the event node from the subscribers of the watcher
}