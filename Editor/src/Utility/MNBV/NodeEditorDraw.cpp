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

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::AssetNode(const char* _name, AssetNodeData& _assetNodeInfo)
{
    PushNodeStyle(Widget::MNBV::GetNodeColors(NodeType_Asset));
    ax::NodeEditor::BeginNode(_assetNodeInfo.id);

    const float headerWidth = NodeHeader("Asset:", _name, Widget::MNBV::GetNodeColors(NodeType_Asset));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    // ----- Pin and Text to connect the solver to the asset -----
    Pin(_assetNodeInfo.inputPins[AssetNodeData::InputPin_Solver], Widget::MNBV::GetPinColors(PinType_Solver));
    ImGui::SameLine(); ImGui::AlignTextToFramePadding(); ImGui::Text("Solver");

    // ----- String List Box and Pin to access the species of the asset -----
    if (NodeCollapsingHeader_Out("Species", _assetNodeInfo.collapsingHeadersIds[AssetNodeData::CollapsingHeader_Species],
        _assetNodeInfo.utilityState, AssetNodeData::State_CollHdrSpecies,
        startX, headerWidth,
        _assetNodeInfo.outputPins[AssetNodeData::OutputPin_CollHdrSpecies], Widget::MNBV::GetPinColors(PinType_Species),
        ImVec2(itemsWidth, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.nlbsData[AssetNodeData::NodeListBoxString_Species], startX, headerWidth, itemsWidth);
    }

    // ----- String List Box and Pin to access the parameters of the asset -----
    if (NodeCollapsingHeader_Out("Parameters", _assetNodeInfo.collapsingHeadersIds[AssetNodeData::CollapsingHeader_Parameters],
        _assetNodeInfo.utilityState, AssetNodeData::State_CollHdrParameters,
        startX, headerWidth,
        _assetNodeInfo.outputPins[AssetNodeData::OutputPin_CollHdrParameters], Widget::MNBV::GetPinColors(PinType_Parameter),
        ImVec2(itemsWidth, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.nlbsData[AssetNodeData::NodeListBoxString_Parameters], startX, headerWidth, itemsWidth);
    }

    // ----- String List Box and Pin to access the equations of the asset -----
    if (NodeCollapsingHeader_Out("Equations", _assetNodeInfo.collapsingHeadersIds[AssetNodeData::CollapsingHeader_Equations],
        _assetNodeInfo.utilityState, AssetNodeData::State_CollHdrEquations,
        startX, headerWidth,
        _assetNodeInfo.outputPins[AssetNodeData::OutputPin_CollHdrEquations], Widget::MNBV::GetPinColors(PinType_Parameter),
        ImVec2(itemsWidth, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.nlbsData[AssetNodeData::NodeListBoxString_Equations], startX, headerWidth, itemsWidth);
    }

    // ----- String List Box and Pin to access the reactions of the asset -----
    if (NodeCollapsingHeader_Out("Reactions", _assetNodeInfo.collapsingHeadersIds[AssetNodeData::CollapsingHeader_Reactions],
        _assetNodeInfo.utilityState, AssetNodeData::State_CollHdrReactions,
        startX, headerWidth,
        _assetNodeInfo.outputPins[AssetNodeData::OutputPin_CollHdrReactions], Widget::MNBV::GetPinColors(PinType_Reaction),
        ImVec2(itemsWidth, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.nlbsData[AssetNodeData::NodeListBoxString_Reactions], startX, headerWidth, itemsWidth);
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();

}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::EquationNode(const char* _name, EquationNodeData& _equationNodeInfo)
{
    PushNodeStyle(Widget::MNBV::GetNodeColors(NodeType_Equation));
    ax::NodeEditor::BeginNode(_equationNodeInfo.id);

    _equationNodeInfo.Update();

    const float headerWidth = NodeHeader("Equation:", _name, Widget::MNBV::GetNodeColors(NodeType_Equation));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    if (NodeCollapsingHeader_InOut("Model Links", _equationNodeInfo.collapsingHeadersIds[EquationNodeData::CollapsingHeader_ModelLinks],
        _equationNodeInfo.utilityState, EquationNodeData::State_CollHdrModelLinks,
        startX, headerWidth,
        _equationNodeInfo.inputPins[EquationNodeData::InputPin_CollHdrModelLinks], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_CollHdrModelLinks], Widget::MNBV::GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0.f)))
    {

        NodeText_In("Asset", startX, _equationNodeInfo.inputPins[EquationNodeData::InputPin_Asset], Widget::MNBV::GetPinColors(PinType_Asset));

        if (_equationNodeInfo.equationDep.size())
        {
            if (NodeCollapsingHeader_InOut("In Equations", _equationNodeInfo.collapsingHeadersIds[EquationNodeData::CollapsingHeader_Equations],
                _equationNodeInfo.utilityState, EquationNodeData::State_CollHdrEquations,
                startX, headerWidth,
                _equationNodeInfo.inputPins[EquationNodeData::InputPin_CollHdrEquations], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_CollHdrEquations], Widget::MNBV::GetPinColors(PinType_Equation),
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
                _equationNodeInfo.inputPins[EquationNodeData::InputPin_CollHdrKineticLaws], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_CollHdrKineticLaws], Widget::MNBV::GetPinColors(PinType_Reaction),
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
        _equationNodeInfo.inputPins[EquationNodeData::InputPin_CollHdrDataFields], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_CollHdrDataFields], Widget::MNBV::GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0)))
    {

        if (NodeCollapsingHeader_InOut("Operands", _equationNodeInfo.collapsingHeadersIds[EquationNodeData::CollapsingHeader_EquationOperands],
            _equationNodeInfo.utilityState, EquationNodeData::State_CollHdrEquationOperands,
            startX, headerWidth,
            _equationNodeInfo.inputPins[EquationNodeData::InputPin_CollHdrEquationOperands], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_CollHdrEquationOperands], Widget::MNBV::GetPinColors(PinType_Default),
            ImVec2(itemsWidth, 0.f)))
        {
            if (_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_SpeciesOperands].data->size())
            {
                const float spTextWidth = ImGui::CalcTextSize("Species").x;
                NodeText_InOut("Species", spTextWidth, startX, headerWidth,
                    _equationNodeInfo.inputPins[EquationNodeData::InputPin_NLBSSpecies], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_NLBSSpecies], Widget::MNBV::GetPinColors(PinType_Species));
                NodeStringListBox(_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_SpeciesOperands], startX, headerWidth, itemsWidth);
            }

            if (_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_ParameterOperands].data->size())
            {
                const float sparamTextWidth = ImGui::CalcTextSize("Parameters").x;
                NodeText_InOut("Parameters", sparamTextWidth, startX, headerWidth,
                    _equationNodeInfo.inputPins[EquationNodeData::InputPin_NLBSParameters], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_NLBSParameters], Widget::MNBV::GetPinColors(PinType_Parameter));
                NodeStringListBox(_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_ParameterOperands], startX, headerWidth, itemsWidth);
            }

            if (_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_EquationOperands].data->size())
            {
                const float cparaTextWidth = ImGui::CalcTextSize("Equations").x;
                NodeText_InOut("Equations", cparaTextWidth, startX, headerWidth,
                    _equationNodeInfo.inputPins[EquationNodeData::InputPin_NLBSEquations], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_NLBSEquations], Widget::MNBV::GetPinColors(PinType_Equation));
                NodeStringListBox(_equationNodeInfo.nlbsData[EquationNodeData::NodeListBoxString_EquationOperands], startX, headerWidth, itemsWidth);
            }
        }

        //const static float inputFieldWidth = headerWidth -  + ImGui::GetStyle().ItemSpacing.x; //100.0f is the default length for a field
        float value = _equationNodeInfo.data->Get();
        NodeInputFloat_InOut("Value", _equationNodeInfo.id.Get(), &value,
            itemsWidth, startX, headerWidth,
            _equationNodeInfo.inputPins[EquationNodeData::InputPin_EquationValue], _equationNodeInfo.outputPins[EquationNodeData::OutputPin_EquationValue], Widget::MNBV::GetPinColors(PinType_ValueFloat),
            ImGuiInputTextFlags_ReadOnly);
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::LinePlotNode(const char* _name, LinePlotNodeData& _linePlotNodeData)
{
    PushNodeStyle(Widget::MNBV::GetNodeColors(NodeType_Default));
    ax::NodeEditor::BeginNode(_linePlotNodeData.id);

    const float headerWidth = NodeHeader("Plot:", _name, Widget::MNBV::GetNodeColors(NodeType_Reaction), 300.f, 1, 1);
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
            NodeInputText("Plot Title", _linePlotNodeData.plotTitle, buffer, 64, headerWidth, startX, headerWidth);

            NodeInputText("X Axis Label", _linePlotNodeData.xAxisLabel, buffer, 64, headerWidth, startX, headerWidth);

            NodeInputText("Y Axis Label", _linePlotNodeData.yAxisLabel, buffer, 64, headerWidth, startX, headerWidth);

            NodeInputText("Line Legend", _linePlotNodeData.lineLegend, buffer, 64, headerWidth, startX, headerWidth);

            ImGui::SetNextItemWidth(headerWidth - ImGui::CalcTextSize("Plot Size").x - style.ItemSpacing.x);
            ImGui::DragFloat2("Plot Size", _linePlotNodeData.plotSize, 1.f, 10.f, 1000.f);
        }

        if (NodeCollapsingHeader("Plot Flags", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_PlotFlags],
            _linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrPlotFlags,
            startX, headerWidth, ImVec2(headerWidth, 0.f)))
        {
            NodeAllImPlotFlags(&_linePlotNodeData.plotFlags);
        }

        if (NodeCollapsingHeader("X Axis Flags", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_XAxisFlags],
            _linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrXAxisFlags,
            startX, headerWidth, ImVec2(headerWidth, 0.f)))
        {
            NodeAllImPlotAxisFlags(&_linePlotNodeData.xAxisFlags);
        }

        if (NodeCollapsingHeader("Y Axis Flags", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_YAxisFlags],
            _linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrYAxisFlags,
            startX, headerWidth, ImVec2(headerWidth, 0.f)))
        {
            NodeAllImPlotAxisFlags(&_linePlotNodeData.yAxisFlags);
        }
        
        if (NodeCollapsingHeader("Axis Scale Flags", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_AxisScaleFlags],
            _linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrAxisScaleFlags,
            startX, headerWidth, ImVec2(headerWidth, 0.f)))
        {
            NodeAllImPlotAxisScale(_linePlotNodeData.xAxisScale, _linePlotNodeData.yAxisScale);
        }
    }

    NodeHorizontalSeparator(headerWidth);

    if (NodeCollapsingHeader_In("Plot", _linePlotNodeData.collapsingHeadersIds[LinePlotNodeData::CollapsingHeader_Plot],
        _linePlotNodeData.utilityState, LinePlotNodeData::State_CollHdrPlot,
        startX, headerWidth,
        _linePlotNodeData.inputPins[LinePlotNodeData::InputPin_CollHdrPlot], Widget::MNBV::GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0.f)))
    {
        NodeText_In(_linePlotNodeData.xAxisLabel, startX, _linePlotNodeData.inputPins[LinePlotNodeData::InputPin_XAxis], Widget::MNBV::GetPinColors(PinType_ValueFloat));
        NodeText_In(_linePlotNodeData.yAxisLabel, startX, _linePlotNodeData.inputPins[LinePlotNodeData::InputPin_YAxis], Widget::MNBV::GetPinColors(PinType_ValueFloat));

        AlignToCenter(startX, headerWidth, headerWidth);

        if (_linePlotNodeData.IsPlotOpen())
        {
            if (ImGui::Button("Show", ImVec2(0.5f * (headerWidth - style.ItemSpacing.x), 0.f)))
            {
                //ImGuiWindow* window = ImGui::FindWindowByName(_name);
                ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName(_name));
                ImGui::SetNextWindowPos(ax::NodeEditor::CanvasToScreen(ImGui::GetCursorPos()));
            }
            ImGui::SameLine();
            if (ImGui::Button("Close", ImVec2(0.5f * (headerWidth - style.ItemSpacing.x), 0.f)))
            {
                _linePlotNodeData.SwitchState(LinePlotNodeData::State_PlotIsOpen);
            }
        }
        else
        {
            if (ImGui::Button("Open", ImVec2(headerWidth, 0.f)))
            {
                _linePlotNodeData.SwitchState(LinePlotNodeData::State_PlotIsOpen);
                ImGui::SetNextWindowPos(ax::NodeEditor::CanvasToScreen(ImGui::GetCursorPos()));
            }
        }
    }

    if (_linePlotNodeData.IsPlotOpen())
    {
        ImGuiStyle& imguiStyle = ImGui::GetStyle();
        ImPlotStyle& implotStyle = ImPlot::GetStyle();
        ImGui::SetNextWindowSize(ImVec2(_linePlotNodeData.plotSize[0] + 2 * imguiStyle.WindowPadding.x,
            _linePlotNodeData.plotSize[1] + 2 * imguiStyle.WindowPadding.y));
        if (ImGui::Begin(_name, NULL, _linePlotNodeData.plotWindowFlags))
        {
            if (ImPlot::BeginPlot(_linePlotNodeData.plotTitle, ImVec2(_linePlotNodeData.plotSize[0], _linePlotNodeData.plotSize[1]), _linePlotNodeData.plotFlags))
            {
                ImPlot::SetupAxes(_linePlotNodeData.xAxisLabel, _linePlotNodeData.yAxisLabel, _linePlotNodeData.xAxisFlags, _linePlotNodeData.yAxisFlags);
                ImPlot::SetupAxisScale(ImAxis_X1, _linePlotNodeData.xAxisScale);
                ImPlot::SetupAxisScale(ImAxis_Y1, _linePlotNodeData.yAxisScale);
                
                _linePlotNodeData.Update();

                ImPlot::PlotLine(_linePlotNodeData.lineLegend,
                    &_linePlotNodeData.dataPoints.Data[0].x, &_linePlotNodeData.dataPoints.Data[0].y,
                    _linePlotNodeData.dataPoints.Data.Size, _linePlotNodeData.dataPoints.Offset, 2 * sizeof(float));

                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::ReactionNode(const char* _name, ReactionNodeData& _reactionNodeInfo)
{
    PushNodeStyle(Widget::MNBV::GetNodeColors(NodeType_Reaction));
    ax::NodeEditor::BeginNode(_reactionNodeInfo.id);

    _reactionNodeInfo.Update();

    const float headerWidth = NodeHeader("Reaction:", _name, Widget::MNBV::GetNodeColors(NodeType_Reaction));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    if (NodeCollapsingHeader_InOut("Model Links", _reactionNodeInfo.collapsingHeadersIds[ReactionNodeData::CollapsingHeader_ModelLinks],
        _reactionNodeInfo.utilityState, ReactionNodeData::State_CollHdrModelLinks,
        startX, headerWidth,
        _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_CollHdrModelLinks], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_CollHdrModelLinks], Widget::MNBV::GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0.f)))
    {

        NodeText_In("Asset", startX, _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_Asset], Widget::MNBV::GetPinColors(PinType_Asset));

        if (NodeCollapsingHeader_InOut("Reactants", _reactionNodeInfo.collapsingHeadersIds[ReactionNodeData::CollapsingHeader_Reactants],
            _reactionNodeInfo.utilityState, ReactionNodeData::State_CollHdrReactants,
            startX, headerWidth,
            _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_CollHdrReactants], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_CollHdrReactants], Widget::MNBV::GetPinColors(PinType_Species),
            ImVec2(itemsWidth, 0.f), false))
        {
            NodeStringListBox(_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_Reactants], startX, headerWidth, itemsWidth);
        }

        if (NodeCollapsingHeader_InOut("Products", _reactionNodeInfo.collapsingHeadersIds[ReactionNodeData::CollapsingHeader_Products],
            _reactionNodeInfo.utilityState, ReactionNodeData::State_CollHdrProducts,
            startX, headerWidth,
            _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_CollHdrProducts], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_CollHdrProducts], Widget::MNBV::GetPinColors(PinType_Species),
            ImVec2(itemsWidth, 0.f), false))
        {
            NodeStringListBox(_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_Products], startX, headerWidth, itemsWidth);
        }
    }

    NodeHorizontalSeparator(headerWidth);

    if (NodeCollapsingHeader_InOut("Kinetic Law", _reactionNodeInfo.collapsingHeadersIds[ReactionNodeData::CollapsingHeader_KineticLaw],
        _reactionNodeInfo.utilityState, ReactionNodeData::State_CollHdrKineticLaw,
        startX, headerWidth,
        _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_CollHdrKineticLaw], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_CollHdrKineticLaw], Widget::MNBV::GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0)))
    {

        if (NodeCollapsingHeader_InOut("Operands", _reactionNodeInfo.collapsingHeadersIds[ReactionNodeData::CollapsingHeader_KineticLawOperands],
            _reactionNodeInfo.utilityState, ReactionNodeData::State_CollHdrKineticLawOperands,
            startX, headerWidth,
            _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_CollHdrKineticLawOperands], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_CollHdrKineticLawOperands], Widget::MNBV::GetPinColors(PinType_Default),
            ImVec2(itemsWidth, 0.f)))
        {
            if (_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_SpeciesOperands].data->size())
            {
                const float spTextWidth = ImGui::CalcTextSize("Species").x;
                NodeText_InOut("Species", spTextWidth, startX, headerWidth,
                    _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_NLBSSpecies], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_NLBSSpecies], Widget::MNBV::GetPinColors(PinType_Species));
                NodeStringListBox(_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_SpeciesOperands], startX, headerWidth, itemsWidth);
            }

            if (_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_ParameterOperands].data->size())
            {
                const float sparamTextWidth = ImGui::CalcTextSize("Parameters").x;
                NodeText_InOut("Parameters", sparamTextWidth, startX, headerWidth,
                    _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_NLBSParameters], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_NLBSParameters], Widget::MNBV::GetPinColors(PinType_Parameter));
                NodeStringListBox(_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_ParameterOperands], startX, headerWidth, itemsWidth);
            }

            if (_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_EquationOperands].data->size())
            {
                const float cparaTextWidth = ImGui::CalcTextSize("Equations").x;
                NodeText_InOut("Equations", cparaTextWidth, startX, headerWidth,
                    _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_NLBSEquations], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_NLBSEquations], Widget::MNBV::GetPinColors(PinType_Equation));
                NodeStringListBox(_reactionNodeInfo.nlbsData[ReactionNodeData::NodeListBoxString_EquationOperands], startX, headerWidth, itemsWidth);
            }
        }

        float value = _reactionNodeInfo.data->GetKineticLawValue();
        NodeInputFloat_InOut("Value", _reactionNodeInfo.id.Get(), &value,
            itemsWidth, startX, headerWidth,
            _reactionNodeInfo.inputPins[ReactionNodeData::InputPin_KineticLawValue], _reactionNodeInfo.outputPins[ReactionNodeData::OutputPin_KineticLawValue], Widget::MNBV::GetPinColors(PinType_ValueFloat),
            ImGuiInputTextFlags_ReadOnly);
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::ParameterNode(const char* _name, ParameterNodeData& _parameterNodeInfo)
{
    PushNodeStyle(Widget::MNBV::GetNodeColors(NodeType_Parameter));
    ax::NodeEditor::BeginNode(_parameterNodeInfo.id);

    _parameterNodeInfo.Update();

    const float headerWidth = NodeHeader("Parameter:", _name, Widget::MNBV::GetNodeColors(NodeType_Parameter));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    if (NodeCollapsingHeader_InOut("Model Links", _parameterNodeInfo.collapsingHeadersIds[ParameterNodeData::CollapsingHeader_ModelLinks],
        _parameterNodeInfo.utilityState, ParameterNodeData::State_CollHdrModelLinks,
        startX, headerWidth,
        _parameterNodeInfo.inputPins[ParameterNodeData::InputPin_CollHdrModelLinks], _parameterNodeInfo.outputPins[ParameterNodeData::OutputPin_CollHdrModelLinks], Widget::MNBV::GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0.f)))
    {

        NodeText_In("Asset", startX, _parameterNodeInfo.inputPins[ParameterNodeData::InputPin_Asset], Widget::MNBV::GetPinColors(PinType_Asset));

        if (_parameterNodeInfo.equationDep.size())
        {
            if (NodeCollapsingHeader_InOut("In Equations", _parameterNodeInfo.collapsingHeadersIds[ParameterNodeData::CollapsingHeader_Equations],
                _parameterNodeInfo.utilityState, ParameterNodeData::State_CollHdrEquations,
                startX, headerWidth,
                _parameterNodeInfo.inputPins[ParameterNodeData::InputPin_CollHdrEquations], _parameterNodeInfo.outputPins[ParameterNodeData::OutputPin_CollHdrEquations], Widget::MNBV::GetPinColors(PinType_Parameter),
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
                _parameterNodeInfo.inputPins[ParameterNodeData::InputPin_CollHdrKineticLaws], _parameterNodeInfo.outputPins[ParameterNodeData::OutputPin_CollHdrKineticLaws], Widget::MNBV::GetPinColors(PinType_Parameter),
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
        _parameterNodeInfo.inputPins[ParameterNodeData::InputPin_CollHdrDataFields], _parameterNodeInfo.outputPins[ParameterNodeData::OutputPin_CollHdrDataFields], Widget::MNBV::GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0)))
    {
        if (NodeInputFloat_InOut("Value", _parameterNodeInfo.id.Get(), &_parameterNodeInfo.parameterValueBuffer,
            itemsWidth, startX, headerWidth,
            _parameterNodeInfo.inputPins[ParameterNodeData::InputPin_ParameterValue], _parameterNodeInfo.outputPins[ParameterNodeData::OutputPin_ParameterValue], Widget::MNBV::GetPinColors(PinType_ValueFloat),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            _parameterNodeInfo.data->Set(_parameterNodeInfo.parameterValueBuffer);
        }
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::SimulationTimeNode(const char* _name, SimulationTimeNodeData& _simulationTimeNodeInfo)
{
    PushNodeStyle(Widget::MNBV::GetNodeColors(NodeType_Data));
    ax::NodeEditor::BeginNode(_simulationTimeNodeInfo.id);

    const float headerWidth = NodeHeader("Time:", _name, Widget::MNBV::GetNodeColors(NodeType_Data));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    NodeInputFloat_Out("Elapsed Time", _simulationTimeNodeInfo.id.Get(), &_simulationTimeNodeInfo.simulationTimer->elapsedTime,
        itemsWidth, startX, headerWidth,
        _simulationTimeNodeInfo.outputPins[SimulationTimeNodeData::OutputPin_SimulationTime], Widget::MNBV::GetPinColors(PinType_ValueFloat),
        ImGuiInputTextFlags_ReadOnly);

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::SolverNode(const char* _name, const SolverNodeData& _solverNodeInfo)
{
    PushNodeStyle(Widget::MNBV::GetNodeColors(NodeType_Solver));
    ax::NodeEditor::BeginNode(_solverNodeInfo.id);

    const float headerWidth = NodeHeader("Solver:", _name, Widget::MNBV::GetNodeColors(NodeType_Solver));
    //const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float labelWidth = ImGui::CalcTextSize("Target Asset").x;

    NodeText_Out("Target Asset", labelWidth,
        ImGui::GetCursorPosX(), headerWidth, ImGui::GetStyle().ItemSpacing.x,
        _solverNodeInfo.outputPins[SolverNodeData::OutputPin_Solver], Widget::MNBV::GetPinColors(PinType_Asset));

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::SpeciesNode(const char* _name, SpeciesNodeData& _speciesNodeInfo)
{
    PushNodeStyle(Widget::MNBV::GetNodeColors(NodeType_Species));
    ax::NodeEditor::BeginNode(_speciesNodeInfo.id);

    const float headerWidth = NodeHeader("Species:", _name, Widget::MNBV::GetNodeColors(NodeType_Species));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    if (NodeCollapsingHeader_InOut("Model Links", _speciesNodeInfo.collapsingHeadersIds[SpeciesNodeData::CollapsingHeader_ModelLinks],
        _speciesNodeInfo.utilityState, SpeciesNodeData::State_CollHdrModelLinks,
        startX, headerWidth,
        _speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrModelLinks], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrModelLinks], Widget::MNBV::GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0)))
    {
        NodeText_In("Asset", startX, _speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_Asset], Widget::MNBV::GetPinColors(PinType_Asset));

        if (_speciesNodeInfo.equationDep.size())
        {
            if (NodeCollapsingHeader_InOut("In Equations", _speciesNodeInfo.collapsingHeadersIds[SpeciesNodeData::CollapsingHeader_InEquation],
                _speciesNodeInfo.utilityState, SpeciesNodeData::State_CollHdrInEquation,
                startX, headerWidth,
                _speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrInEquation], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrInEquation], Widget::MNBV::GetPinColors(PinType_Equation),
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
                _speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrAsReactant], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrAsReactant], Widget::MNBV::GetPinColors(PinType_Reaction),
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
                _speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrAsProduct], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrAsProduct], Widget::MNBV::GetPinColors(PinType_Reaction),
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
                _speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrInKineticLaw], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrInKineticLaw], Widget::MNBV::GetPinColors(PinType_Reaction),
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
        _speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_CollHdrDataFields], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_CollHdrDataFields], Widget::MNBV::GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0)))
    {
        if (NodeInputFloat_InOut("Quantity", _speciesNodeInfo.id.Get(), &_speciesNodeInfo.speciesQuantityBuffer,
            itemsWidth, startX, headerWidth,
            _speciesNodeInfo.inputPins[SpeciesNodeData::InputPin_Quantity], _speciesNodeInfo.outputPins[SpeciesNodeData::OutputPin_Quantity], Widget::MNBV::GetPinColors(PinType_ValueFloat),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            _speciesNodeInfo.data->Set(_speciesNodeInfo.speciesQuantityBuffer);
        }
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::ValueFloatNode(const char* _name, ValueFloatNodeData& _valueFloatNodeInfo)
{
    PushNodeStyle(Widget::MNBV::GetNodeColors(NodeType_Data));
    ax::NodeEditor::BeginNode(_valueFloatNodeInfo.id);

    const float headerWidth = NodeHeader("Value:", _name, Widget::MNBV::GetNodeColors(NodeType_Data));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    if (NodeDragFloat_Out("Value", _valueFloatNodeInfo.id.Get(), &_valueFloatNodeInfo.value,
        itemsWidth, startX, headerWidth,
        _valueFloatNodeInfo.outputPins[ValueFloatNodeData::OutputPin_Value], Widget::MNBV::GetPinColors(PinType_ValueFloat),
        ImGuiInputTextFlags_ReadOnly))
    {
        _valueFloatNodeInfo.OutputUpdate(_valueFloatNodeInfo.outputPins[ValueFloatNodeData::OutputPin_Value]);
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}
#pragma endregion

#pragma region Node Pins
void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::Pin(const NodePinData& _pinData, const ImVec4 _pinColors[])
{
    ax::NodeEditor::BeginPin(_pinData.id, _pinData.kind);

    //We center on the Y axis the start position of the cursor relatively to texts within buttons in ImGui
    //We chose buttons as reference as they are used extensively in nodes.
    const ImVec2 startPos = ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 0.5f * ImGui::GetTextLineHeight());
    const ImVec2 endPos = ImVec2(startPos.x + Widget::MNBV::GetMNBVStyle()->pinWidth, startPos.y + Widget::MNBV::GetMNBVStyle()->pinWidth);

    const ImRect bb(startPos, endPos);
    ax::NodeEditor::PinRect(bb.Min, bb.Max);
    if (ImGui::ItemAdd(bb, 0))
    {
        PinColorType bgColor = PinColorType_BgInactivated;
        if (_pinData.isUsed || ImGui::IsItemHovered())
        {
            bgColor = PinColorType_BgActivated;
        }

        //Drawing the center of the pin
        ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, ImColor(_pinColors[bgColor]));
        //Going back to start position
        ImGui::SetCursorPos(startPos);
        //Drawing the border of the pin on top of the center.
        ImGui::GetWindowDrawList()->AddRect(bb.Min, bb.Max, ImColor(_pinColors[PinColorType_Border]));

        // We set the cursor here to where it already is trigger the automatic addition of the Itemspacing.x 
        // at the end of the pin when anything is drawn on the same line after it.
        ImGui::SetCursorPosX(endPos.x);
    }
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

                if (startPin == endPin)
                {
                    ax::NodeEditor::RejectNewItem(ImVec4(1.0f, 0.f, 0.f, 1.0f), 2.0f);
                    ax::NodeEditor::Suspend();
                    ImGui::SetTooltip("You cannot connect a pin to itself.");
                    ax::NodeEditor::Resume();
                }
                else if (startPin->kind == endPin->kind)
                {
                    ax::NodeEditor::RejectNewItem(ImVec4(1.0f, 0.f, 0.f, 1.0f), 2.0f);
                    ax::NodeEditor::Suspend();
                    ImGui::SetTooltip("You cannot connect an input pin to another input pin\nor output pin to another ouput pin.");
                    ax::NodeEditor::Resume();
                }
                else if (!Widget::MNBV::IsDynamicLinkAuthorized(startPin->type, endPin->type))
                {
                    ax::NodeEditor::RejectNewItem(ImVec4(1.0f, 0.f, 0.f, 1.0f), 2.0f);
                    ax::NodeEditor::Suspend();
                    ImGui::SetTooltip("You cannot connect a pin of type %s to a pin of type %s", Widget::MNBV::GetPinTypeName(startPin->type), Widget::MNBV::GetPinTypeName(endPin->type));
                    ax::NodeEditor::Resume();
                }
                else
                {
                    // ax::NodeEditor::AcceptNewItem() return true when user release mouse button.
                    if (ax::NodeEditor::AcceptNewItem(ImVec4(0.0f, 1.f, 0.f, 1.0f), 2.0f))
                    {
                        NodeOutputPinData* outputPin = static_cast<NodeOutputPinData*>(startPin);
                        NodeInputPinData* inputPin = static_cast<NodeInputPinData*>(endPin);

                        // Since we accepted new link, lets add one to our list of links.
                        _links.push_back(LinkData(outputPin, inputPin));

                        outputPin->AddSubscriber(inputPin);
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
    const NodePinData& _pin, const ImVec4 _pinColors[],
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
        Pin(_pin, _pinColors);
    }

    return open;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeCollapsingHeader_InOut(const char* _label, const std::size_t _id,
    unsigned short& _utilityState, const short _stateBitPos,
    const float _startX, const float _drawLength,
    const NodePinData& _inputPin, const NodePinData& _outputPin, const ImVec4 _pinColors[],
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
        Pin(_inputPin, _pinColors);

        ImGui::SameLine(); AlignToRight(_startX, _drawLength, Widget::MNBV::GetMNBVStyle()->pinWidth);
        Pin(_outputPin, _pinColors);
    }

    return open;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeCollapsingHeader_Out(const char* _label, const std::size_t _id,
    unsigned short& _utilityState, const short _stateBitPos,
    const float _startX, const float _drawLength,
    const NodePinData& _pin, const ImVec4 _pinColors[],
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
        AlignToRight(_startX, _drawLength, Widget::MNBV::GetMNBVStyle()->pinWidth);
        Pin(_pin, _pinColors);
    }

    return open;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeDragFloat_Out(const char* _label, const std::size_t _id, float* valueBuffer,
    const float _inputFieldWidth, const float _startX, const float _drawLength,
    const NodePinData& _pin, const ImVec4 _pinColors[],
    const ImGuiInputTextFlags _flags)
{
    ImGui::SetCursorPosX(_startX);

    ImGui::PushID((int)_id);
    ImGui::AlignTextToFramePadding();
    AlignToRight(_startX, _drawLength - Widget::MNBV::GetMNBVStyle()->pinWidth - ImGui::GetStyle().ItemSpacing.x - (_inputFieldWidth - ImGui::CalcTextSize(_label).x), ImGui::CalcTextSize(_label).x);

    ImGui::Text(_label); ImGui::SameLine();

    ImGui::SetNextItemWidth(_inputFieldWidth - ImGui::CalcTextSize(_label).x - ImGui::GetStyle().ItemSpacing.x);
    bool edited = ImGui::DragFloat("##value", valueBuffer, 1.0f, 0.f, 0.f, "%.3f", _flags);
    ImGui::PopID();

    ImGui::SameLine();

    Pin(_pin, _pinColors);

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

    if (ImGui::ItemAdd(bb, 0))
    {
        /*NodeStyleColor colFlag = NodeStyleColor_HeaderBg;
        if (ImGui::IsItemHovered())
        {
            colFlag = NodeStyleColor_HeaderHovered;
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                ECellEngine::Logging::Logger::GetSingleton().LogDebug("Id of Hovered Item is: " + std::to_string(ImGui::GetItemID()));
            }
        }

        if (ImGui::IsItemClicked())
        {
            colFlag = NodeStyleColor_HeaderActivated;
        }*/

        ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, ImColor(_colorSet[NodeColorType_HeaderBg]), ax::NodeEditor::GetStyle().NodeRounding);
        ImGui::SetCursorPos(ImVec2(startPos.x + ImGui::GetStyle().FramePadding.x, startPos.y));
        ImGui::Text(_type); ImGui::SameLine(); ImGui::Text(_name);
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);

    NodeHorizontalSeparator(bb.GetWidth());

    return bb.GetWidth();
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeHorizontalSeparator(const float _width, const float _thickness)
{
    //ItemAdd and ImRect are part of imgui_internal.hpp
    const ImVec2 startPos = ImGui::GetCursorPos();
    const ImRect bb(startPos.x, startPos.y, startPos.x + _width, startPos.y);
    if (ImGui::ItemAdd(bb, 0))
    {
        ImGui::ItemSize(ImVec2(0.0f, _thickness));
        ImGui::GetWindowDrawList()->AddLine(bb.Min, bb.Max, ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Separator)), _thickness);
    }
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeInputFloat_InOut(const char* _label, const std::size_t _id, float* valueBuffer,
    const float _inputFieldWidth, const float _startX, const float _drawLength,
    const NodePinData& _inputPin, const NodePinData& _outputPin, const ImVec4 _pinColors[],
    const ImGuiInputTextFlags _flags)
{
    ImGui::SetCursorPosX(_startX);

    Pin(_inputPin, _pinColors); ImGui::SameLine();

    ImGui::PushID((int)_id);
    ImGui::AlignTextToFramePadding(); AlignToCenter(_startX, _drawLength, _inputFieldWidth);

    ImGui::Text(_label); ImGui::SameLine();
    ImGui::SetNextItemWidth(_inputFieldWidth - ImGui::CalcTextSize(_label).x - ImGui::GetStyle().ItemSpacing.x);
    bool edited = ImGui::InputFloat("##quantity", valueBuffer, 0.f, 0.f, "%.3f", _flags);
    if (ImGui::IsItemActive() && (_flags & ImGuiInputTextFlags_EnterReturnsTrue))
    {
        ax::NodeEditor::Suspend();
        ImGui::SetTooltip("Press ENTER to confirm.");
        ax::NodeEditor::Resume();
    }
    ImGui::PopID();

    ImGui::SameLine();

    AlignToRight(_startX, _drawLength, Widget::MNBV::GetMNBVStyle()->pinWidth);
    Pin(_outputPin, _pinColors);

    return edited;
}

bool ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeInputFloat_Out(const char* _label, const std::size_t _id, float* valueBuffer,
    const float _inputFieldWidth, const float _startX, const float _drawLength,
    const NodePinData& _pin, const ImVec4 _pinColors[],
    const ImGuiInputTextFlags _flags)
{
    ImGui::SetCursorPosX(_startX);

    ImGui::PushID((int)_id);
    ImGui::AlignTextToFramePadding();
    AlignToRight(_startX, _drawLength - Widget::MNBV::GetMNBVStyle()->pinWidth - ImGui::GetStyle().ItemSpacing.x - (_inputFieldWidth - ImGui::CalcTextSize(_label).x), ImGui::CalcTextSize(_label).x);

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

    Pin(_pin, _pinColors);

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
        PushScrollBarStyle(style);
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
        PopScrollBarStyle(); //poping after PushScrollBarStyle(style);
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
    const NodePinData& _pin, const ImVec4 _pinColors[])
{
    ImGui::SetCursorPosX(_startX);
    Pin(_pin, _pinColors);

    ImGui::SameLine();

    ImGui::AlignTextToFramePadding();
    ImGui::Text(_label);
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeText_InOut(const char* _label, const float _labelWidth,
    const float _startX, const float _drawLength,
    const NodePinData& _inputPin, const NodePinData& _outputPin,
    const ImVec4 _pinColors[])
{
    ImGui::SetCursorPosX(_startX);
    Pin(_inputPin, _pinColors); ImGui::SameLine();

    ImGui::AlignTextToFramePadding();
    AlignToCenter(_startX, _drawLength, _labelWidth);
    ImGui::Text(_label);

    ImGui::SameLine();

    AlignToRight(_startX, _drawLength, Widget::MNBV::GetMNBVStyle()->pinWidth);
    Pin(_outputPin, _pinColors);
}

void ECellEngine::Editor::Utility::MNBV::NodeEditorDraw::NodeText_Out(const char* _label, const float _labelWidth,
    const float _startX, const float _drawLength, const float itemSpacingX,
    const NodePinData& _pin, const ImVec4 _pinColors[])
{
    AlignToRight(_startX, _drawLength - Widget::MNBV::GetMNBVStyle()->pinWidth - itemSpacingX, _labelWidth);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(_label);

    ImGui::SameLine();

    Pin(_pin, _pinColors);
}

#pragma endregion