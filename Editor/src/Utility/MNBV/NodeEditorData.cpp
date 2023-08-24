#include "Core/Events/ModifyDataStateValueEvent.hpp"
#include "Util/BinarySearch.hpp"
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

#pragma region LinkData
void ECellEngine::Editor::Utility::MNBV::LinkData::Refresh()
{
	startPin = (NodeOutputPinData*)Widget::MNBV::FindNodePinInAll((std::size_t)startIds[0]);
	endPin = (NodeInputPinData*)Widget::MNBV::FindNodePinInAll((std::size_t)endIds[0]);

	startPin->OnRefresh(endPin);
}
#pragma endregion

#pragma region NodeData

void ECellEngine::Editor::Utility::MNBV::ArithmeticOperationNodeData::InputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//Left hand side value
	if (_nodeInputPinData->id == inputPins[ArithmeticOperationNodeData::InputPin_LHS].id)
	{
		data->updateLHSSubToken = std::move(*((Core::Callback<const float, const float>*)_data) += std::bind(&Operation::UpdateLHS, data, std::placeholders::_1, std::placeholders::_2));
	}

	//Right hand side value
	if (_nodeInputPinData->id == inputPins[ArithmeticOperationNodeData::InputPin_RHS].id)
	{
		data->updateRHSSubToken = std::move(*((Core::Callback<const float, const float>*)_data) += std::bind(&Operation::UpdateRHS, data, std::placeholders::_1, std::placeholders::_2));
	}
}

void ECellEngine::Editor::Utility::MNBV::ArithmeticOperationNodeData::InputDisconnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//LHS input pin of the logic comparison
	if (_nodeInputPin->id == inputPins[LogicOperationNodeData::InputPin_LHS].id)
	{
		*((Core::Callback<const float, const float>*)_data) -= data->updateLHSSubToken;
		data->updateLHSSubToken = nullptr;
	}

	//RHS input pin of the logic comparison
	if (_nodeInputPin->id == inputPins[LogicOperationNodeData::InputPin_RHS].id)
	{
		*((Core::Callback<const float, const float>*)_data) -= data->updateRHSSubToken;
		data->updateRHSSubToken = nullptr;
	}
}

void ECellEngine::Editor::Utility::MNBV::ArithmeticOperationNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//The output pin with the result of the comparison when LHS or RHS changes
	if (_nodeOutputPinData->id == outputPins[ArithmeticOperationNodeData::OutputPin_OnOperandChange].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &data->onOperandChange);
		float res = data->Get();
		// The -1 is just to ensure that every subsequent callbacks are indeed called.
		data->onOperandChange(res-1, res, data->onOperandChange.Size() - 1);//Callback to the last subscriber
	}

	//The output pin with the result of the comparison when the result changes
	if (_nodeOutputPinData->id == outputPins[ArithmeticOperationNodeData::OutputPin_OnResultChange].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &data->onResultChange);
		float res = data->Get();
		// The -1 is just to ensure that every subsequent callbacks are indeed called.
		data->onResultChange(res-1, res, data->onResultChange.Size() - 1);//Callback to the last subscriber
	}
}

void ECellEngine::Editor::Utility::MNBV::ArithmeticOperationNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//The output pin with the result of the comparison when LHS or RHS changes
	if (_nodeOutputPinData->id == outputPins[ArithmeticOperationNodeData::OutputPin_OnOperandChange].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &data->onOperandChange);
	}

	//The output pin with the result of the comparison when the result changes
	if (_nodeOutputPinData->id == outputPins[ArithmeticOperationNodeData::OutputPin_OnResultChange].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &data->onResultChange);
	}
}

void ECellEngine::Editor::Utility::MNBV::AssetNodeData::OutputConnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData)
{
	//The node output pin representing the data of the asset
	if (_nodeOutputPinData->id == outputPins[AssetNodeData::OutputPin_ThisData].id)
	{
		_nodeInputPin->OnConnect(_nodeOutputPinData, data);
	}
}

void ECellEngine::Editor::Utility::MNBV::AssetNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
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

void ECellEngine::Editor::Utility::MNBV::EquationNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//Equation operation value
	if (_nodeOutputPin->id == outputPins[EquationNodeData::OutputPin_EquationValue].id)
	{
		//TODO: change to callback unregistration
		_nodeInputPinData->OnDisconnect(_nodeOutputPin, nullptr);
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
		linePlot->updateXSubToken = std::move( *(Core::Callback<const float, const float>*)_data += std::bind(&Plot::LinePlot::UpdateX, linePlot, std::placeholders::_1, std::placeholders::_2));

		//we set the input pin of the collapsing header as the fallback
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[LinePlotNodeData::InputPin_CollHdrPlot].id, 1);
	}

	//Y axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_YAxis].id)
	{
		std::shared_ptr<Plot::Line>& line = linePlot->AddLine((std::size_t)_nodeOutputPinData->node->id);

		line->updateLineSubToken = std::move(*(Core::Callback<const float, const float>*)_data += std::bind(&Plot::Line::UpdateLine, line, std::placeholders::_1, std::placeholders::_2));

		//Some nodes are representing data that have names.
		//We try to get the name of the node and use it as the legend of the line.
		//TODO: When the project uses reflection, this should be done differently than
		//		by using dynamic_cast.
		EquationNodeData* end = dynamic_cast<EquationNodeData*>(_nodeOutputPinData->node);
		if (end)
		{
			std::memcpy(line->lineLegend, end->data->GetOperand()->name.c_str(), 64);
		}

		ParameterNodeData* pnd = dynamic_cast<ParameterNodeData*>(_nodeOutputPinData->node);
		if (pnd)
		{
			std::memcpy(line->lineLegend, pnd->data->name.c_str(), 64);
		}

		ReactionNodeData* rnd = dynamic_cast<ReactionNodeData*>(_nodeOutputPinData->node);
		if (rnd)
		{
			std::memcpy(line->lineLegend, rnd->data->name.c_str(), 64);
			std::strcat(line->lineLegend, " (rate)");
		}

		SpeciesNodeData* snd = dynamic_cast<SpeciesNodeData*>(_nodeOutputPinData->node);
		if (snd)
		{
			std::memcpy(line->lineLegend, snd->data->name.c_str(), 64);
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
		//Unsub
		*(Core::Callback<const float, const float>*)_data -= linePlot->updateXSubToken;
		linePlot->updateXSubToken = nullptr;
	}

	//Y axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_YAxis].id)
	{
		std::vector<std::shared_ptr<Plot::Line>>::iterator it = ECellEngine::Util::BinarySearch::LowerBound(linePlot->lines.begin(), linePlot->lines.end(), (std::size_t)_nodeOutputPinData->node->id);

		if ((*it)->id == (std::size_t)_nodeOutputPinData->node->id)
		{
			//Unsub
			linePlot->lines.erase(it);
		}
	}
}

void ECellEngine::Editor::Utility::MNBV::LogicOperationNodeData::InputConnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//LHS input pin of the logic comparison
	if (_nodeInputPin->id == inputPins[LogicOperationNodeData::InputPin_LHS].id)
	{
		data->updateLHSSubToken = std::move(*((Core::Callback<const bool, const bool>*)_data) += std::bind(&Maths::LogicOperation::UpdateLHS, data, std::placeholders::_1, std::placeholders::_2));
	}

	//RHS input pin of the logic comparison
	if (_nodeInputPin->id == inputPins[LogicOperationNodeData::InputPin_RHS].id)
	{
		data->updateRHSSubToken = std::move(*((Core::Callback<const bool, const bool>*)_data) += std::bind(&Maths::LogicOperation::UpdateRHS, data, std::placeholders::_1, std::placeholders::_2));
	}
}

void ECellEngine::Editor::Utility::MNBV::LogicOperationNodeData::InputDisconnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//LHS input pin of the logic comparison
	if (_nodeInputPin->id == inputPins[LogicOperationNodeData::InputPin_LHS].id)
	{
		*((Core::Callback<const bool, const bool>*)_data) -= data->updateLHSSubToken;
		data->updateLHSSubToken = nullptr;
	}

	//RHS input pin of the logic comparison
	if (_nodeInputPin->id == inputPins[LogicOperationNodeData::InputPin_RHS].id)
	{
		*((Core::Callback<const bool, const bool>*)_data) -= data->updateRHSSubToken;
		data->updateRHSSubToken = nullptr;
	}
}

void ECellEngine::Editor::Utility::MNBV::LogicOperationNodeData::OutputConnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData)
{
	//The output pin when LHS or RHS changes
	if (_nodeOutputPinData->id == outputPins[LogicOperationNodeData::OutputPin_OnOperandChange].id)
	{
		_nodeInputPin->OnConnect(_nodeOutputPinData, &data->onOperandChange);
		bool res = (*data.get())();
		data->onOperandChange(!res, res, data->onOperandChange.Size()-1);//Callback to the last subscriber
	}

	//The output pin with the result of the comparison when the result changes
	if (_nodeOutputPinData->id == outputPins[LogicOperationNodeData::OutputPin_OnResultChange].id)
	{
		_nodeInputPin->OnConnect(_nodeOutputPinData, &data->onResultChange);
		bool res = (*data.get())();
		data->onResultChange(!res, res, data->onResultChange.Size() - 1);//Callback to the last subscriber
	}
}

void ECellEngine::Editor::Utility::MNBV::LogicOperationNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPin, NodeOutputPinData* _nodeOutputPinData)
{
	//The output pin with the result of the comparison when LHS or RHS changes
	if (_nodeOutputPinData->id == outputPins[LogicOperationNodeData::OutputPin_OnOperandChange].id)
	{
		_nodeInputPin->OnDisconnect(_nodeOutputPinData, &data->onOperandChange);
	}

	//The output pin with the result of the comparison when the result changes
	if (_nodeOutputPinData->id == outputPins[LogicOperationNodeData::OutputPin_OnResultChange].id)
	{
		_nodeInputPin->OnDisconnect(_nodeOutputPinData, &data->onResultChange);
	}
}

void ECellEngine::Editor::Utility::MNBV::ModifyDataStateValueEventNodeData::InputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//Input pin of the value to use to modify the data state
	if (_nodeInputPinData->id == inputPins[ModifyDataStateValueEventNodeData::InputPin_FloatValue].id)
	{
		data->valueCallbackToken = std::move(*((Core::Callback<float, float>*)_data) += std::bind(&Core::Events::ModifyDataStateValueEvent::UpdateValue, data, std::placeholders::_1, std::placeholders::_2));
	}

	//Input pin of the condition to use to validate modification of the data state
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
		data->valueCallbackToken = nullptr;
	}

	if (_nodeInputPinData->id == inputPins[ModifyDataStateValueEventNodeData::InputPin_Condition].id)
	{
		//remove callback
		*((Core::Callback<bool, bool>*)_data) -= data->conditionCallbackToken;
		data->conditionCallbackToken = nullptr;
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
	//_nodeInputPinData->OnDisconnect(_nodeOutputPinData, nullptr);
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

void ECellEngine::Editor::Utility::MNBV::ReactionNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//Reaction kinetic law value
	if (_nodeOutputPin->id == outputPins[ReactionNodeData::OutputPin_KineticLawValue].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPin, nullptr);
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

void ECellEngine::Editor::Utility::MNBV::ParameterNodeData::InputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin, void* _data)
{
	//Quantity value
	if (_nodeInputPinData->id == inputPins[ParameterNodeData::InputPin_ParameterValue].id)
	{
		data->updateValueSubToken = std::move(*((Core::Callback<const float, const float>*)_data) += std::bind(&Data::Parameter::UpdateValue, data, std::placeholders::_1, std::placeholders::_2));
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(inputPins[ParameterNodeData::OutputPin_CollHdrDataFields].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::ParameterNodeData::InputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin, void* _data)
{
	//Quantity value
	if (_nodeInputPinData->id == inputPins[ParameterNodeData::InputPin_ParameterValue].id)
	{
		*((Core::Callback<const float, const float>*)_data) -= data->updateValueSubToken;
		data->updateValueSubToken = nullptr;
	}
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
		_nodeInputPinData->OnConnect(_nodeOutputPin, &data->onValueChange);
		float res = data->Get();
		// The -1 is just to ensure that every subsequent callbacks are indeed called.
		data->onValueChange(res-1, res, data->onValueChange.Size() - 1);//Callback to the last subscriber

		//we set the output pin of the data field collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[ParameterNodeData::OutputPin_CollHdrDataFields].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::ParameterNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	if (_nodeOutputPin->id == outputPins[ParameterNodeData::OutputPin_ThisData].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPin, nullptr);
	}

	//Parameter value
	if (_nodeOutputPin->id == outputPins[ParameterNodeData::OutputPin_ParameterValue].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPin, &data->onValueChange);
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

void ECellEngine::Editor::Utility::MNBV::SolverNodeData::InputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//There is only one input pin for the solver node so we don't need to check the id at this time
	Widget::MNBV::QueueEngineTASToMCmd(((Data::Module*)_data)->id, data->id);
}

void ECellEngine::Editor::Utility::MNBV::SolverNodeData::InputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData, void* _data)
{
	//There is only one input pin for the solver node so we don't need to check the id at this time
	//TODO: remove the link between the solver and the asset data in the simulation
	//		by calling the appropriate command in the engine (also TODO)
	//		This can be done here or in AssetNodeData::InputDisconnect()
}

void ECellEngine::Editor::Utility::MNBV::SolverNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	if (_nodeOutputPinData->id == outputPins[SolverNodeData::OutputPin_DeltaTime].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &data->GetStepper()->timer.onDeltaTimeUpdate);
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[SolverNodeData::OutputPin_CollHdrTime].id, 1);
	}

	if (_nodeOutputPinData->id == outputPins[SolverNodeData::OutputPin_ElapsedTime].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &data->GetStepper()->timer.onElapsedTimeUpdate);
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[SolverNodeData::OutputPin_CollHdrTime].id, 1);
	}

	if (_nodeOutputPinData->id == outputPins[SolverNodeData::OutputPin_StartTime].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &data->GetStepper()->timer.onStartTimeUpdate);
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[SolverNodeData::OutputPin_CollHdrTime].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::SolverNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	if (_nodeOutputPinData->id == outputPins[SolverNodeData::OutputPin_DeltaTime].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &data->GetStepper()->timer.onDeltaTimeUpdate);
	}

	if (_nodeOutputPinData->id == outputPins[SolverNodeData::OutputPin_ElapsedTime].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &data->GetStepper()->timer.onElapsedTimeUpdate);
	}

	if (_nodeOutputPinData->id == outputPins[SolverNodeData::OutputPin_StartTime].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &data->GetStepper()->timer.onStartTimeUpdate);
	}
}

void ECellEngine::Editor::Utility::MNBV::SpeciesNodeData::InputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin, void* _data)
{
	//Quantity value
	if (_nodeInputPinData->id == inputPins[SpeciesNodeData::InputPin_Quantity].id)
	{
		data->updateQuantitySubToken = std::move(*((Core::Callback<const float, const float>*)_data) += std::bind(&Data::Species::UpdateQuantity, data, std::placeholders::_1, std::placeholders::_2));
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(inputPins[SpeciesNodeData::OutputPin_CollHdrDataFields].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::SpeciesNodeData::InputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin, void* _data)
{
	//Quantity value
	if (_nodeInputPinData->id == inputPins[SpeciesNodeData::InputPin_Quantity].id)
	{
		*((Core::Callback<const float, const float>*)_data) -= data->updateQuantitySubToken;
		data->updateQuantitySubToken = nullptr;
	}
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
		_nodeInputPinData->OnConnect(_nodeOutputPin, &data->onValueChange);
		float res = data->Get();
		// The -1 is just to ensure that every subsequent callbacks are indeed called.
		data->onValueChange(res-1, res, data->onValueChange.Size() - 1);//Callback to the last subscriber

		//we set the output pin of the data field collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[SpeciesNodeData::OutputPin_CollHdrDataFields].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::SpeciesNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//This data (the pointer to the species)
	if (_nodeOutputPin->id == outputPins[SpeciesNodeData::OutputPin_ThisData].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPin, nullptr);
	}

	//Quantity value
	if (_nodeOutputPin->id == outputPins[SpeciesNodeData::OutputPin_Quantity].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPin, &data->onValueChange);
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
	/*if (_nodeOutputPin->id == outputPins[SpeciesNodeData::OutputPin_Quantity].id)
	{
		_nodeInputPinData->OnRefresh(_nodeOutputPin, &speciesQuantityBuffer);
	}*/
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

void ECellEngine::Editor::Utility::MNBV::TimeNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	if (_nodeOutputPinData->id == outputPins[TimeNodeData::OutputPin_DeltaTime].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &timer->onDeltaTimeUpdate);
	}

	if (_nodeOutputPinData->id == outputPins[TimeNodeData::OutputPin_ElapsedTime].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &timer->onElapsedTimeUpdate);
	}

	if (_nodeOutputPinData->id == outputPins[TimeNodeData::OutputPin_StartTime].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &timer->onStartTimeUpdate);
	}
}

void ECellEngine::Editor::Utility::MNBV::TimeNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	if (_nodeOutputPinData->id == outputPins[TimeNodeData::OutputPin_DeltaTime].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &timer->onDeltaTimeUpdate);
	}

	if (_nodeOutputPinData->id == outputPins[TimeNodeData::OutputPin_ElapsedTime].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &timer->onElapsedTimeUpdate);
	}

	if (_nodeOutputPinData->id == outputPins[TimeNodeData::OutputPin_StartTime].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &timer->onStartTimeUpdate);
	}
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
	//We defer the subscription of the event to the watcher, to the event node.

	if (_nodeOutputPinData->id == outputPins[TriggerNodeData::OutputPin_OnTriggerEnter].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &data->onTriggerEnter);
	}

	if (_nodeOutputPinData->id == outputPins[TriggerNodeData::OutputPin_OnTriggerStay].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &data->onTriggerStay);
	}

	if (_nodeOutputPinData->id == outputPins[TriggerNodeData::OutputPin_OnTriggerExit].id)
	{
		_nodeInputPinData->OnConnect(_nodeOutputPinData, &data->onTriggerExit);
	}

}

void ECellEngine::Editor::Utility::MNBV::TriggerNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//We defer the unsubscription of the event to the watcher, to the event node.

	if (_nodeOutputPinData->id == outputPins[TriggerNodeData::OutputPin_OnTriggerEnter].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &data->onTriggerEnter);
	}

	if (_nodeOutputPinData->id == outputPins[TriggerNodeData::OutputPin_OnTriggerStay].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &data->onTriggerStay);
	}

	if (_nodeOutputPinData->id == outputPins[TriggerNodeData::OutputPin_OnTriggerExit].id)
	{
		_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &data->onTriggerExit);
	}
}

void ECellEngine::Editor::Utility::MNBV::ValueFloatNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the ValueFloatNodeData

	_nodeInputPinData->OnConnect(_nodeOutputPinData, &value.onValueChange);
}

void ECellEngine::Editor::Utility::MNBV::ValueFloatNodeData::OutputDisconnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
{
	//There is only one output pin in the ValueFloatNodeData

	_nodeInputPinData->OnDisconnect(_nodeOutputPinData, &value.onValueChange);
}

//void ECellEngine::Editor::Utility::MNBV::ValueFloatNodeData::OutputRefresh(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPinData)
//{
//	//There is only one output pin in the ValueFloatNodeData
//
//	_nodeInputPinData->OnRefresh(_nodeOutputPinData, &value);
//}
#pragma endregion