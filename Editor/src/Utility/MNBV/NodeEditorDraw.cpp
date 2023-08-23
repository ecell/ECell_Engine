#include "Utility/MNBV/NodeEditorDraw.hpp"

#pragma region Nodes

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeDestruction()
{
	if (ax::NodeEditor::BeginDelete())
	{
		// There may be many links marked for deletion, let's loop over them.
		ax::NodeEditor::NodeId deletedNodeId;
		while (ax::NodeEditor::QueryDeletedNode(&deletedNodeId))
		{
			// If you agree that link can be deleted, accept deletion.
			if (ax::NodeEditor::AcceptDeletedItem())
			{
				ECellEngine::Editor::Widget::MNBV::EraseNode((std::size_t)deletedNodeId);
			}

			// You may reject link deletion by calling:
			// ax::NodeEditor::RejectDeletedItem();
		}
	}
	ax::NodeEditor::EndDelete(); // Wrap up deletion action
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::ArithmeticOperationNode(ArithmeticOperationNodeData& _arithmeticOperationNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_LogicOperation));
	ax::NodeEditor::BeginNode(_arithmeticOperationNodeInfo.id);
	
	ImGui::PushID((std::size_t)_arithmeticOperationNodeInfo.id);

	const float headerWidth = NodeHeader("Arithmetic Operation", "", Style::EditorStyle::GetNodeColors(NodeType_Data), 300.f);
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();
	const float onOperandChangeWidth = ImGui::CalcTextSize("onOperandChange").x;
	const float onResultChangeWidth = ImGui::CalcTextSize("onResultChange").x;

	if (NodeComboBox("Function", _arithmeticOperationNodeInfo.operatorTypes, 6, (int&)_arithmeticOperationNodeInfo.data->GetFunctionType(),
		itemsWidth, startX, headerWidth))
	{
		_arithmeticOperationNodeInfo.data->UpdateFunction();
		//not bullet proof but should do the trick
		_arithmeticOperationNodeInfo.data->UpdateLHS(_arithmeticOperationNodeInfo.lhs.Get() + 1.f, _arithmeticOperationNodeInfo.lhs.Get());
	}

	ImGuiSliderFlags dragFlags = ImGuiSliderFlags_None;
	if (_arithmeticOperationNodeInfo.inputPins[ArithmeticOperationNodeData::InputPin_LHS].nbConnectedLinks > 0)
	{
		dragFlags |= ImGuiSliderFlags_ReadOnly;
	}
	float bufferTarget = _arithmeticOperationNodeInfo.lhs.Get();
	if (NodeDragFloat_In("LHS", _arithmeticOperationNodeInfo.inputPins[ArithmeticOperationNodeData::InputPin_LHS], &bufferTarget,
		0.5f * itemsWidth, startX,_arithmeticOperationNodeInfo.inputPins[ArithmeticOperationNodeData::InputPin_LHS], dragFlags))
	{
		_arithmeticOperationNodeInfo.data->UpdateLHS(_arithmeticOperationNodeInfo.lhs.Get(), bufferTarget);
	}

	ImGui::SameLine();

	NodeText_Out("onOperandChange", onOperandChangeWidth, startX, headerWidth,
		_arithmeticOperationNodeInfo.outputPins[LogicOperationNodeData::OutputPin_OnOperandChange]);

	dragFlags = ImGuiSliderFlags_None;
	if (_arithmeticOperationNodeInfo.inputPins[ArithmeticOperationNodeData::InputPin_RHS].nbConnectedLinks > 0)
	{
		dragFlags |= ImGuiSliderFlags_ReadOnly;
	}
	bufferTarget = _arithmeticOperationNodeInfo.rhs.Get();
	if (NodeDragFloat_In("RHS", _arithmeticOperationNodeInfo.inputPins[ArithmeticOperationNodeData::InputPin_RHS], &bufferTarget,
		0.5f * itemsWidth, startX, _arithmeticOperationNodeInfo.inputPins[ArithmeticOperationNodeData::InputPin_RHS], dragFlags))
	{
		_arithmeticOperationNodeInfo.data->UpdateRHS(_arithmeticOperationNodeInfo.rhs.Get(), bufferTarget);
	}

	ImGui::SameLine();

	NodeText_Out("onResultChange", onResultChangeWidth, startX, headerWidth,
		_arithmeticOperationNodeInfo.outputPins[LogicOperationNodeData::OutputPin_OnResultChange]);

	ImGui::PopID();
	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::AssetNode(const char* _name, AssetNodeData& _assetNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Asset));
	ax::NodeEditor::BeginNode(_assetNodeInfo.id);

	const float headerWidth = NodeHeader("Asset:", _name, Style::EditorStyle::GetNodeColors(NodeType_Asset));
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();
	const float thisLabelWidth = ImGui::CalcTextSize("This").x;

	// ----- Pin and Text to connect the solver to the asset -----
	NodeText_Out("This", thisLabelWidth, startX, headerWidth, _assetNodeInfo.outputPins[AssetNodeData::OutputPin_ThisData]);

	// ----- String List Box and Pin to access the species of the asset -----
	if (NodeCollapsingHeader_Out("Species", _assetNodeInfo.collapsingHeadersIds[AssetNodeData::CollapsingHeader_Species],
		_assetNodeInfo.utilityState, AssetNodeData::State_CollHdrSpecies,
		startX, headerWidth,
		_assetNodeInfo.outputPins[AssetNodeData::OutputPin_CollHdrSpecies],
		ImVec2(itemsWidth, 0), false))
	{
		NodeStringListBox(_assetNodeInfo.nlbsData[AssetNodeData::NodeListBoxString_Species], startX, headerWidth, itemsWidth);
	}

	// ----- String List Box and Pin to access the parameters of the asset -----
	if (NodeCollapsingHeader_Out("Parameters", _assetNodeInfo.collapsingHeadersIds[AssetNodeData::CollapsingHeader_Parameters],
		_assetNodeInfo.utilityState, AssetNodeData::State_CollHdrParameters,
		startX, headerWidth,
		_assetNodeInfo.outputPins[AssetNodeData::OutputPin_CollHdrParameters],
		ImVec2(itemsWidth, 0), false))
	{
		NodeStringListBox(_assetNodeInfo.nlbsData[AssetNodeData::NodeListBoxString_Parameters], startX, headerWidth, itemsWidth);
	}

	// ----- String List Box and Pin to access the equations of the asset -----
	if (NodeCollapsingHeader_Out("Equations", _assetNodeInfo.collapsingHeadersIds[AssetNodeData::CollapsingHeader_Equations],
		_assetNodeInfo.utilityState, AssetNodeData::State_CollHdrEquations,
		startX, headerWidth,
		_assetNodeInfo.outputPins[AssetNodeData::OutputPin_CollHdrEquations],
		ImVec2(itemsWidth, 0), false))
	{
		NodeStringListBox(_assetNodeInfo.nlbsData[AssetNodeData::NodeListBoxString_Equations], startX, headerWidth, itemsWidth);
	}

	// ----- String List Box and Pin to access the reactions of the asset -----
	if (NodeCollapsingHeader_Out("Reactions", _assetNodeInfo.collapsingHeadersIds[AssetNodeData::CollapsingHeader_Reactions],
		_assetNodeInfo.utilityState, AssetNodeData::State_CollHdrReactions,
		startX, headerWidth,
		_assetNodeInfo.outputPins[AssetNodeData::OutputPin_CollHdrReactions],
		ImVec2(itemsWidth, 0), false))
	{
		NodeStringListBox(_assetNodeInfo.nlbsData[AssetNodeData::NodeListBoxString_Reactions], startX, headerWidth, itemsWidth);
	}

	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();

}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::EquationNode(const char* _name, EquationNodeData& _equationNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Equation));
	ax::NodeEditor::BeginNode(_equationNodeInfo.id);

	_equationNodeInfo.Update();

	const float headerWidth = NodeHeader("Equation:", _name, Style::EditorStyle::GetNodeColors(NodeType_Equation));
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();

	if (NodeCollapsingHeader_InOut("Model Links", _equationNodeInfo.collapsingHeadersIds[EquationNodeData::CollapsingHeader_ModelLinks],
		_equationNodeInfo.utilityState, EquationNodeData::State_CollHdrModelLinks,
		startX, headerWidth,
		_equationNodeInfo.inputPins[EquationNodeData::InputPin_CollHdrModelLinks], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_CollHdrModelLinks],
		ImVec2(itemsWidth, 0.f)))
	{

		NodeText_In("Asset", startX, _equationNodeInfo.inputPins[EquationNodeData::InputPin_Asset]);

		if (_equationNodeInfo.equationDep.size())
		{
			if (NodeCollapsingHeader_InOut("In Equations", _equationNodeInfo.collapsingHeadersIds[EquationNodeData::CollapsingHeader_Equations],
				_equationNodeInfo.utilityState, EquationNodeData::State_CollHdrEquations,
				startX, headerWidth,
				_equationNodeInfo.inputPins[EquationNodeData::InputPin_CollHdrEquations], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_CollHdrEquations],
				ImVec2(itemsWidth, 0.f), false))
			{
				NodeStringListBox(_equationNodeInfo.nlbsDataEqDep, startX, headerWidth, itemsWidth);
			}
		}

		if (_equationNodeInfo.reactionKLDep.size())
		{
			if (NodeCollapsingHeader_InOut("In Kinetic Laws", _equationNodeInfo.collapsingHeadersIds[EquationNodeData::CollapsingHeader_KineticLaws],
				_equationNodeInfo.utilityState, EquationNodeData::State_CollHdrKineticLaws,
				startX, headerWidth,
				_equationNodeInfo.inputPins[EquationNodeData::InputPin_CollHdrKineticLaws], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_CollHdrKineticLaws],
				ImVec2(itemsWidth, 0.f), false))
			{
				NodeStringListBox(_equationNodeInfo.nlbsDataRKLDep, startX, headerWidth, itemsWidth);
			}
		}
	}

	NodeHorizontalSeparator(headerWidth);

	if (NodeCollapsingHeader_InOut("Data Fields", _equationNodeInfo.collapsingHeadersIds[EquationNodeData::CollapsingHeader_DataFields],
		_equationNodeInfo.utilityState, EquationNodeData::State_CollHdrDataFields,
		startX, headerWidth,
		_equationNodeInfo.inputPins[EquationNodeData::InputPin_CollHdrDataFields], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_CollHdrDataFields],
		ImVec2(itemsWidth, 0)))
	{

		if (NodeCollapsingHeader_InOut("Operands", _equationNodeInfo.collapsingHeadersIds[EquationNodeData::CollapsingHeader_EquationOperands],
			_equationNodeInfo.utilityState, EquationNodeData::State_CollHdrEquationOperands,
			startX, headerWidth,
			_equationNodeInfo.inputPins[EquationNodeData::InputPin_CollHdrEquationOperands], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_CollHdrEquationOperands],
			ImVec2(itemsWidth, 0.f)))
		{
			if (_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_SpeciesOperands].data->size())
			{
				const float spTextWidth = ImGui::CalcTextSize("Species").x;
				NodeText_InOut("Species", spTextWidth, startX, headerWidth,
					_equationNodeInfo.inputPins[EquationNodeData::InputPin_NLBSSpecies], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_NLBSSpecies]);
				NodeStringListBox(_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_SpeciesOperands], startX, headerWidth, itemsWidth);
			}

			if (_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_ParameterOperands].data->size())
			{
				const float sparamTextWidth = ImGui::CalcTextSize("Parameters").x;
				NodeText_InOut("Parameters", sparamTextWidth, startX, headerWidth,
					_equationNodeInfo.inputPins[EquationNodeData::InputPin_NLBSParameters], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_NLBSParameters]);
				NodeStringListBox(_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_ParameterOperands], startX, headerWidth, itemsWidth);
			}

			if (_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_EquationOperands].data->size())
			{
				const float cparaTextWidth = ImGui::CalcTextSize("Equations").x;
				NodeText_InOut("Equations", cparaTextWidth, startX, headerWidth,
					_equationNodeInfo.inputPins[EquationNodeData::InputPin_NLBSEquations], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_NLBSEquations]);
				NodeStringListBox(_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_EquationOperands], startX, headerWidth, itemsWidth);
			}
		}

		//const static float inputFieldWidth = headerWidth -  + ImGui::GetStyle().ItemSpacing.x; //100.0f is the default length for a field
		float value = _equationNodeInfo.data->Get();
		NodeInputFloat_InOut("Value", _equationNodeInfo.id.Get(), &value,
			itemsWidth, startX, headerWidth,
			_equationNodeInfo.inputPins[EquationNodeData::InputPin_EquationValue], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_EquationValue],
			ImGuiInputTextFlags_ReadOnly);
	}

	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::LinePlotNode(LinePlotNodeData& _linePlotNodeData)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Default));
	ax::NodeEditor::BeginNode(_linePlotNodeData.id);
	ImGui::PushID((std::size_t)_linePlotNodeData.id);

	const float headerWidth = NodeHeader("Line Plot", "", Style::EditorStyle::GetNodeColors(NodeType_Reaction), 300.f, 1, 1);
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth, 1);
	const float startX = ImGui::GetCursorPosX();
	ImGuiStyle& style = ImGui::GetStyle();

	if (NodeCollapsingHeader("All Parameters", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_AllParameters],
		_linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrAllParameters,
		startX, headerWidth, ImVec2(headerWidth, 0.f)))
	{
		if (NodeCollapsingHeader("General", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_GeneralParameters],
			_linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrGeneralParameters,
			startX, headerWidth, ImVec2(headerWidth, 0.f)))
		{
			char buffer[64] = "\0";
			NodeInputText("Plot Title", _linePlotNodeData.linePlot->plotTitle, buffer, 64, headerWidth, startX, headerWidth);

			NodeInputText("X Axis Label", _linePlotNodeData.linePlot->xAxisLabel, buffer, 64, headerWidth, startX, headerWidth);

			NodeInputText("Y Axis Label", _linePlotNodeData.linePlot->yAxisLabel, buffer, 64, headerWidth, startX, headerWidth);

			ImGui::SetNextItemWidth(headerWidth - ImGui::CalcTextSize("Plot Size").x - style.ItemSpacing.x);
			ImGui::DragFloat2("Plot Size", _linePlotNodeData.linePlot->plotSize, 1.f, 10.f, 1000.f);
		}

		if (NodeCollapsingHeader("Plot Flags", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_PlotFlags],
			_linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrPlotFlags,
			startX, headerWidth, ImVec2(headerWidth, 0.f)))
		{
			NodeAllImPlotFlags(&_linePlotNodeData.linePlot->plotFlags);
		}

		if (NodeCollapsingHeader("X Axis Flags", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_XAxisFlags],
			_linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrXAxisFlags,
			startX, headerWidth, ImVec2(headerWidth, 0.f)))
		{
			NodeAllImPlotAxisFlags(&_linePlotNodeData.linePlot->xAxisFlags);
		}

		if (NodeCollapsingHeader("Y Axis Flags", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_YAxisFlags],
			_linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrYAxisFlags,
			startX, headerWidth, ImVec2(headerWidth, 0.f)))
		{
			NodeAllImPlotAxisFlags(&_linePlotNodeData.linePlot->yAxisFlags);
		}

		if (NodeCollapsingHeader("Axis Scale Flags", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_AxisScaleFlags],
			_linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrAxisScaleFlags,
			startX, headerWidth, ImVec2(headerWidth, 0.f)))
		{
			NodeAllImPlotAxisScale(_linePlotNodeData.linePlot->xAxisScale, _linePlotNodeData.linePlot->yAxisScale);
		}
	}

	NodeHorizontalSeparator(headerWidth);

	if (NodeCollapsingHeader_In("Plot", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_Plot],
		_linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrPlot,
		startX, headerWidth,
		_linePlotNodeData.inputPins[LinePlotNodeData::InputPin_CollHdrPlot],
		ImVec2(itemsWidth, 0.f)))
	{
		NodeText_In(_linePlotNodeData.linePlot->xAxisLabel, startX, _linePlotNodeData.inputPins[LinePlotNodeData::InputPin_XAxis]);
		NodeText_In(_linePlotNodeData.linePlot->yAxisLabel, startX, _linePlotNodeData.inputPins[LinePlotNodeData::InputPin_YAxis]);

		//Draw Parameters for each line
		for (unsigned short i = 0; i < _linePlotNodeData.linePlot->lines.size(); i++)
		{
			ImGui::PushID(i);
			ApplyPinDrawOffset();
			ImGui::SetNextItemWidth(0.25f * itemsWidth);
			ImGui::InputText("##lineIdx", _linePlotNodeData.linePlot->lines[i]->lineLegend, 64, ImGuiInputTextFlags_EnterReturnsTrue);
			if (ImGui::IsItemActive())
			{
				ax::NodeEditor::Suspend();
				ImGui::SetTooltip("Press ENTER to confirm.");
				ax::NodeEditor::Resume();
			}

			ImGui::SameLine();
			bool open = ImGui::ColorButton("##lineColor", _linePlotNodeData.linePlot->lines[i]->color);
			ax::NodeEditor::Suspend();
			if (open)
			{
				ImGui::OpenPopup("##ColorPicker");
			}
			if (ImGui::BeginPopup("##ColorPicker"))
			{
				ImGui::ColorPicker4("##lineColorPicker", &_linePlotNodeData.linePlot->lines[i]->color.x);
				ImGui::EndPopup();
			}
			ax::NodeEditor::Resume();

			//ImGui::SameLine();
			//ImGui::AlignTextToFramePadding(); ImGui::Text("Update:");
			ImGui::SameLine();
			bool EveryNthFrame = std::strcmp(_linePlotNodeData.linePlot->lines[i]->GetCurrentUpdateSchemeName(), "EveryNthFrame") == 0;
			bool EveryXSeconds = std::strcmp(_linePlotNodeData.linePlot->lines[i]->GetCurrentUpdateSchemeName(), "EveryXSeconds") == 0;
			float width = headerWidth - (ImGui::GetCursorPosX() - startX);
			if (EveryNthFrame || EveryXSeconds)
			{
				width = 0.5f * (width - ImGui::GetStyle().ItemSpacing.x);
			}
			open = ImGui::Button(_linePlotNodeData.linePlot->lines[i]->GetCurrentUpdateSchemeName(), ImVec2(width, 0.f));

			if (EveryNthFrame)
			{
				ImGui::SameLine();
				ImGui::SetNextItemWidth(width);
				ImGui::DragInt("##nbFrames", (int*)_linePlotNodeData.linePlot->lines[i]->updateStrategy->Get(),
					1.f, 1, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);
			}

			else if (EveryXSeconds)
			{
				ImGui::SameLine();
				ImGui::SetNextItemWidth(width);
				ImGui::DragFloat("##xSeconds", (float*)_linePlotNodeData.linePlot->lines[i]->updateStrategy->Get(),
					0.1f, 0.1f, FLT_MAX, "%.1f", ImGuiSliderFlags_AlwaysClamp);
			}

			ax::NodeEditor::Suspend();
			if (open)
			{
				ImGui::OpenPopup("##combo");
			}

			if (ImGui::BeginPopup("##combo", ImGuiWindowFlags_NoMove))
			{
				for (int j = 0; j < Utility::Plot::UpdateStrategyType_Count; ++j)
				{
					if (ImGui::MenuItem(Utility::Plot::UpdateStrategy::GetName((Utility::Plot::UpdateStrategyType)j)))
					{
						_linePlotNodeData.linePlot->lines[i]->SwitchUpdateStrategy(j);
					}
				}
				ImGui::EndPopup();
			}
			ax::NodeEditor::Resume();

			ImGui::PopID();
		}

		AlignToCenter(startX, headerWidth, headerWidth);

		if (_linePlotNodeData.IsPlotOpen())
		{
			if (ImGui::Button("Show##", ImVec2(0.5f * (headerWidth - style.ItemSpacing.x), 0.f)))
			{
				std::string windowName = "Line Plot##lineplot" + std::to_string((std::size_t)_linePlotNodeData.id);
				ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName(windowName.c_str()));
				ImGui::SetNextWindowPos(ax::NodeEditor::CanvasToScreen(ImGui::GetCursorPos()));
			}
			ImGui::SameLine();
			if (ImGui::Button("Close##", ImVec2(0.5f * (headerWidth - style.ItemSpacing.x), 0.f)))
			{
				_linePlotNodeData.SwitchState(LinePlotNodeData::State_PlotIsOpen);
			}
		}
		else
		{
			if (ImGui::Button("Open##", ImVec2(headerWidth, 0.f)))
			{
				_linePlotNodeData.SwitchState(LinePlotNodeData::State_PlotIsOpen);
				ImGui::SetNextWindowPos(ax::NodeEditor::CanvasToScreen(ImGui::GetCursorPos()));
			}
		}
	}

	if (_linePlotNodeData.IsPlotOpen())
	{
		ImVec2& windowPadding = ImGui::GetStyle().WindowPadding;

		ImGui::SetNextWindowSize(ImVec2(_linePlotNodeData.linePlot->plotSize[0] + 2 * windowPadding.x,
			_linePlotNodeData.linePlot->plotSize[1] + 2 * windowPadding.y));
		ax::NodeEditor::Suspend();
		std::string windowName = "Line Plot##lineplot"+std::to_string((std::size_t)_linePlotNodeData.id);
		if (ImGui::Begin(windowName.c_str(), NULL, _linePlotNodeData.linePlot->plotWindowFlags))
		{
			_linePlotNodeData.linePlot->Draw();
		}
		ImGui::End();
		ax::NodeEditor::Resume();
	}

	ImGui::PopID();
	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::LogicOperationNode(LogicOperationNodeData& _logicOperationNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_LogicOperation));
	ax::NodeEditor::BeginNode(_logicOperationNodeInfo.id);
	ImGui::PushID((std::size_t)_logicOperationNodeInfo.id);
	const float headerWidth = NodeHeader("Logic Operation", "", Style::EditorStyle::GetNodeColors(NodeType_Data), 250.f);
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();
	const float onOperandChangeWidth = ImGui::CalcTextSize("onOperandChange").x;
	const float onResultChangeWidth = ImGui::CalcTextSize("onResultChange").x;
	std::string textValue = "";

	if (NodeComboBox("Logical operator", _logicOperationNodeInfo.operatorTypes, 3, (int&)_logicOperationNodeInfo.data->GetLogicalOperator(),
		itemsWidth, startX, headerWidth))
	{
		_logicOperationNodeInfo.data->SetLogic();
		//We use UpdateLHS to trigger the update of the result.
		//We chose parameter values that will not change the value of LHS but will surely trigger
		//the onOperandChange callback as well as onResultChange callback.
		_logicOperationNodeInfo.data->UpdateLHS(!_logicOperationNodeInfo.lhs, _logicOperationNodeInfo.lhs);
	}

	if (!_logicOperationNodeInfo.inputPins[LogicOperationNodeData::InputPin_LHS].nbConnectedLinks > 0)
	{
		NodeText_In("LHS", startX, _logicOperationNodeInfo.inputPins[LogicOperationNodeData::InputPin_LHS]);
		ImGui::SameLine();
		bool previousLHS = _logicOperationNodeInfo.lhs;
		if (ImGui::Checkbox("##LHS", &_logicOperationNodeInfo.lhs))
		{
			_logicOperationNodeInfo.data->UpdateLHS(previousLHS, _logicOperationNodeInfo.lhs);
		}
	}
	else
	{
		_logicOperationNodeInfo.lhs = _logicOperationNodeInfo.data->GetLHS();
		textValue = "LHS (";
		textValue += _logicOperationNodeInfo.lhs ? "true)" : "false)";
		NodeText_In(textValue.c_str(), startX, _logicOperationNodeInfo.inputPins[LogicOperationNodeData::InputPin_LHS]);
	}

	ImGui::SameLine();

	NodeText_Out("onOperandChange", onOperandChangeWidth, startX, headerWidth,
		_logicOperationNodeInfo.outputPins[LogicOperationNodeData::OutputPin_OnOperandChange]);

	if (!_logicOperationNodeInfo.inputPins[LogicOperationNodeData::InputPin_RHS].nbConnectedLinks > 0)
	{
		NodeText_In("RHS", startX, _logicOperationNodeInfo.inputPins[LogicOperationNodeData::InputPin_RHS]);
		ImGui::SameLine();
		bool previousRHS = _logicOperationNodeInfo.rhs;
		if (ImGui::Checkbox("##RHS", &_logicOperationNodeInfo.rhs))
		{
			_logicOperationNodeInfo.data->UpdateRHS(previousRHS, _logicOperationNodeInfo.rhs);
		}
	}
	else
	{
		_logicOperationNodeInfo.rhs = _logicOperationNodeInfo.data->GetRHS();
		textValue = "RHS (";
		textValue += _logicOperationNodeInfo.rhs ? "true)" : "false)";
		NodeText_In(textValue.c_str(), startX, _logicOperationNodeInfo.inputPins[LogicOperationNodeData::InputPin_RHS]);
	}

	ImGui::SameLine();

	NodeText_Out("onResultChange", onResultChangeWidth, startX, headerWidth,
		_logicOperationNodeInfo.outputPins[LogicOperationNodeData::OutputPin_OnResultChange]);

	ImGui::PopID();
	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::ModifyDataStateValueEventNode(ModifyDataStateValueEventNodeData& _modifyDSValueEventNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Event));
	ax::NodeEditor::BeginNode(_modifyDSValueEventNodeInfo.id);

	const float headerWidth = NodeHeader("Modify DataState Value Event", "", Style::EditorStyle::GetNodeColors(NodeType_Event));
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();
	ImGuiSliderFlags dragFlags = _modifyDSValueEventNodeInfo.inputPins[ModifyDataStateValueEventNodeData::InputPin_FloatValue].nbConnectedLinks > 0 ? ImGuiSliderFlags_ReadOnly : ImGuiSliderFlags_None;

	_modifyDSValueEventNodeInfo.value = _modifyDSValueEventNodeInfo.data->GetValue();
	float buffer = _modifyDSValueEventNodeInfo.value;
	if (NodeDragFloat_In("New Value", _modifyDSValueEventNodeInfo.inputPins[ModifyDataStateValueEventNodeData::InputPin_FloatValue],
		&buffer, itemsWidth, startX, _modifyDSValueEventNodeInfo.inputPins[ModifyDataStateValueEventNodeData::InputPin_FloatValue], dragFlags))
	{
		_modifyDSValueEventNodeInfo.data->UpdateValue(_modifyDSValueEventNodeInfo.value, buffer);
	}

	NodeHorizontalSeparator(headerWidth);

	if (NodeCollapsingHeader_InOut("Execution", _modifyDSValueEventNodeInfo.collapsingHeadersIds[ModifyDataStateValueEventNodeData::CollapsingHeader_Execution],
		_modifyDSValueEventNodeInfo.utilityState, ModifyDataStateValueEventNodeData::State_CollHdrExecution,
		startX, headerWidth,
		_modifyDSValueEventNodeInfo.inputPins[ModifyDataStateValueEventNodeData::InputPin_CollHdrExecution],
		_modifyDSValueEventNodeInfo.outputPins[ModifyDataStateValueEventNodeData::OutputPin_CollHdrExecution], ImVec2(itemsWidth, 0.f)))
	{
		ImGui::SetCursorPosX(startX + GetPinDrawOffset());

		ImGui::Checkbox("##ManualExecution", &_modifyDSValueEventNodeInfo.activateManualExecution);
		if (ImGui::IsItemHovered())
		{
			ax::NodeEditor::Suspend();
			ImGui::SetTooltip("Activate Manual Execution");
			ax::NodeEditor::Resume();
		}

		ImGui::SameLine();
		if (!_modifyDSValueEventNodeInfo.activateManualExecution)
		{
			ImGui::BeginDisabled();
		}
		
		if (ImGui::Button("Execute", ImVec2(itemsWidth - ImGui::GetTextLineHeight() - 2*ImGui::GetStyle().FramePadding.y - ImGui::GetStyle().ItemSpacing.x, 0.f)))
		{
			//TODO Simulation Index MUST NOT be hardcoded
			//Give an accessor from the context? Since MNBV context is actually binded to only one simulation.
			_modifyDSValueEventNodeInfo.data->Execute(0);
		}

		if (!_modifyDSValueEventNodeInfo.activateManualExecution)
		{
			ImGui::EndDisabled();
		}

		if (!_modifyDSValueEventNodeInfo.inputPins[ModifyDataStateValueEventNodeData::InputPin_Condition].nbConnectedLinks > 0)
		{
			NodeText_In("Condition", startX,
				_modifyDSValueEventNodeInfo.inputPins[ModifyDataStateValueEventNodeData::InputPin_Condition]);

			ImGui::SameLine();
			bool buffer2 = _modifyDSValueEventNodeInfo.data->GetCondition();
			if (ImGui::Checkbox("##ConditionInput", &buffer2))
			{
				_modifyDSValueEventNodeInfo.data->UpdateCondition(_modifyDSValueEventNodeInfo.data->GetCondition(), buffer2);
			}
		}
		else
		{
			std::string condition = "Condition (";
			condition += _modifyDSValueEventNodeInfo.data->GetCondition() ? "true)" : "false)";
			NodeText_In(condition.c_str(), startX,
						_modifyDSValueEventNodeInfo.inputPins[ModifyDataStateValueEventNodeData::InputPin_Condition]);
		}

		ImGui::SameLine();

		NodeText_Out("Modify", ImGui::CalcTextSize("Modify").x, startX, headerWidth,
			_modifyDSValueEventNodeInfo.outputPins[ModifyDataStateValueEventNodeData::OutputPin_Modify]);
	}

	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::ReactionNode(const char* _name, ReactionNodeData& _reactionNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Reaction));
	ax::NodeEditor::BeginNode(_reactionNodeInfo.id);

	_reactionNodeInfo.Update();

	const float headerWidth = NodeHeader("Reaction:", _name, Style::EditorStyle::GetNodeColors(NodeType_Reaction));
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();

	if (NodeCollapsingHeader_InOut("Model Links", _reactionNodeInfo.collapsingHeadersIds[ReactionNodeData::CollapsingHeader_ModelLinks],
		_reactionNodeInfo.utilityState, ReactionNodeData::State_CollHdrModelLinks,
		startX, headerWidth,
		_reactionNodeInfo.inputPins[ReactionNodeData::InputPin_CollHdrModelLinks], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_CollHdrModelLinks],
		ImVec2(itemsWidth, 0.f)))
	{

		NodeText_In("Asset", startX, _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_Asset]);

		if (NodeCollapsingHeader_InOut("Reactants", _reactionNodeInfo.collapsingHeadersIds[ReactionNodeData::CollapsingHeader_Reactants],
			_reactionNodeInfo.utilityState, ReactionNodeData::State_CollHdrReactants,
			startX, headerWidth,
			_reactionNodeInfo.inputPins[ReactionNodeData::InputPin_CollHdrReactants], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_CollHdrReactants],
			ImVec2(itemsWidth, 0.f), false))
		{
			NodeStringListBox(_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_Reactants], startX, headerWidth, itemsWidth);
		}

		if (NodeCollapsingHeader_InOut("Products", _reactionNodeInfo.collapsingHeadersIds[ReactionNodeData::CollapsingHeader_Products],
			_reactionNodeInfo.utilityState, ReactionNodeData::State_CollHdrProducts,
			startX, headerWidth,
			_reactionNodeInfo.inputPins[ReactionNodeData::InputPin_CollHdrProducts], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_CollHdrProducts],
			ImVec2(itemsWidth, 0.f), false))
		{
			NodeStringListBox(_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_Products], startX, headerWidth, itemsWidth);
		}
	}

	NodeHorizontalSeparator(headerWidth);

	if (NodeCollapsingHeader_InOut("Kinetic Law", _reactionNodeInfo.collapsingHeadersIds[ReactionNodeData::CollapsingHeader_KineticLaw],
		_reactionNodeInfo.utilityState, ReactionNodeData::State_CollHdrKineticLaw,
		startX, headerWidth,
		_reactionNodeInfo.inputPins[ReactionNodeData::InputPin_CollHdrKineticLaw], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_CollHdrKineticLaw],
		ImVec2(itemsWidth, 0)))
	{

		if (NodeCollapsingHeader_InOut("Operands", _reactionNodeInfo.collapsingHeadersIds[ReactionNodeData::CollapsingHeader_KineticLawOperands],
			_reactionNodeInfo.utilityState, ReactionNodeData::State_CollHdrKineticLawOperands,
			startX, headerWidth,
			_reactionNodeInfo.inputPins[ReactionNodeData::InputPin_CollHdrKineticLawOperands], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_CollHdrKineticLawOperands],
			ImVec2(itemsWidth, 0.f)))
		{
			if (_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_SpeciesOperands].data->size())
			{
				const float spTextWidth = ImGui::CalcTextSize("Species").x;
				NodeText_InOut("Species", spTextWidth, startX, headerWidth,
					_reactionNodeInfo.inputPins[ReactionNodeData::InputPin_NLBSSpecies], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_NLBSSpecies]);
				NodeStringListBox(_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_SpeciesOperands], startX, headerWidth, itemsWidth);
			}

			if (_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_ParameterOperands].data->size())
			{
				const float sparamTextWidth = ImGui::CalcTextSize("Parameters").x;
				NodeText_InOut("Parameters", sparamTextWidth, startX, headerWidth,
					_reactionNodeInfo.inputPins[ReactionNodeData::InputPin_NLBSParameters], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_NLBSParameters]);
				NodeStringListBox(_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_ParameterOperands], startX, headerWidth, itemsWidth);
			}

			if (_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_EquationOperands].data->size())
			{
				const float cparaTextWidth = ImGui::CalcTextSize("Equations").x;
				NodeText_InOut("Equations", cparaTextWidth, startX, headerWidth,
					_reactionNodeInfo.inputPins[ReactionNodeData::InputPin_NLBSEquations], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_NLBSEquations]);
				NodeStringListBox(_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_EquationOperands], startX, headerWidth, itemsWidth);
			}
		}

		float value = _reactionNodeInfo.data->GetKineticLawValue();
		NodeInputFloat_InOut("Value", _reactionNodeInfo.id.Get(), &value,
			itemsWidth, startX, headerWidth,
			_reactionNodeInfo.inputPins[ReactionNodeData::InputPin_KineticLawValue], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_KineticLawValue],
			ImGuiInputTextFlags_ReadOnly);
	}

	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::ParameterNode(const char* _name, ParameterNodeData& _parameterNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Parameter));
	ax::NodeEditor::BeginNode(_parameterNodeInfo.id);

	_parameterNodeInfo.Update();

	const float headerWidth = NodeHeader("Parameter:", _name, Style::EditorStyle::GetNodeColors(NodeType_Parameter));
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();
	const float thisWidth = ImGui::CalcTextSize("this").x;

	if (NodeCollapsingHeader_InOut("Model Links", _parameterNodeInfo.collapsingHeadersIds[ParameterNodeData::CollapsingHeader_ModelLinks],
		_parameterNodeInfo.utilityState, ParameterNodeData::State_CollHdrModelLinks,
		startX, headerWidth,
		_parameterNodeInfo.inputPins[ParameterNodeData::InputPin_CollHdrModelLinks], _parameterNodeInfo.outputPins[ParameterNodeData::OutputPin_CollHdrModelLinks],
		ImVec2(itemsWidth, 0.f)))
	{

		NodeText_In("Origin", startX, _parameterNodeInfo.inputPins[ParameterNodeData::InputPin_Asset]);

		ImGui::SameLine();

		NodeText_Out("this", thisWidth, startX, headerWidth,
			_parameterNodeInfo.outputPins[SpeciesNodeData::OutputPin_ThisData]);

		if (_parameterNodeInfo.equationDep.size())
		{
			if (NodeCollapsingHeader_InOut("In Equations", _parameterNodeInfo.collapsingHeadersIds[ParameterNodeData::CollapsingHeader_Equations],
				_parameterNodeInfo.utilityState, ParameterNodeData::State_CollHdrEquations,
				startX, headerWidth,
				_parameterNodeInfo.inputPins[ParameterNodeData::InputPin_CollHdrEquations], _parameterNodeInfo.outputPins[ParameterNodeData::OutputPin_CollHdrEquations],
				ImVec2(itemsWidth, 0.f), false))
			{
				NodeStringListBox(_parameterNodeInfo.nlbsDataEqDep, startX, headerWidth, itemsWidth);
			}
		}
		if (_parameterNodeInfo.reactionKLDep.size())
		{
			if (NodeCollapsingHeader_InOut("In Kinetic Laws", _parameterNodeInfo.collapsingHeadersIds[ParameterNodeData::CollapsingHeader_KineticLaws],
				_parameterNodeInfo.utilityState, ParameterNodeData::State_CollHdrKineticLaws,
				startX, headerWidth,
				_parameterNodeInfo.inputPins[ParameterNodeData::InputPin_CollHdrKineticLaws], _parameterNodeInfo.outputPins[ParameterNodeData::OutputPin_CollHdrKineticLaws],
				ImVec2(itemsWidth, 0.f), false))
			{
				NodeStringListBox(_parameterNodeInfo.nlbsDataRKLDep, startX, headerWidth, itemsWidth);
			}
		}
	}

	NodeHorizontalSeparator(headerWidth);

	if (NodeCollapsingHeader_InOut("Data Fields", _parameterNodeInfo.collapsingHeadersIds[ParameterNodeData::CollapsingHeader_DataFields],
		_parameterNodeInfo.utilityState, ParameterNodeData::State_CollHdrDataFields,
		startX, headerWidth,
		_parameterNodeInfo.inputPins[ParameterNodeData::InputPin_CollHdrDataFields], _parameterNodeInfo.outputPins[ParameterNodeData::OutputPin_CollHdrDataFields],
		ImVec2(itemsWidth, 0)))
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
		if (_parameterNodeInfo.inputPins[ParameterNodeData::InputPin_ParameterValue].nbConnectedLinks > 0)
		{
			flags |= ImGuiInputTextFlags_ReadOnly;
		}
		_parameterNodeInfo.parameterValueBuffer = _parameterNodeInfo.data->Get();
		if (NodeInputFloat_InOut("Value", _parameterNodeInfo.id.Get(), &_parameterNodeInfo.parameterValueBuffer,
			itemsWidth, startX, headerWidth,
			_parameterNodeInfo.inputPins[ParameterNodeData::InputPin_ParameterValue], _parameterNodeInfo.outputPins[ParameterNodeData::OutputPin_ParameterValue],
			flags))
		{
			_parameterNodeInfo.data->UpdateValue(_parameterNodeInfo.data->Get(), _parameterNodeInfo.parameterValueBuffer);
		}
	}

	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::SolverNode(const char* _name, SolverNodeData& _solverNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Solver));
	ax::NodeEditor::BeginNode(_solverNodeInfo.id);

	const float headerWidth = NodeHeader("Solver:", _name, Style::EditorStyle::GetNodeColors(NodeType_Solver));
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();

	NodeText_In("Data", startX, _solverNodeInfo.inputPins[SolverNodeData::InputPin_Data]);

	if (NodeCollapsingHeader_Out("Time", _solverNodeInfo.collapsingHeadersIds[SolverNodeData::CollapsingHeader_Time],
		_solverNodeInfo.utilityState, SolverNodeData::State_CollHdrTime,
		startX, headerWidth, _solverNodeInfo.outputPins[SolverNodeData::OutputPin_CollHdrTime],
		ImVec2(itemsWidth, 0.f)))
	{
		NodeInputFloat_Out("Delta Time", _solverNodeInfo.id.Get(), &_solverNodeInfo.data->GetStepper()->timer.deltaTime,
			itemsWidth, startX, headerWidth,
			_solverNodeInfo.outputPins[SolverNodeData::OutputPin_DeltaTime],
			ImGuiInputTextFlags_ReadOnly);

		NodeInputFloat_Out("Elapsed Time", _solverNodeInfo.id.Get(), &_solverNodeInfo.data->GetStepper()->timer.elapsedTime,
			itemsWidth, startX, headerWidth,
			_solverNodeInfo.outputPins[SolverNodeData::OutputPin_ElapsedTime],
			ImGuiInputTextFlags_ReadOnly);

		NodeInputFloat_Out("Start Time", _solverNodeInfo.id.Get(), &_solverNodeInfo.data->GetStepper()->timer.startTime,
			itemsWidth, startX, headerWidth,
			_solverNodeInfo.outputPins[SolverNodeData::OutputPin_StartTime],
			ImGuiInputTextFlags_ReadOnly);
	}

	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::SpeciesNode(const char* _name, SpeciesNodeData& _speciesNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Species));
	ax::NodeEditor::BeginNode(_speciesNodeInfo.id);

	_speciesNodeInfo.Update();

	const float headerWidth = NodeHeader("Species:", _name, Style::EditorStyle::GetNodeColors(NodeType_Species));
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();
	const float thisWidth = ImGui::CalcTextSize("this").x;

	if (NodeCollapsingHeader_InOut("Model Links", _speciesNodeInfo.collapsingHeadersIds[SpeciesNodeData::CollapsingHeader_ModelLinks],
		_speciesNodeInfo.utilityState, SpeciesNodeData::State_CollHdrModelLinks,
		startX, headerWidth,
		_speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrModelLinks], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrModelLinks],
		ImVec2(itemsWidth, 0)))
	{
		NodeText_In("Origin", startX, _speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_Asset]);

		ImGui::SameLine();

		NodeText_Out("this", thisWidth, startX, headerWidth,
			_speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_ThisData]);

		if (_speciesNodeInfo.equationDep.size())
		{
			if (NodeCollapsingHeader_InOut("In Equations", _speciesNodeInfo.collapsingHeadersIds[SpeciesNodeData::CollapsingHeader_InEquation],
				_speciesNodeInfo.utilityState, SpeciesNodeData::State_CollHdrInEquation,
				startX, headerWidth,
				_speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrInEquation], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrInEquation],
				ImVec2(itemsWidth, 0.f), false))
			{

				NodeStringListBox(_speciesNodeInfo.nlbsDataEqDep, startX, headerWidth, itemsWidth);
			}
		}
		
		if (_speciesNodeInfo.reactionRDep.size())
		{
			if (NodeCollapsingHeader_InOut("As Reactant", _speciesNodeInfo.collapsingHeadersIds[SpeciesNodeData::CollapsingHeader_AsReactant],
				_speciesNodeInfo.utilityState, SpeciesNodeData::State_CollHdrAsReactant,
				startX, headerWidth,
				_speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrAsReactant], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrAsReactant],
				ImVec2(itemsWidth, 0.f), false))
			{

				NodeStringListBox(_speciesNodeInfo.nlbsDataRRDep, startX, headerWidth, itemsWidth);
			}
		}
		
		if (_speciesNodeInfo.reactionPDep.size())
		{
			if (NodeCollapsingHeader_InOut("As Product", _speciesNodeInfo.collapsingHeadersIds[SpeciesNodeData::CollapsingHeader_AsProduct],
				_speciesNodeInfo.utilityState, SpeciesNodeData::State_CollHdrAsProduct,
				startX, headerWidth,
				_speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrAsProduct], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrAsProduct],
				ImVec2(itemsWidth, 0.f), false))
			{

				NodeStringListBox(_speciesNodeInfo.nlbsDataRPDep, startX, headerWidth, itemsWidth);
			}
		}
		
		if (_speciesNodeInfo.reactionKLDep.size())
		{
			if (NodeCollapsingHeader_InOut("In Kinetic Laws", _speciesNodeInfo.collapsingHeadersIds[SpeciesNodeData::CollapsingHeader_InKineticLaw],
				_speciesNodeInfo.utilityState, SpeciesNodeData::State_CollHdrInKineticLaw,
				startX, headerWidth,
				_speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrInKineticLaw], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrInKineticLaw],
				ImVec2(itemsWidth, 0.f), false))
			{

				NodeStringListBox(_speciesNodeInfo.nlbsDataRKLDep, startX, headerWidth, itemsWidth);
			}
		}
	}

	NodeHorizontalSeparator(headerWidth);

	if (NodeCollapsingHeader_InOut("Data Fields", _speciesNodeInfo.collapsingHeadersIds[SpeciesNodeData::CollapsingHeader_DataFields],
		_speciesNodeInfo.utilityState, SpeciesNodeData::State_CollHdrDataFields,
		startX, headerWidth,
		_speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrDataFields], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrDataFields],
		ImVec2(itemsWidth, 0)))
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
		if (_speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_Quantity].nbConnectedLinks > 0)
		{
			flags |= ImGuiInputTextFlags_ReadOnly;
		}
		_speciesNodeInfo.speciesQuantityBuffer = _speciesNodeInfo.data->Get();
		if (NodeInputFloat_InOut("Quantity", _speciesNodeInfo.id.Get(), &_speciesNodeInfo.speciesQuantityBuffer,
			itemsWidth, startX, headerWidth,
			_speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_Quantity], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_Quantity],
			flags))
		{
			_speciesNodeInfo.data->UpdateQuantity(_speciesNodeInfo.data->Get(), _speciesNodeInfo.speciesQuantityBuffer);
		}
	}

	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::TimeNode(const char* _name, TimeNodeData& _timeNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Data));
	ax::NodeEditor::BeginNode(_timeNodeInfo.id);

	const float headerWidth = NodeHeader("Time:", _name, Style::EditorStyle::GetNodeColors(NodeType_Data));
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();

	NodeInputFloat_Out("Delta Time", _timeNodeInfo.id.Get(), &_timeNodeInfo.timer->deltaTime,
		itemsWidth, startX, headerWidth,
		_timeNodeInfo.outputPins[TimeNodeData::OutputPin_DeltaTime],
		ImGuiInputTextFlags_ReadOnly);

	NodeInputFloat_Out("Elapsed Time", _timeNodeInfo.id.Get(), &_timeNodeInfo.timer->elapsedTime,
		itemsWidth, startX, headerWidth,
		_timeNodeInfo.outputPins[TimeNodeData::OutputPin_ElapsedTime],
		ImGuiInputTextFlags_ReadOnly);

	NodeInputFloat_Out("Start Time", _timeNodeInfo.id.Get(), &_timeNodeInfo.timer->startTime,
		itemsWidth, startX, headerWidth,
		_timeNodeInfo.outputPins[TimeNodeData::OutputPin_StartTime],
		ImGuiInputTextFlags_ReadOnly);

	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::TriggerNode(TriggerNodeData& _triggerNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Event));
	ax::NodeEditor::BeginNode(_triggerNodeInfo.id);
	ImGui::PushID((std::size_t)_triggerNodeInfo.id);

	const float headerWidth = NodeHeader("Trigger", "", Style::EditorStyle::GetNodeColors(NodeType_Event), 300.f);
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();
	const float onTriggerEnterWidth = ImGui::CalcTextSize("onTriggerEnter").x;
	const float onTriggerStayWidth = ImGui::CalcTextSize("onTriggerStay").x;
	const float onTriggerExitWidth = ImGui::CalcTextSize("onTriggerExit").x;

	if (NodeComboBox("Comparator", _triggerNodeInfo.comparators, 6, (int&)_triggerNodeInfo.data->GetComparator(),
		itemsWidth, startX, headerWidth))
	{
		_triggerNodeInfo.data->UpdateAndCall();
	}

	ImGuiSliderFlags dragFlags = ImGuiSliderFlags_None;
	if (_triggerNodeInfo.inputPins[TriggerNodeData::InputPin_Target].nbConnectedLinks > 0)
	{
		dragFlags |= ImGuiSliderFlags_ReadOnly;
		_triggerNodeInfo.target.Set(_triggerNodeInfo.data->GetTarget()->Get());
	}
	float bufferTarget = _triggerNodeInfo.target.Get();
	if (NodeDragFloat_In("Target", _triggerNodeInfo.inputPins[TriggerNodeData::InputPin_Target], &bufferTarget,
		0.5f * itemsWidth, startX,
		_triggerNodeInfo.inputPins[TriggerNodeData::InputPin_Target],
		dragFlags))
	{
		_triggerNodeInfo.target.Set(bufferTarget);
		_triggerNodeInfo.data->UpdateAndCall();
	}

	ImGui::SameLine();

	NodeText_Out("onTriggerEnter", onTriggerEnterWidth, startX, headerWidth,
		_triggerNodeInfo.outputPins[TriggerNodeData::OutputPin_OnTriggerEnter]);

	dragFlags = ImGuiSliderFlags_None;
	if (_triggerNodeInfo.inputPins[TriggerNodeData::InputPin_Threshold].nbConnectedLinks > 0)
	{
		dragFlags |= ImGuiSliderFlags_ReadOnly;
		_triggerNodeInfo.threshold.Set(_triggerNodeInfo.data->GetThreshold()->Get());
	}
	float bufferThreshold = _triggerNodeInfo.threshold.Get();
	if (NodeDragFloat_In("Threshold", _triggerNodeInfo.inputPins[TriggerNodeData::InputPin_Threshold], &bufferThreshold,
		0.5f * itemsWidth, startX,
		_triggerNodeInfo.inputPins[TriggerNodeData::InputPin_Threshold],
		dragFlags))
	{
		_triggerNodeInfo.threshold.Set(bufferThreshold);
		_triggerNodeInfo.data->UpdateAndCall();
	}

	ImGui::SameLine();

	NodeText_Out("onTriggerStay", onTriggerStayWidth, startX, headerWidth,
		_triggerNodeInfo.outputPins[TriggerNodeData::OutputPin_OnTriggerStay]);

	NodeText_Out("onTriggerExit", onTriggerExitWidth, startX, headerWidth,
		_triggerNodeInfo.outputPins[TriggerNodeData::OutputPin_OnTriggerExit]);

	ImGui::PopID();
	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::ValueFloatNode(const char* _name, ValueFloatNodeData& _valueFloatNodeInfo)
{
	Style::NodeEditorStyle::PushNodeStyle(Style::EditorStyle::GetNodeColors(NodeType_Data));
	ax::NodeEditor::BeginNode(_valueFloatNodeInfo.id);

	const float headerWidth = NodeHeader("Value:", _name, Style::EditorStyle::GetNodeColors(NodeType_Data));
	const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
	const float startX = ImGui::GetCursorPosX();

	float buffer = _valueFloatNodeInfo.value.Get();
	if (NodeDragFloat_Out("Value", _valueFloatNodeInfo.outputPins[ValueFloatNodeData::OutputPin_Value], &buffer,
		itemsWidth, startX, headerWidth,
		_valueFloatNodeInfo.outputPins[ValueFloatNodeData::OutputPin_Value]))
	{
		_valueFloatNodeInfo.value.UpdateValue(_valueFloatNodeInfo.value.Get(), buffer);
	}

	ax::NodeEditor::EndNode();
	Style::NodeEditorStyle::PopNodeStyle();
}
#pragma endregion

#pragma region Node Pins
void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::Pin(const NodePinData& _pinData)
{
	ax::NodeEditor::BeginPin(_pinData.id, _pinData.kind);

	//We center on the Y axis the start position of the cursor relatively to texts within buttons in ImGui
	//We chose buttons as reference as they are used extensively in nodes.
	const ImVec2 startPos = ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 0.5f * ImGui::GetTextLineHeight());
	const ImVec2 endPos = ImVec2(startPos.x + Style::EditorStyle::GetMNBVStyle().pinWidth, startPos.y + Style::EditorStyle::GetMNBVStyle().pinWidth);

	const ImRect bb(startPos, endPos);
	ax::NodeEditor::PinRect(bb.Min, bb.Max);
	Style::PinColorType bgColor = Style::PinColorType_BgInactivated;
	ImGui::ItemAdd(bb, (std::size_t)_pinData.id);
	if (_pinData.nbConnectedLinks > 0 || ImGui::IsItemHovered())
	{
		bgColor = Style::PinColorType_BgActivated;
	}

	//Drawing the center of the pin
	ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, ImColor(Style::EditorStyle::GetPinColors(_pinData.type)[bgColor]));
	//Going back to start position
	ImGui::SetCursorPos(startPos);
	//Drawing the border of the pin on top of the center.
	ImGui::GetWindowDrawList()->AddRect(bb.Min, bb.Max, ImColor(Style::EditorStyle::GetPinColors(_pinData.type)[Style::PinColorType_Border]));

	// We set the cursor here to where it already is trigger the automatic addition of the Itemspacing.x 
	// at the end of the pin when anything is drawn on the same line after it.
	ImGui::SetCursorPosX(endPos.x);

	ax::NodeEditor::EndPin();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::LinkCreation(std::vector<LinkData>& _links)
{
	// Handle creation action, returns true if editor want to create new object (node or link)
	if (ax::NodeEditor::BeginCreate())
	{
		ax::NodeEditor::PinId startPinId, endPinId;
		if (ax::NodeEditor::QueryNewLink(&startPinId, &endPinId))
		{
			// QueryNewLink returns true if editor want to create new link between pins.
			//
			// Link can be created only for two valid pins, it is up to you to
			// validate if connection make sense. Editor is happy to make any.
			//
			// Link always goes from input to output. User may choose to drag
			// link from output pin or input pin. This determine which pin ids
			// are valid and which are not:
			//   * input valid, output invalid - user started to drag new link from input pin
			//   * input invalid, output valid - user started to drag new link from output pin
			//   * input valid, output valid   - user dragged link over other pin, can be validated

			if (startPinId && endPinId) // both are valid, let's accept link
			{
				NodePinData* startPin = Widget::MNBV::FindNodePinInAll((std::size_t)startPinId);
				NodePinData* endPin = Widget::MNBV::FindNodePinInAll((std::size_t)endPinId);

				//The start pin of a link is the output of a node
				if (startPin->kind == ax::NodeEditor::PinKind::Input)
				{
					std::swap(startPin, endPin);
					std::swap(startPinId, endPinId);
				}

				//If pin is itself --> reject
				if (startPin == endPin)
				{
					ax::NodeEditor::RejectNewItem(ImVec4(1.0f, 0.f, 0.f, 1.0f), 8.0f);
					ax::NodeEditor::Suspend();
					ImGui::SetTooltip("You cannot connect a pin to itself.");
					ax::NodeEditor::Resume();
				}

				//If both pins qre input or output
				else if (startPin->kind == endPin->kind)
				{
					ax::NodeEditor::RejectNewItem(ImVec4(1.0f, 0.f, 0.f, 1.0f), 8.0f);
					ax::NodeEditor::Suspend();
					ImGui::SetTooltip("You cannot connect an INPUT pin to another INPUT pin\nor OUTPUT pin to another OUTPUT pin.");
					ax::NodeEditor::Resume();
				}

				//If pins types are not compatible (see ModelNodeBasedViewerContext.hpp)
				else if (!Widget::MNBV::IsDynamicLinkAuthorized(startPin->type, endPin->type))
				{
					ax::NodeEditor::RejectNewItem(ImVec4(1.0f, 0.f, 0.f, 1.0f), 8.0f);
					ax::NodeEditor::Suspend();
					ImGui::SetTooltip("You cannot connect a pin of type %s to a pin of type %s", Style::EditorStyle::GetPinTypeName(startPin->type), Style::EditorStyle::GetPinTypeName(endPin->type));
					ax::NodeEditor::Resume();
				}

				//If output pin has too many connections
				else if (startPin->nbConnectedLinks >= startPin->maxNbConnectedLinks)
				{
					//We handle the error message a bit differently here to be able to display the
					//error message for both pins at the same time if it happens.

					ax::NodeEditor::Suspend();
					
					std::string msg = "OUTPUT pin has already reached max\nnumber of connections (%d link(s)).\nDelete a link before using this pin.";
					ax::NodeEditor::RejectNewItem(ImVec4(1.0f, 0.f, 0.f, 1.0f), 8.0f);
					
					//If input pin also has too many connections
					if (endPin->nbConnectedLinks >= endPin->maxNbConnectedLinks)
					{
						msg += "\n\nINPUT pin has already reached max\nnumber of connections (%d link(s)).\nDelete a link before using this pin.", endPin->maxNbConnectedLinks;
						ImGui::SetTooltip(msg.c_str(), startPin->maxNbConnectedLinks, endPin->maxNbConnectedLinks);
					}
					
					else
					{
						ImGui::SetTooltip(msg.c_str(), startPin->maxNbConnectedLinks);
					}
					ax::NodeEditor::Resume();
				}

				//If only input pin has too many connections
				else if (endPin->nbConnectedLinks >= endPin->maxNbConnectedLinks)
				{
					ax::NodeEditor::RejectNewItem(ImVec4(1.0f, 0.f, 0.f, 1.0f), 8.0f);
					ax::NodeEditor::Suspend();
					ImGui::SetTooltip("INPUT pin has already reached max\nnumber of connections (%d link(s)).\nDelete a link before using this pin.", endPin->maxNbConnectedLinks);
					ax::NodeEditor::Resume();
				}

				else
				{
					// ax::NodeEditor::AcceptNewItem() return true when user release mouse button.
					if (ax::NodeEditor::AcceptNewItem(ImVec4(0.0f, 1.f, 0.f, 1.0f), 8.0f))
					{
						// Since we accepted new link, lets add one to our list of links.
						LinkData& newLink = _links.emplace_back(LinkData(startPin, endPin));

						newLink.startPin->OnConnect(newLink.endPin);
					}
				}
			}
		}
	}
	ax::NodeEditor::EndCreate(); // Wraps up object creation action handling.
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::DynamicLinkDestruction(std::vector<LinkData>& _dynamicLinks)
{
	if (ax::NodeEditor::BeginDelete())
	{
		// There may be many links marked for deletion, let's loop over them.
		ax::NodeEditor::LinkId deletedLinkId;
		while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId))
		{
			//Search the Link data to erase.
			std::vector<Utility::MNBV::LinkData>::iterator link = ECellEngine::Data::BinaryOperation::LowerBound(_dynamicLinks.begin(), _dynamicLinks.end(), (std::size_t)deletedLinkId);

			if (link != _dynamicLinks.end() && link->id == deletedLinkId)
			{
				// If you agree that link can be deleted, accept deletion.
				if (ax::NodeEditor::AcceptDeletedItem())
				{
					Editor::Widget::MNBV::EraseDynamicLink(link);
				}
			}
		}
	}
	ax::NodeEditor::EndDelete(); // Wrap up deletion action
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::Link(LinkData& linkInfo)
{
	if (ax::NodeEditor::Link(linkInfo.id, linkInfo.startIds[0], linkInfo.endIds[0])) { return; }

	if (ax::NodeEditor::Link(linkInfo.id, linkInfo.startIds[0], linkInfo.endIds[1])) { return; }

	if (ax::NodeEditor::Link(linkInfo.id, linkInfo.startIds[1], linkInfo.endIds[0])) { return; }

	ax::NodeEditor::Link(linkInfo.id, linkInfo.startIds[1], linkInfo.endIds[1]);
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::StaticLinkDestruction(std::vector<LinkData>& _staticLinks)
{
	if (ax::NodeEditor::BeginDelete())
	{
		// There may be many links marked for deletion, let's loop over them.
		ax::NodeEditor::LinkId deletedLinkId;
		while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId))
		{
			//We do not destroy static links that are selected
			if (!ax::NodeEditor::IsLinkSelected(deletedLinkId))
			{
				ax::NodeEditor::RejectDeletedItem();
			}
			else
			{
				//Search the Link data to erase.
				std::vector<Utility::MNBV::LinkData>::iterator link = ECellEngine::Data::BinaryOperation::LowerBound(_staticLinks.begin(), _staticLinks.end(), (std::size_t)deletedLinkId);

				if (link != _staticLinks.end() && link->id == deletedLinkId)
				{
					// If you agree that link can be deleted, accept deletion.
					if (ax::NodeEditor::AcceptDeletedItem())
					{
						Editor::Widget::MNBV::EraseStaticLink(link);
					}
				}
			}
			
		}
	}
	ax::NodeEditor::EndDelete(); // Wrap up deletion action
}

#pragma endregion

#pragma region Custom Node Widgets

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeCheckBoxFlag(const char* _label, int* _flags, const int _flag, const char* _tooltip)
{
	ImGui::CheckboxFlags(_label, _flags, _flag);
	if (_tooltip)
	{
		if (ImGui::IsItemHovered())
		{
			ax::NodeEditor::Suspend();
			ImGui::SetTooltip(_tooltip);
			ax::NodeEditor::Resume();
		}
	}
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeAllImPlotAxisFlags(ImPlotFlags* _flags)
{
	NodeCheckBoxFlag("ImPlotAxisFlags_NoLabel", _flags, ImPlotAxisFlags_NoLabel, "The axis label will not be displayed (axis labels also hidden if the supplied string name is NULL).");
	NodeCheckBoxFlag("ImPlotAxisFlags_NoGridLines", _flags, ImPlotAxisFlags_NoGridLines, "No grid lines will be displayed.");
	NodeCheckBoxFlag("ImPlotAxisFlags_NoTickMarks", _flags, ImPlotAxisFlags_NoTickMarks, "No tick marks will be displayed.");
	NodeCheckBoxFlag("ImPlotAxisFlags_NoTickLabels", _flags, ImPlotAxisFlags_NoTickLabels, "No text labels will be displayed.");
	NodeCheckBoxFlag("ImPlotAxisFlags_NoInitialFit", _flags, ImPlotAxisFlags_NoInitialFit, "Axis will not be initially fit to data extents on the first rendered frame.");
	NodeCheckBoxFlag("ImPlotAxisFlags_NoMenus", _flags, ImPlotAxisFlags_NoMenus, "The user will not be able to open context menus with right-click.");
	NodeCheckBoxFlag("ImPlotAxisFlags_Opposite", _flags, ImPlotAxisFlags_Opposite, "Axis ticks and labels will be rendered on conventionally opposite side (i.e, right or top).");
	NodeCheckBoxFlag("ImPlotAxisFlags_Foreground", _flags, ImPlotAxisFlags_Foreground, "Grid lines will be displayed in the foreground (i.e. on top of data) in stead of the background.");
	NodeCheckBoxFlag("ImPlotAxisFlags_Invert", _flags, ImPlotAxisFlags_Invert, "The axis will be inverted.");
	NodeCheckBoxFlag("ImPlotAxisFlags_AutoFit", _flags, ImPlotAxisFlags_AutoFit, "Axis will be auto-fitting to data extents.");
	NodeCheckBoxFlag("ImPlotAxisFlags_RangeFit", _flags, ImPlotAxisFlags_RangeFit, "Axis will only fit points if the point is in the visible range of the **orthogonal** axis.");
	NodeCheckBoxFlag("ImPlotAxisFlags_LockMin", _flags, ImPlotAxisFlags_LockMin, "The axis minimum value will be locked when panning/zooming.");
	NodeCheckBoxFlag("ImPlotAxisFlags_LockMax", _flags, ImPlotAxisFlags_LockMax, "The axis maximum value will be locked when panning/zooming.");
	NodeCheckBoxFlag("ImPlotAxisFlags_Lock", _flags, ImPlotAxisFlags_Lock, "ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax,");
	NodeCheckBoxFlag("ImPlotAxisFlags_NoDecorations", _flags, ImPlotAxisFlags_NoDecorations, "ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels,");
	NodeCheckBoxFlag("ImPlotAxisFlags_AuxDefault", _flags, ImPlotAxisFlags_AuxDefault, "ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_Opposite");
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeAllImPlotFlags(ImPlotFlags* _flags)
{
	NodeCheckBoxFlag("ImPlotFlags_NoTitle", _flags, ImPlotFlags_NoTitle, "The plot title will not be displayed (titles are also hidden if preceeded by double hashes, e.g. \"##MyPlot\").");
	NodeCheckBoxFlag("ImPlotFlags_NoLegend", _flags, ImPlotFlags_NoLegend, "The legend will not be displayed.");
	NodeCheckBoxFlag("ImPlotFlags_NoMouseText", _flags, ImPlotFlags_NoMouseText, "The mouse position, in plot coordinates, will not be displayed inside of the plot.");
	NodeCheckBoxFlag("ImPlotFlags_NoInputs", _flags, ImPlotFlags_NoInputs, "The user will not be able to interact with the plot.");
	NodeCheckBoxFlag("ImPlotFlags_NoMenus", _flags, ImPlotFlags_NoMenus, "The user will not be able to open context menus.");
	NodeCheckBoxFlag("ImPlotFlags_NoBoxSelect", _flags, ImPlotFlags_NoBoxSelect, "The user will not be able to box-select.");
	NodeCheckBoxFlag("ImPlotFlags_NoChild", _flags, ImPlotFlags_NoChild, "A child window region will not be used to capture mouse scroll (can boost performance for single ImGui window applications).");
	NodeCheckBoxFlag("ImPlotFlags_NoFrame", _flags, ImPlotFlags_NoFrame, "The ImGui frame will not be rendered.");
	NodeCheckBoxFlag("ImPlotFlags_Equal", _flags, ImPlotFlags_Equal, "X and Y axes pairs will be constrained to have the same units/pixel.");
	NodeCheckBoxFlag("ImPlotFlags_Crosshairs", _flags, ImPlotFlags_Crosshairs, "The default mouse cursor will be replaced with a crosshair when hovered.");
	//NodeCheckBoxFlag("ImPlotFlags_AntiAliased", _flags, ImPlotFlags_AntiAliased, "Plot items will be software anti-aliased (not recommended for high density plots, prefer MSAA).");
	NodeCheckBoxFlag("ImPlotFlags_CanvasOnly", _flags, ImPlotFlags_CanvasOnly, "ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText");
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeAllImPlotAxisScale(ImPlotScale& _xAxisScale, ImPlotScale& _yAxisScale)
{
	/*if (ImGui::RadioButton("ImPlotScale_SymLog", _xAxisScale == ImPlotScale_SymLog && _yAxisScale == ImPlotScale_SymLog))
	{
		_xAxisScale = ImPlotScale_SymLog;
		_yAxisScale = ImPlotScale_SymLog;
	}
	if (ImGui::IsItemHovered())
	{
		ax::NodeEditor::Suspend();
		ImGui::SetTooltip("Symmetric log scale for the X and Y axis.");
		ax::NodeEditor::Resume();
	}*/

	if (ImGui::RadioButton("ImPlotScale_Linear for X", _xAxisScale == ImPlotScale_Linear))
	{
		_xAxisScale = ImPlotScale_Linear;
	}
	if (ImGui::IsItemHovered())
	{
		ax::NodeEditor::Suspend();
		ImGui::SetTooltip("Default linear scale.");
		ax::NodeEditor::Resume();
	}

	if (ImGui::RadioButton("ImPlotScale_Log10 for X", _xAxisScale == ImPlotScale_Log10))
	{
		_xAxisScale = ImPlotScale_Log10;
	}
	if(ImGui::IsItemHovered())
	{
		ax::NodeEditor::Suspend();
		ImGui::SetTooltip("Base 10 logartithmic scale.");
		ax::NodeEditor::Resume();
	}

	if (ImGui::RadioButton("ImPlotScale_Time for X", _xAxisScale == ImPlotScale_Time))
	{
		_xAxisScale = ImPlotScale_Time;
	}
	if (ImGui::IsItemHovered())
	{
		ax::NodeEditor::Suspend();
		ImGui::SetTooltip("Date/time scale.");
		ax::NodeEditor::Resume();
	}

	if (ImGui::RadioButton("ImPlotScale_Linear for Y", _yAxisScale == ImPlotScale_Linear))
	{
		_yAxisScale = ImPlotScale_Linear;
	}
	if (ImGui::IsItemHovered())
	{
		ax::NodeEditor::Suspend();
		ImGui::SetTooltip("Default linear scale.");
		ax::NodeEditor::Resume();
	}

	if (ImGui::RadioButton("ImPlotScale_Log10 for Y", _yAxisScale == ImPlotScale_Log10))
	{
		_yAxisScale = ImPlotScale_Log10;
	}
	if(ImGui::IsItemHovered())
	{
		ax::NodeEditor::Suspend();
		ImGui::SetTooltip("Base 10 logartithmic scale.");
		ax::NodeEditor::Resume();
	}    
	
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeCollapsingHeader(const char* _label, const std::size_t _id,
	unsigned short& _utilityState, const short _stateBitPos,
	const float _startX, const float _drawLength,
	const ImVec2& _size)
{
	AlignToCenter(_startX, _drawLength, _size.x);
	ImGui::PushID((int)_id);
	if (ImGui::Button(_label, _size))
	{
		_utilityState ^= 1 << _stateBitPos;
	}
	ImGui::PopID();

	return _utilityState >> _stateBitPos & 1;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeCollapsingHeader_In(const char* _label, const std::size_t _id,
	unsigned short& _utilityState, const short _stateBitPos,
	const float _startX, const float _drawLength,
	const NodePinData& _pin,
	const ImVec2& _size, const bool _hidePinsOnExpand)
{
	AlignToRight(_startX, _drawLength, _size.x);
	ImGui::PushID((int)_id);
	if (ImGui::Button(_label, _size))
	{
		_utilityState ^= 1 << _stateBitPos;
	}
	ImGui::PopID();

	bool open = _utilityState >> _stateBitPos & 1;

	if (!open || !_hidePinsOnExpand)
	{
		ImGui::SameLine(); ImGui::SetCursorPosX(_startX);
		Pin(_pin);
	}

	return open;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeCollapsingHeader_InOut(const char* _label, const std::size_t _id,
	unsigned short& _utilityState, const short _stateBitPos,
	const float _startX, const float _drawLength,
	const NodePinData& _inputPin, const NodePinData& _outputPin,
	const ImVec2& _size, const bool _hidePinsOnExpand)
{
	AlignToCenter(_startX, _drawLength, _size.x);
	ImGui::PushID((int)_id);
	if (ImGui::Button(_label, _size))
	{
		_utilityState ^= 1 << _stateBitPos;
	}
	ImGui::PopID();

	bool open = _utilityState >> _stateBitPos & 1;

	if (!open || !_hidePinsOnExpand)
	{
		ImGui::SameLine(); ImGui::SetCursorPosX(_startX);
		Pin(_inputPin);

		ImGui::SameLine(); AlignToRight(_startX, _drawLength, Style::EditorStyle::GetMNBVStyle().pinWidth);
		Pin(_outputPin);
	}

	return open;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeCollapsingHeader_Out(const char* _label, const std::size_t _id,
	unsigned short& _utilityState, const short _stateBitPos,
	const float _startX, const float _drawLength,
	const NodePinData& _pin,
	const ImVec2& _size, const bool _hidePinsOnExpand)
{
	AlignToCenter(_startX, _drawLength, _size.x);
	ImGui::PushID((int)_id);
	if (ImGui::Button(_label, _size))
	{
		_utilityState ^= 1 << _stateBitPos;
	}
	ImGui::PopID();

	bool open = _utilityState >> _stateBitPos & 1;

	if (!open || !_hidePinsOnExpand)
	{
		ImGui::SameLine();
		AlignToRight(_startX, _drawLength, Style::EditorStyle::GetMNBVStyle().pinWidth);
		Pin(_pin);
	}

	return open;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeComboBox(const char* _label, const char* _items[], const int _itemsCount, int& _currentItemIndex,
	const float _comboBoxWidth, const float _startX, const float _drawLength)
{
	ImGui::SetCursorPosX(_startX);

	AlignToCenter(_startX, _drawLength, _comboBoxWidth);
	ImGui::AlignTextToFramePadding();
	ImGui::Text(_label); ImGui::SameLine();

	const float buttonWidth = _comboBoxWidth - ImGui::CalcTextSize(_label).x - ImGui::GetStyle().ItemSpacing.x;
	
	bool open = ImGui::Button(_items[_currentItemIndex], ImVec2(buttonWidth, 0.f));

	ax::NodeEditor::Suspend();
	if (open)
	{
		ImGui::OpenPopup("##combo");
	}
	
	const char* previewValue = _items[_currentItemIndex];
	if (ImGui::BeginPopup("##combo", ImGuiWindowFlags_NoMove))
	{
		for (int i = 0; i < _itemsCount; ++i)
		{
			if (ImGui::MenuItem(_items[i]))
			{
				_currentItemIndex = i;
			}
		}
		ImGui::EndPopup();
	}
	ax::NodeEditor::Resume();

	return previewValue != _items[_currentItemIndex];
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeDragFloat_In(const char* _label, const std::size_t _id, float* valueBuffer,
	const float _inputFieldWidth, const float _startX,
	const NodePinData& _pin,
	const ImGuiSliderFlags _flags)
{
	ImGui::SetCursorPosX(_startX);

	Pin(_pin); ImGui::SameLine();

	ImGui::PushID((int)_id);
	ImGui::AlignTextToFramePadding();

	ImGui::Text(_label); ImGui::SameLine();

	ImGui::SetNextItemWidth(_inputFieldWidth - ImGui::CalcTextSize(_label).x - ImGui::GetStyle().ItemSpacing.x);
	bool edited = ImGui::DragFloat("##value", valueBuffer, 1.0f, 0.f, 0.f, "%.3f", _flags);
	ImGui::PopID();

	return edited;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeDragFloat_Out(const char* _label, const std::size_t _id, float* valueBuffer,
	const float _inputFieldWidth, const float _startX, const float _drawLength,
	const NodePinData& _pin,
	const ImGuiSliderFlags _flags)
{
	ImGui::SetCursorPosX(_startX);

	ImGui::PushID((int)_id);
	ImGui::AlignTextToFramePadding();
	AlignToRight(_startX, _drawLength - Style::EditorStyle::GetMNBVStyle().pinWidth - ImGui::GetStyle().ItemSpacing.x - (_inputFieldWidth - ImGui::CalcTextSize(_label).x), ImGui::CalcTextSize(_label).x);

	ImGui::Text(_label); ImGui::SameLine();

	ImGui::SetNextItemWidth(_inputFieldWidth - ImGui::CalcTextSize(_label).x - ImGui::GetStyle().ItemSpacing.x);
	bool edited = ImGui::DragFloat("##value", valueBuffer, 1.0f, 0.f, 0.f, "%.3f", _flags);
	ImGui::PopID();

	ImGui::SameLine();

	Pin(_pin);

	return edited;
}

float ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeHeader(const char* _type, const char* _name,
	const ImVec4 _colorSet[], const float _width, const short _height, const short _nbPins)
{
	const float titleSize = ImGui::CalcTextSize(_type).x + ImGui::CalcTextSize(_name).x + ImGui::GetStyle().ItemSpacing.x;

	const ImVec2 startPos = ImGui::GetCursorScreenPos();
	const ImVec2 endPos = ImVec2(
		startPos.x + std::max(_width + _nbPins * GetPinDrawOffset(), titleSize + _nbPins * ImGui::GetStyle().FramePadding.x),
		startPos.y + ImGui::GetTextLineHeightWithSpacing()
	);

	const ImRect bb(startPos, endPos);
	ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, ImColor(_colorSet[Style::NodeColorType_HeaderBg]), ax::NodeEditor::GetStyle().NodeRounding);
	ImGui::SetCursorPos(ImVec2(startPos.x + ImGui::GetStyle().FramePadding.x, startPos.y));
	ImGui::Text(_type); ImGui::SameLine(); ImGui::Text(_name);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);

	NodeHorizontalSeparator(bb.GetWidth());

	return bb.GetWidth();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeHorizontalSeparator(const float _width, const float _thickness)
{
	//ItemAdd and ImRect are part of imgui_internal.hpp
	const ImVec2 startPos = ImGui::GetCursorPos();
	const ImRect bb(startPos.x, startPos.y, startPos.x + _width, startPos.y);

	ImGui::ItemSize(ImVec2(0.0f, _thickness));
	ImGui::GetWindowDrawList()->AddLine(bb.Min, bb.Max, ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Separator)), _thickness);
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeInputFloat_InOut(const char* _label, const std::size_t _id, float* valueBuffer,
	const float _inputFieldWidth, const float _startX, const float _drawLength,
	const NodePinData& _inputPin, const NodePinData& _outputPin,
	const ImGuiInputTextFlags _flags)
{
	ImGui::SetCursorPosX(_startX);

	Pin(_inputPin); ImGui::SameLine();

	ImGui::PushID((int)_id);
	ImGui::AlignTextToFramePadding(); AlignToCenter(_startX, _drawLength, _inputFieldWidth);

	ImGui::Text(_label); ImGui::SameLine();
	ImGui::SetNextItemWidth(_inputFieldWidth - ImGui::CalcTextSize(_label).x - ImGui::GetStyle().ItemSpacing.x);
	bool edited = ImGui::InputFloat("##quantity", valueBuffer, 0.f, 0.f, "%.6f", _flags);
	if (ImGui::IsItemActive() && (_flags & ImGuiInputTextFlags_EnterReturnsTrue))
	{
		ax::NodeEditor::Suspend();
		ImGui::SetTooltip("Press ENTER to confirm.");
		ax::NodeEditor::Resume();
	}
	ImGui::PopID();

	ImGui::SameLine();

	AlignToRight(_startX, _drawLength, Style::EditorStyle::GetMNBVStyle().pinWidth);
	Pin(_outputPin);

	return edited;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeInputFloat_Out(const char* _label, const std::size_t _id, float* valueBuffer,
	const float _inputFieldWidth, const float _startX, const float _drawLength,
	const NodePinData& _pin,
	const ImGuiInputTextFlags _flags)
{
	ImGui::SetCursorPosX(_startX);

	ImGui::PushID((int)_id);
	ImGui::AlignTextToFramePadding();
	AlignToRight(_startX, _drawLength - Style::EditorStyle::GetMNBVStyle().pinWidth - ImGui::GetStyle().ItemSpacing.x - (_inputFieldWidth - ImGui::CalcTextSize(_label).x), ImGui::CalcTextSize(_label).x);

	ImGui::Text(_label); ImGui::SameLine();

	ImGui::SetNextItemWidth(_inputFieldWidth - ImGui::CalcTextSize(_label).x - ImGui::GetStyle().ItemSpacing.x);
	bool edited = ImGui::InputFloat("##quantity", valueBuffer, 0.f, 0.f, "%.3f", _flags);
	if (ImGui::IsItemActive() && !(_flags & ImGuiInputTextFlags_ReadOnly) && (_flags & ImGuiInputTextFlags_EnterReturnsTrue))
	{
		ax::NodeEditor::Suspend();
		ImGui::SetTooltip("Press ENTER to confirm.");
		ax::NodeEditor::Resume();
	}
	ImGui::PopID();

	ImGui::SameLine();

	Pin(_pin);

	return edited;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeInputText(const char* _label, char* _target, char* _buffer,
	const std::size_t _bufferSize, const float _inputFieldWidth, const float _startX, const float _drawLength,
	const ImGuiInputTextFlags _flags)
{
	std::size_t targetLen = std::strlen(_target);
	std::memcpy(_buffer, _target, targetLen);
	if (targetLen < _bufferSize)
	{
		_buffer[targetLen] = '\0';
	}
	AlignToCenter(_startX, _drawLength, _inputFieldWidth);
	ImGui::SetNextItemWidth(_inputFieldWidth - ImGui::CalcTextSize(_label).x - ImGui::GetStyle().ItemSpacing.x);
	if (ImGui::InputText(_label, _buffer, _bufferSize, _flags))
	{
		//std::memset(_target, '\0', std::max(targetLen, std::strlen(_buffer)));
		std::size_t bufferLen = std::strlen(_buffer);
		std::memcpy(_target, _buffer, bufferLen);
		if (bufferLen < _bufferSize)
		{
			_target[bufferLen] = '\0';
		}

		return true;
	}
	if (ImGui::IsItemActive() && (_flags & ImGuiInputTextFlags_EnterReturnsTrue))
	{
		ax::NodeEditor::Suspend();
		ImGui::SetTooltip("Press ENTER to confirm.");
		ax::NodeEditor::Resume();
	}

	return false;
}

template<typename DataType>
void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeStringListBox(NodeListBoxStringData<DataType>& _nlbsData,
	const float _startX, const float _drawLength,
	const float _widgetWidth, const unsigned short _itemViewHeight)
{
	ImGuiStyle& style = ImGui::GetStyle();
	AlignToCenter(_startX, _drawLength, _widgetWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));

	ImGui::BeginGroup();
	const unsigned short nbItems = (const unsigned short)_nlbsData.data->size();
	const unsigned short actualViewHeight = std::min(_itemViewHeight, nbItems);
	const bool hasScrollBar = nbItems > _itemViewHeight;

	const ImVec2 startPos = ImGui::GetCursorPos();

	//Delimiter
	const ImVec4 NodeFramePadding = ImVec4(4, 4, 2, 2);
	ImRect bb(
		startPos.x,
		startPos.y,
		startPos.x + _widgetWidth,
		startPos.y + actualViewHeight * ImGui::GetTextLineHeightWithSpacing() + NodeFramePadding.z + NodeFramePadding.w);//includes top and bottom padding
	ImGui::GetWindowDrawList()->AddRect(bb.Min, bb.Max, ImColor(ImGui::GetStyleColorVec4(ImGuiCol_TableBorderStrong)));

	//The list of items to display
	ImGui::BeginGroup();
	const float itemWidth = _widgetWidth - hasScrollBar * style.ScrollbarSize - NodeFramePadding.x - NodeFramePadding.y;//includes left and right
	const char* itemString;
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + NodeFramePadding.z);//Top Padding
	for (std::size_t n = _nlbsData.cursor; n > _nlbsData.cursor - actualViewHeight; n--)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + NodeFramePadding.x);//Left Padding

		itemString = _nlbsData.At(nbItems - n);
		if (ImGui::Selectable(itemString, false, ImGuiSelectableFlags_None, ImVec2(itemWidth, 0)))
		{
			_nlbsData.selectedItem = nbItems - n;
			_nlbsData.SetItemClicked();
		}

		if (ImGui::IsItemHovered())
		{
			_nlbsData.hoveredItem = nbItems - n;
			_nlbsData.SetItemHovered();

			if (ImGui::CalcTextSize(itemString).x > itemWidth)
			{
				ax::NodeEditor::Suspend();
				ImGui::SetTooltip("%s", itemString);
				ax::NodeEditor::Resume();
			}

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				_nlbsData.doubleClickedItem = nbItems - n;
				_nlbsData.SetItemDoubleClicked();
			}
		}
	}
	ImGui::EndGroup();

	ImGui::SameLine();

	//The vertical slider to control the list of items
	//We draw it only if there are more items than the _itemViewHeight
	if (hasScrollBar)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + NodeFramePadding.z);//TopPadding

		//No need to do the right padding because it's included in the
		//reduced size of the selectable items in the list box.

		ImGui::PushID((int)_nlbsData.scrollBarID);
		Style::EditorStyle::PushScrollBarStyle(style);
		ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
		ImGui::VSliderInt("##VertSliderInt", ImVec2(style.ScrollbarSize, actualViewHeight * ImGui::GetTextLineHeightWithSpacing()),
			(int*)&_nlbsData.cursor, actualViewHeight, nbItems, "", ImGuiSliderFlags_None);
		if (ImGui::IsItemHovered())
		{
			_nlbsData.SetScrollBarHovered();
		}
		if (ImGui::IsItemActive())
		{
			_nlbsData.SetScrollBarActivated();
		}
		ImGui::PopStyleVar(); //poping ImGuiStyleVar_GrabMinSize
		Style::EditorStyle::PopScrollBarStyle(); //poping after Style::EditorStyle::PushScrollBarStyle(style);
		ImGui::PopID();
	}

	ImGui::PopStyleVar(); //poping ImGuiStyleVar_ItemSpacing
	ImGui::EndGroup();

	if (_nlbsData.IsAnItemHovered() || _nlbsData.IsScrollBarHovered())
	{
		ax::NodeEditor::SuspendNavigation();
		if (ImGui::GetIO().MouseWheel != 0)
		{
			_nlbsData.cursor += ImGui::GetIO().MouseWheel;
			if (_nlbsData.cursor < actualViewHeight)
			{
				_nlbsData.cursor = actualViewHeight;
			}

			if (nbItems < _nlbsData.cursor)
			{
				_nlbsData.cursor = nbItems;
			}
		}
		ax::NodeEditor::ResumeNavigation();
	}
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeText_In(const char* _label, const float _startX,
	const NodePinData& _pin)
{
	ImGui::SetCursorPosX(_startX);
	Pin(_pin);

	ImGui::SameLine();

	ImGui::AlignTextToFramePadding();
	ImGui::Text(_label);
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeText_InOut(const char* _label, const float _labelWidth,
	const float _startX, const float _drawLength,
	const NodePinData& _inputPin, const NodePinData& _outputPin)
{
	ImGui::SetCursorPosX(_startX);
	Pin(_inputPin); ImGui::SameLine();

	ImGui::AlignTextToFramePadding();
	AlignToCenter(_startX, _drawLength, _labelWidth);
	ImGui::Text(_label);

	ImGui::SameLine();

	AlignToRight(_startX, _drawLength, Style::EditorStyle::GetMNBVStyle().pinWidth);
	Pin(_outputPin);
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeText_Out(const char* _label, const float _labelWidth,
	const float _startX, const float _drawLength, const NodePinData& _pin)
{
	AlignToRight(_startX, _drawLength - Style::EditorStyle::GetMNBVStyle().pinWidth - ImGui::GetStyle().ItemSpacing.x, _labelWidth);
	ImGui::AlignTextToFramePadding();
	ImGui::Text(_label);

	ImGui::SameLine();

	Pin(_pin);
}

#pragma endregion