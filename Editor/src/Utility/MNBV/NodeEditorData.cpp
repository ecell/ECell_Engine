#include "Utility/MNBV/NodeEditorData.hpp"
#include "Core/Events/ModifyDataStateValueEvent.hpp"

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

#pragma region LinkData
void ECellEngine::Editor::Utility::MNBV::LinkData::Refresh()
{
	startPin = (NodeOutputPinData*)Widget::MNBV::FindNodePinInAll((std::size_t)startIds[0]);
	endPin = (NodeInputPinData*)Widget::MNBV::FindNodePinInAll((std::size_t)endIds[0]);

	startPin->OnRefresh(endPin);
}
#pragma enregion

#pragma region NodeData
void ECellEngine::Editor::Utility::MNBV::AssetNodeData::InputConnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//The node input pin representing the solver attached to this asset.
	if (_nodeInputPin->id == inputPins[AssetNodeData::InputPin_Solver].id)
	{
		//In this case, the parameter _data is the name of the solver node that is attempting to attach.
		//So, we know that the action to perform is to queue the TryAttachSolverToModuleCommand
		//of the engine.
		//This command will be processed after the current context has been entirely draw in this frame.
		Widget::MNBV::QueueEngineTASToMCmd(data->GetName(), ((std::string*)_data)->c_str());
	}
}

void ECellEngine::Editor::Utility::MNBV::AssetNodeData::InputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin, void* _data)
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

void ECellEngine::Editor::Utility::MNBV::EquationNodeData::OutputRefresh(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//Equation operation value
	if (_nodeOutputPin->id == outputPins[EquationNodeData::OutputPin_EquationValue].id)
	{
		_nodeInputPinData->OnRefresh(_nodeOutputPin, &lhsValueBuffer);
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
		linePlot.ptrX = (float*)_data;

		linePlot.Update();

		//we set the input pin of the collapsing header as the fallback
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[LinePlotNodeData::InputPin_CollHdrPlot].id, 1);
	}

	//Y axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_YAxis].id)
	{
		Plot::Line& line = linePlot.AddLine((std::size_t)_nodeOutputPinData->node->id);

		line.ptrY = (float*)_data;

		if (linePlot.ptrX)
		{
			line.Update(*linePlot.ptrX);
		}

		//Some nodes are representing data that have names.
		//We try to get the name of the node and use it as the legend of the line.
		//TODO: When the project uses reflection, this should be done differently than
		//		by using dynamic_cast.
		EquationNodeData* end = dynamic_cast<EquationNodeData*>(_nodeOutputPinData->node);
		if (end)
		{
			std::memcpy(line.lineLegend, end->data->GetOperand()->name.c_str(), 64);
		}

		ParameterNodeData* pnd = dynamic_cast<ParameterNodeData*>(_nodeOutputPinData->node);
		if (pnd)
		{
			std::memcpy(line.lineLegend, pnd->data->name.c_str(), 64);
		}

		ReactionNodeData* rnd = dynamic_cast<ReactionNodeData*>(_nodeOutputPinData->node);
		if (rnd)
		{
			std::memcpy(line.lineLegend, rnd->data->name.c_str(), 64);
			std::strcat(line.lineLegend, " (rate)");
		}

		SpeciesNodeData* snd = dynamic_cast<SpeciesNodeData*>(_nodeOutputPinData->node);
		if (snd)
		{
			std::memcpy(line.lineLegend, snd->data->name.c_str(), 64);
		}

		//we set the input pin of the collapsing header as the fallback
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[LinePlotNodeData::InputPin_CollHdrPlot].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::LinePlotNodeData::InputDisconnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//X axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_XAxis].id)
	{
		linePlot.ptrX = nullptr;

		//we set the input pin of the collapsing header as the fallback
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[LinePlotNodeData::InputPin_CollHdrPlot].id, 1);
	}

	//Y axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_YAxis].id)
	{
		std::vector<Plot::Line>::iterator it = ECellEngine::Data::BinaryOperation::LowerBound(linePlot.lines.begin(), linePlot.lines.end(), (std::size_t)_nodeOutputPinData->node->id);

		if (it->id == (std::size_t)_nodeOutputPinData->node->id)
		{
			linePlot.lines.erase(it);
		}

		//we set the input pin of the collapsing header as the fallback
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[LinePlotNodeData::InputPin_CollHdrPlot].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::LinePlotNodeData::InputRefresh(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//X axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_XAxis].id)
	{
		linePlot.ptrX = (float*)_data;
	}

	//Y axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_YAxis].id)
	{
		std::vector<Plot::Line>::iterator it = ECellEngine::Data::BinaryOperation::LowerBound(linePlot.lines.begin(), linePlot.lines.end(), (std::size_t)_nodeOutputPinData->node->id);

		if (it->id == (std::size_t)_nodeOutputPinData->node->id)
		{
			it->ptrY = (float*)_data;
		}
	}
}

void ECellEngine::Editor::Utility::MNBV::ModifyDataStateValueEventNodeData::InputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	if (_nodeInputPinData->id == inputPins[ModifyDataStateValueEventNodeData::InputPin_FloatValue].id)
	{
		data->valueCallbackToken = std::move(*((Core::Callback<float, float>*)_data) += std::bind(&Core::Events::ModifyDataStateValueEvent::UpdateValue, data, std::placeholders::_1, std::placeholders::_2));
	}

	if (_nodeInputPinData->id == inputPins[ModifyDataStateValueEventNodeData::InputPin_Condition].id)
	{
		data->conditionCallbackToken = std::move(*((Core::Callback<bool, bool>*)_data) += std::bind(&Core::Events::ModifyDataStateValueEvent::UpdateCondition, data, std::placeholders::_1, std::placeholders::_2));
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[ModifyDataStateValueEventNodeData::InputPin_CollHdrExecution].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::ModifyDataStateValueEventNodeData::InputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	if (_nodeInputPinData->id == inputPins[ModifyDataStateValueEventNodeData::InputPin_FloatValue].id)
	{
		value = data->GetValue();
		//remove callback
		*((Core::Callback<float, float>*)_data) -= data->valueCallbackToken;
	}

	if (_nodeInputPinData->id == inputPins[ModifyDataStateValueEventNodeData::InputPin_Condition].id)
	{
		//remove callback
		*((Core::Callback<bool, bool>*)_data) -= data->conditionCallbackToken;
	}
}

//void ECellEngine::Editor::Utility::MNBV::ModifyDataStateValueEventNodeData::InputRefresh(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
//{
//	if (_nodeInputPinData->id == inputPins[ModifyDataStateValueEventNodeData::InputPin_FloatValue].id)
//	{
//		data->newValue = (float*)_data;
//	}
//}

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

void ECellEngine::Editor::Utility::MNBV::ReactionNodeData::OutputRefresh(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//Reaction kinetic law value
	if (_nodeOutputPin->id == outputPins[ReactionNodeData::OutputPin_KineticLawValue].id)
	{
		_nodeInputPinData->OnRefresh(_nodeOutputPin, &kineticLawValueBuffer);
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
	if (_nodeOutputPin->id == outputPins[ParameterNodeData::OutputPin_ThisData].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPin, data.get());
		
		//we set the output pin of the model links collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[ParameterNodeData::OutputPin_CollHdrModelLinks].id, 1);
	}

	//Parameter value
	if (_nodeOutputPin->id == outputPins[ParameterNodeData::OutputPin_ParameterValue].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPin, &parameterValueBuffer);

		//we set the output pin of the data field collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[ParameterNodeData::OutputPin_CollHdrDataFields].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::ParameterNodeData::OutputRefresh(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	if (_nodeOutputPin->id == outputPins[ParameterNodeData::OutputPin_ThisData].id)
	{
		_nodeInputPinData->OnRefresh(_nodeOutputPin, data.get());
	}

	//Parameter value
	if (_nodeOutputPin->id == outputPins[ParameterNodeData::OutputPin_ParameterValue].id)
	{
		_nodeInputPinData->OnRefresh(_nodeOutputPin, &parameterValueBuffer);
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

void ECellEngine::Editor::Utility::MNBV::SimulationTimeNodeData::OutputRefresh(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the SimulationTimeNodeData

	_nodeInputPinData->OnRefresh(_nodeOutputPinData, &(simulationTimer->elapsedTime));
}

void ECellEngine::Editor::Utility::MNBV::SolverNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the SolverNodeData

	_nodeInputPinData->OnConnect(_nodeOutputPinData, &data->GetName());
}

void ECellEngine::Editor::Utility::MNBV::SolverNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//TODO: remove the link between the solver and the asset adata in the simulation
	//		by calling the appropriate command in the engine (also TODO)
	//		This can be done here or in AssetNodeData::InputDisconnect()
}

void ECellEngine::Editor::Utility::MNBV::SolverNodeData::OutputRefresh(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the SolverNodeData

	_nodeInputPinData->OnRefresh(_nodeOutputPinData, &data->GetName());
}

void ECellEngine::Editor::Utility::MNBV::SpeciesNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//This data (the pointer to the species)
	if (_nodeOutputPin->id == outputPins[SpeciesNodeData::OutputPin_ThisData].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPin, data.get());

		//we set the output pin of the model links collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[ParameterNodeData::OutputPin_CollHdrModelLinks].id, 1);
	}

	//Quantity value
	if (_nodeOutputPin->id == outputPins[SpeciesNodeData::OutputPin_Quantity].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPin, &speciesQuantityBuffer);

		//we set the output pin of the data field collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[SpeciesNodeData::OutputPin_CollHdrDataFields].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::SpeciesNodeData::OutputRefresh(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//This data (the pointer to the species)
	if (_nodeOutputPin->id == outputPins[SpeciesNodeData::OutputPin_ThisData].id)
	{
		_nodeInputPinData->OnRefresh(_nodeOutputPin, data.get());
	}

	//Quantity value
	if (_nodeOutputPin->id == outputPins[SpeciesNodeData::OutputPin_Quantity].id)
	{
		_nodeInputPinData->OnRefresh(_nodeOutputPin, &speciesQuantityBuffer);
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

void ECellEngine::Editor::Utility::MNBV::ValueFloatNodeData::OutputRefresh(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the ValueFloatNodeData

	_nodeInputPinData->OnRefresh(_nodeOutputPinData, &value);
}

void ECellEngine::Editor::Utility::MNBV::TriggerNodeData::InputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//The LHS input pin
	if (_nodeInputPinData->id == inputPins[TriggerNodeData::InputPin_Target].id)
	{
		data->SetTarget((Operand*)_data);
	}
	
	//The RHS input pin
	if (_nodeInputPinData->id == inputPins[TriggerNodeData::InputPin_Threshold].id)
	{
		data->SetThreshold((Operand*)_data);
	}
}

void ECellEngine::Editor::Utility::MNBV::TriggerNodeData::InputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//The LHS input pin
	if (_nodeInputPinData->id == inputPins[TriggerNodeData::InputPin_Target].id)
	{
		//We start by setting the local target value to the previous value of the target in the watcher.
		target.Set(data->GetTarget()->Get());

		//We then set the target of the watcher to the local target value.
		data->SetTarget(&target);
	}

	//The RHS input pin
	if (_nodeInputPinData->id == inputPins[TriggerNodeData::InputPin_Threshold].id)
	{
		//We start by setting the local threshold value to the previous value of the rhs in the watcher.
		threshold.Set(data->GetThreshold()->Get());

		//We then set the threshold of the watcher to the local threshold value.
		data->SetThreshold(&threshold);
	}
}

void ECellEngine::Editor::Utility::MNBV::TriggerNodeData::InputRefresh(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//The Target input pin
	if (_nodeInputPinData->id == inputPins[TriggerNodeData::InputPin_Target].id)
	{
		data->SetTarget((Operand*)_data);
	}

	//The Threshold input pin
	if (_nodeInputPinData->id == inputPins[TriggerNodeData::InputPin_Threshold].id)
	{
		data->SetThreshold((Operand*)_data);
	}
}

void ECellEngine::Editor::Utility::MNBV::TriggerNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the TriggerNodeData and it is the output pin of the events to trigger.

	//We defer the subscription of the event to the watcher, to the event node.
	_nodeInputPinData->OnConnect(_nodeOutputPinData, nullptr);
}

void ECellEngine::Editor::Utility::MNBV::TriggerNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the TriggerNodeData and it is the output pin of the events to trigger.

	//We defer the unsubscription of the event to the watcher, to the event node.
	_nodeInputPinData->OnDisconnect(_nodeOutputPinData);
}

#pragma endregion