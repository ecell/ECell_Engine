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

void ECellEngine::Editor::Utility::MNBV::AssetNodeData::InputConnect(NodeInputPinData* _nodeInputPin, char* _data)
{
	//The node input pin representing the solver attached to this asset.
	if (_nodeInputPin->id == inputPins[AssetNodeData::InputPin_Solver].id)
	{
		//In this case, the parameter _data is the name of the solver node that is attempting to attach.
		//So, we know that the action to perform is to queue the TryAttachSolverToModuleCommand
		//of the engine.
		//This command will be processed after the current context has been entirely draw in this frame.
		Widget::MNBV::QueueEngineTASToMCmd(data->GetName(), _data);
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
		_nodeInputPinData->OnConnect(&lhsValueBuffer);

		//we set the input pin of the data field collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[EquationNodeData::CollapsingHeader_EquationOperands].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::EquationNodeData::Update()
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

void ECellEngine::Editor::Utility::MNBV::LinePlotNodeData::InputConnect(NodeInputPinData* _nodeInputPin, float* _data)
{
	//X axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_XAxis].id)
	{
		ptrX = _data;

		//we set the input pin of the collapsing header as the fallback
		Widget::MNBV::GetDynamicLinks().back().OverrideEndFallbackPin(inputPins[LinePlotNodeData::InputPin_CollHdrPlot].id, 1);
	}

	//Y axis input pin
	if (_nodeInputPin->id == inputPins[LinePlotNodeData::InputPin_YAxis].id)
	{
		ptrX = _data;

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

void ECellEngine::Editor::Utility::MNBV::LinePlotNodeData::Update()
{
	if (ptrX && ptrY)
	{
		dataPoints.AddPoint(*ptrX, *ptrY);
	}
}

void ECellEngine::Editor::Utility::MNBV::ReactionNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutputPin)
{
	//Reaction kinetic law value
	if (_nodeOutputPin->id == outputPins[ReactionNodeData::OutputPin_KineticLawValue].id)
	{
		_nodeInputPinData->OnConnect(&kineticLawValueBuffer);

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

void ECellEngine::Editor::Utility::MNBV::ReactionNodeData::Update()
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
	//Simple parameter value
	if (_nodeOutputPin->id == outputPins[ParameterNodeData::OutputPin_ParameterValue].id)
	{
		//TODO: transmit to the input pin the pointer to the float value of the parameter data stored in this node.

		//we set the output pin of the data field collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[ParameterNodeData::OutputPin_CollHdrDataFields].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::SimulationTimeNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutput)
{
	//There is only one output pin in the SimulationTimeNodeData

	//TODO: transmit to the input pin the pointer to the float value of the simulation elapsed time.
}

void ECellEngine::Editor::Utility::MNBV::SolverNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutput)
{
	//There is only one output pin in the SolverNodeData

	//TODO: transmit or create the link between the solver and the asset in the simulation object.
	//		To do so, you have to queue a TryAttachSolverToModule command. 
	//		Warning, this is already implemented in the AssetNodeData::InputConnect() function.
	//		See which one is preferable.
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
		//TODO: transmit the pointer to the quantity value of the species data stored in this
		//		node to the input pin.

		//we set the output pin of the data field collapsing header as the fall back
		Widget::MNBV::GetDynamicLinks().back().OverrideStartFallbackPin(outputPins[SpeciesNodeData::OutputPin_CollHdrDataFields].id, 1);
	}
}

void ECellEngine::Editor::Utility::MNBV::SpeciesNodeData::ResetNLBSDUtilityStates() noexcept
{
	nlbsDataEqDep.ResetUtilityState();
	nlbsDataRRDep.ResetUtilityState();
	nlbsDataRPDep.ResetUtilityState();
	nlbsDataRKLDep.ResetUtilityState();
}

void ECellEngine::Editor::Utility::MNBV::ValueFloatNodeData::OutputConnect(NodeInputPinData* _nodeInputPinData, NodeOutputPinData* _nodeOutput)
{
	//There is only one output pin in the ValueFloatNodeData

	//TODO: transmit to the input pin the pointer to the float value of the data stored in this node.
}