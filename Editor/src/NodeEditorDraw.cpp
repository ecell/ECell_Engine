#include "NodeEditorDraw.hpp"

#pragma region Nodes
void ECellEngine::Editor::Utility::NodeEditorDraw::AssetNode(const char* _name, AssetNodeData& _assetNodeInfo)
{
    PushNodeStyle(GetNodeColors(NodeType_Asset));
    ax::NodeEditor::BeginNode(_assetNodeInfo.id);

    const float headerWidth = NodeHeader("Asset:", _name, GetNodeColors(NodeType_Asset));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    // ----- Pin and Text to connect the solver to the asset -----
    Pin(_assetNodeInfo.inputPins[0], GetPinColors(PinType_Solver));
    ImGui::SameLine(); ImGui::AlignTextToFramePadding(); ImGui::Text("Solver");

    // ----- String List Box and Pin to access the species of the asset -----
    if (NodeCollapsingHeader_Out("Species", _assetNodeInfo.collapsingHeadersIds[0],
        _assetNodeInfo.utilityState, 0,
        startX, headerWidth,
        _assetNodeInfo.outputPins[0], GetPinColors(PinType_Species),
        ImVec2(itemsWidth, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.speciesNLB, startX, headerWidth, itemsWidth);
    }

    // ----- String List Box and Pin to access the simple parameters of the asset -----
    if (NodeCollapsingHeader_Out("Constant Parameters", _assetNodeInfo.collapsingHeadersIds[1],
        _assetNodeInfo.utilityState, 1,
        startX, headerWidth,
        _assetNodeInfo.outputPins[1], GetPinColors(PinType_Parameter),
        ImVec2(itemsWidth, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.simpleParametersNLB, startX, headerWidth, itemsWidth);
    }

    // ----- String List Box and Pin to access the computed parameters of the asset -----
    if (NodeCollapsingHeader_Out("Computed Parameters", _assetNodeInfo.collapsingHeadersIds[2],
        _assetNodeInfo.utilityState, 2,
        startX, headerWidth,
        _assetNodeInfo.outputPins[2], GetPinColors(PinType_Parameter),
        ImVec2(itemsWidth, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.computedParametersNLB, startX, headerWidth, itemsWidth);
    }

    // ----- String List Box and Pin to access the reactions of the asset -----
    if (NodeCollapsingHeader_Out("Reactions", _assetNodeInfo.collapsingHeadersIds[3],
        _assetNodeInfo.utilityState, 3,
        startX, headerWidth,
        _assetNodeInfo.outputPins[3], GetPinColors(PinType_Reaction),
        ImVec2(itemsWidth, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.reactionsNLB, startX, headerWidth, itemsWidth);
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();

}

void ECellEngine::Editor::Utility::NodeEditorDraw::ComputedParameterNode(const char* _name, ComputedParameterNodeData& _parameterNodeInfo)
{
    PushNodeStyle(GetNodeColors(NodeType_Parameter));
    ax::NodeEditor::BeginNode(_parameterNodeInfo.id);

    const float headerWidth = NodeHeader("Computed Parameter:", _name, GetNodeColors(NodeType_Parameter));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    if (NodeCollapsingHeader_InOut("Model Links", _parameterNodeInfo.collapsingHeadersIds[0],
        _parameterNodeInfo.utilityState, 0,
        startX, headerWidth,
        _parameterNodeInfo.inputPins[0], _parameterNodeInfo.outputPins[0], GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0.f)))
    {

        NodeText_In("Asset", startX, _parameterNodeInfo.inputPins[1], GetPinColors(PinType_Asset));

        if (NodeCollapsingHeader_InOut("Computed Parameters", _parameterNodeInfo.collapsingHeadersIds[1],
            _parameterNodeInfo.utilityState, 1,
            startX, headerWidth,
            _parameterNodeInfo.inputPins[2], _parameterNodeInfo.outputPins[1], GetPinColors(PinType_Parameter),
            ImVec2(itemsWidth, 0.f), false))
        {
            //NodeStringListBox(_parameterNodeInfo.nslbData[0], GetPinDrawOffset(), 200.f, 0);
            ImGui::Text("A List box will be here.");
        }

        if (NodeCollapsingHeader_InOut("Kinetic Laws", _parameterNodeInfo.collapsingHeadersIds[2],
            _parameterNodeInfo.utilityState, 2,
            startX, headerWidth,
            _parameterNodeInfo.inputPins[3], _parameterNodeInfo.outputPins[2], GetPinColors(PinType_Parameter),
            ImVec2(itemsWidth, 0.f), false))
        {
            //NodeStringListBox(_parameterNodeInfo.nslbData[0], GetPinDrawOffset(), 200.f, 0);
            ImGui::Text("A List box will be here.");
        }
    }

    NodeHorizontalSeparator(headerWidth);

    if (NodeCollapsingHeader_InOut("Data Fields", _parameterNodeInfo.collapsingHeadersIds[3],
        _parameterNodeInfo.utilityState, 3,
        startX, headerWidth,
        _parameterNodeInfo.inputPins[4], _parameterNodeInfo.outputPins[3], GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0)))
    {

        if (NodeCollapsingHeader_InOut("Operands", _parameterNodeInfo.collapsingHeadersIds[4],
            _parameterNodeInfo.utilityState, 4,
            startX, headerWidth,
            _parameterNodeInfo.inputPins[5], _parameterNodeInfo.outputPins[4], GetPinColors(PinType_Default),
            ImVec2(itemsWidth, 0.f)))
        {
            if (_parameterNodeInfo.nslbData[2].data->size())
            {
                const float spTextWidth = ImGui::CalcTextSize("Species").x;
                NodeText_InOut("Species", spTextWidth, startX, headerWidth,
                    _parameterNodeInfo.inputPins[6], _parameterNodeInfo.outputPins[5], GetPinColors(PinType_Species));
                NodeStringListBox(_parameterNodeInfo.nslbData[2], startX, headerWidth, itemsWidth);
            }

            if (_parameterNodeInfo.nslbData[3].data->size())
            {
                const float sparamTextWidth = ImGui::CalcTextSize("Simple Parameters").x;
                NodeText_InOut("Simple Parameters", sparamTextWidth, startX, headerWidth,
                    _parameterNodeInfo.inputPins[7], _parameterNodeInfo.outputPins[6], GetPinColors(PinType_Parameter));
                NodeStringListBox(_parameterNodeInfo.nslbData[3], startX, headerWidth, itemsWidth);
            }

            if (_parameterNodeInfo.nslbData[4].data->size())
            {
                const float cparaTextWidth = ImGui::CalcTextSize("Computed Parameters").x;
                NodeText_InOut("Computed Parameters", cparaTextWidth, startX, headerWidth,
                    _parameterNodeInfo.inputPins[8], _parameterNodeInfo.outputPins[7], GetPinColors(PinType_Parameter));
                NodeStringListBox(_parameterNodeInfo.nslbData[4], startX, headerWidth, itemsWidth);
            }
        }

        //const static float inputFieldWidth = headerWidth -  + ImGui::GetStyle().ItemSpacing.x; //100.0f is the default length for a field
        float value = _parameterNodeInfo.data->Get();
        NodeInputFloat_InOut("Value", _parameterNodeInfo.id.Get(), &value,
            itemsWidth, startX, headerWidth,
            _parameterNodeInfo.inputPins[9], _parameterNodeInfo.outputPins[8], GetPinColors(PinType_Default),
            ImGuiInputTextFlags_ReadOnly);
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::ReactionNode(const char* _name, ReactionNodeData& _reactionNodeInfo)
{
    PushNodeStyle(GetNodeColors(NodeType_Reaction));
    ax::NodeEditor::BeginNode(_reactionNodeInfo.id);

    const float headerWidth = NodeHeader("Reaction:", _name, GetNodeColors(NodeType_Reaction));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    if (NodeCollapsingHeader_InOut("Model Links", _reactionNodeInfo.collapsingHeadersIds[0],
        _reactionNodeInfo.utilityState, 0,
        startX, headerWidth,
        _reactionNodeInfo.inputPins[0], _reactionNodeInfo.outputPins[0], GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0.f)))
    {

        NodeText_In("Asset", startX, _reactionNodeInfo.inputPins[1], GetPinColors(PinType_Asset));

        if (NodeCollapsingHeader_InOut("Reactants", _reactionNodeInfo.collapsingHeadersIds[1],
            _reactionNodeInfo.utilityState, 1,
            startX, headerWidth,
            _reactionNodeInfo.inputPins[2], _reactionNodeInfo.outputPins[1], GetPinColors(PinType_Species),
            ImVec2(itemsWidth, 0.f), false))
        {
            NodeStringListBox(_reactionNodeInfo.nslbData[0], startX, headerWidth, itemsWidth);
        }

        if (NodeCollapsingHeader_InOut("Products", _reactionNodeInfo.collapsingHeadersIds[2],
            _reactionNodeInfo.utilityState, 2,
            startX, headerWidth,
            _reactionNodeInfo.inputPins[3], _reactionNodeInfo.outputPins[2], GetPinColors(PinType_Species),
            ImVec2(itemsWidth, 0.f), false))
        {
            NodeStringListBox(_reactionNodeInfo.nslbData[1], startX, headerWidth, itemsWidth);
        }
    }

    NodeHorizontalSeparator(headerWidth);

    if (NodeCollapsingHeader_InOut("Kinetic Law", _reactionNodeInfo.collapsingHeadersIds[3],
        _reactionNodeInfo.utilityState, 3,
        startX, headerWidth,
        _reactionNodeInfo.inputPins[4], _reactionNodeInfo.outputPins[3], GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0)))
    {

        if (NodeCollapsingHeader_InOut("Operands", _reactionNodeInfo.collapsingHeadersIds[4],
            _reactionNodeInfo.utilityState, 4,
            startX, headerWidth,
            _reactionNodeInfo.inputPins[5], _reactionNodeInfo.outputPins[4], GetPinColors(PinType_Default),
            ImVec2(itemsWidth, 0.f)))
        {
            if (_reactionNodeInfo.nslbData[2].data->size())
            {
                const float spTextWidth = ImGui::CalcTextSize("Species").x;
                NodeText_InOut("Species", spTextWidth, startX, headerWidth,
                    _reactionNodeInfo.inputPins[6], _reactionNodeInfo.outputPins[5], GetPinColors(PinType_Species));
                NodeStringListBox(_reactionNodeInfo.nslbData[2], startX, headerWidth, itemsWidth);
            }

            if (_reactionNodeInfo.nslbData[3].data->size())
            {
                const float sparamTextWidth = ImGui::CalcTextSize("Simple Parameters").x;
                NodeText_InOut("Simple Parameters", sparamTextWidth, startX, headerWidth,
                    _reactionNodeInfo.inputPins[7], _reactionNodeInfo.outputPins[6], GetPinColors(PinType_Parameter));
                NodeStringListBox(_reactionNodeInfo.nslbData[3], startX, headerWidth, itemsWidth);
            }

            if (_reactionNodeInfo.nslbData[4].data->size())
            {
                const float cparaTextWidth = ImGui::CalcTextSize("Computed Parameters").x;
                NodeText_InOut("Computed Parameters", cparaTextWidth, startX, headerWidth,
                    _reactionNodeInfo.inputPins[8], _reactionNodeInfo.outputPins[7], GetPinColors(PinType_Parameter));
                NodeStringListBox(_reactionNodeInfo.nslbData[4], startX, headerWidth, itemsWidth);
            }
        }

        float value = _reactionNodeInfo.data->GetKineticLawValue();
        NodeInputFloat_InOut("Value", _reactionNodeInfo.id.Get(), &value,
            itemsWidth, startX, headerWidth,
            _reactionNodeInfo.inputPins[9], _reactionNodeInfo.outputPins[8], GetPinColors(PinType_Default),
            ImGuiInputTextFlags_ReadOnly);
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::SimpleParameterNode(const char* _name, SimpleParameterNodeData& _parameterNodeInfo)
{
    PushNodeStyle(GetNodeColors(NodeType_Parameter));
    ax::NodeEditor::BeginNode(_parameterNodeInfo.id);

    const float headerWidth = NodeHeader("Simple Parameter:", _name, GetNodeColors(NodeType_Parameter));
    const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float startX = ImGui::GetCursorPosX();

    if (NodeCollapsingHeader_InOut("Model Links", _parameterNodeInfo.collapsingHeadersIds[0],
        _parameterNodeInfo.utilityState, 0,
        startX, headerWidth,
        _parameterNodeInfo.inputPins[0], _parameterNodeInfo.outputPins[0], GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0.f)))
    {

        NodeText_In("Asset", startX, _parameterNodeInfo.inputPins[1], GetPinColors(PinType_Asset));

        if (NodeCollapsingHeader_InOut("Computed Parameters", _parameterNodeInfo.collapsingHeadersIds[1],
            _parameterNodeInfo.utilityState, 1,
            startX, headerWidth,
            _parameterNodeInfo.inputPins[2], _parameterNodeInfo.outputPins[1], GetPinColors(PinType_Parameter),
            ImVec2(itemsWidth, 0.f), false))
        {
            //NodeStringListBox(_parameterNodeInfo.nslbData[0], GetPinDrawOffset(), itemsWidth, 0);
            ImGui::Text("A List box will be here.");
        }

        if (NodeCollapsingHeader_InOut("Kinetic Laws", _parameterNodeInfo.collapsingHeadersIds[2],
            _parameterNodeInfo.utilityState, 2,
            startX, headerWidth,
            _parameterNodeInfo.inputPins[3], _parameterNodeInfo.outputPins[2], GetPinColors(PinType_Parameter),
            ImVec2(itemsWidth, 0.f), false))
        {
            //NodeStringListBox(_parameterNodeInfo.nslbData[0], GetPinDrawOffset(), itemsWidth, 0);
            ImGui::Text("A List box will be here.");
        }
    }

    NodeHorizontalSeparator(headerWidth);

    if (NodeCollapsingHeader_InOut("Data Fields", _parameterNodeInfo.collapsingHeadersIds[3],
        _parameterNodeInfo.utilityState, 3,
        startX, headerWidth,
        _parameterNodeInfo.inputPins[4], _parameterNodeInfo.outputPins[3], GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0)))
    {
        float value = _parameterNodeInfo.data->Get();
        if (NodeInputFloat_InOut("Value", _parameterNodeInfo.id.Get(), &value,
            itemsWidth, startX, headerWidth,
            _parameterNodeInfo.inputPins[5], _parameterNodeInfo.outputPins[4], GetPinColors(PinType_Default),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            _parameterNodeInfo.data->Set(value);
        }
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::SolverNode(const char* _name, const SolverNodeData& _solverNodeInfo)
{
    PushNodeStyle(GetNodeColors(NodeType_Solver));
    ax::NodeEditor::BeginNode(_solverNodeInfo.id);

    const float headerWidth = NodeHeader("Solver:", _name, GetNodeColors(NodeType_Solver));
    //const float itemsWidth = GetNodeCenterAreaWidth(headerWidth);
    const float labelWidth = ImGui::CalcTextSize("Target Asset").x;

    NodeText_Out("Target Asset", labelWidth,
        ImGui::GetCursorPosX(), headerWidth, ImGui::GetStyle().ItemSpacing.x,
        _solverNodeInfo.outputPins[0], GetPinColors(PinType_Asset));

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::SpeciesNode(const char* _name, SpeciesNodeData& _speciesNodeInfo)
{
    PushNodeStyle(GetNodeColors(NodeType_Species));
    ax::NodeEditor::BeginNode(_speciesNodeInfo.id);
    
    const float headerSize = NodeHeader("Species:", _name, GetNodeColors(NodeType_Species));
    const float itemsWidth = GetNodeCenterAreaWidth(headerSize);
    const float startX = ImGui::GetCursorPosX();

    if (NodeCollapsingHeader_InOut("Model Links", _speciesNodeInfo.collapsingHeadersIds[0],
        _speciesNodeInfo.utilityState, 0,
        startX, headerSize,
        _speciesNodeInfo.inputPins[0], _speciesNodeInfo.outputPins[0], GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0)))
    {
        const static float icpTextWidth = ImGui::CalcTextSize("In Computed Parameters").x;
        const static float irTextWidth = ImGui::CalcTextSize("Is Reactant").x;
        const static float ipTextWidth = ImGui::CalcTextSize("Is Product").x;
        const static float iklTextWidth = ImGui::CalcTextSize("In Kinetic Law").x;

        NodeText_In("Asset", startX, _speciesNodeInfo.inputPins[1], GetPinColors(PinType_Asset));

        NodeText_InOut("In Computed Parameters", icpTextWidth,
            startX, headerSize,
            _speciesNodeInfo.inputPins[2], _speciesNodeInfo.outputPins[1], GetPinColors(PinType_Parameter));

        NodeText_InOut("Is Reactant", irTextWidth,
            startX, headerSize,
            _speciesNodeInfo.inputPins[3], _speciesNodeInfo.outputPins[2], GetPinColors(PinType_Reaction));

        NodeText_InOut("Is Product", ipTextWidth,
            startX, headerSize,
            _speciesNodeInfo.inputPins[4], _speciesNodeInfo.outputPins[3], GetPinColors(PinType_Reaction));

        NodeText_InOut("In Kinetic Law", iklTextWidth,
            startX, headerSize,
            _speciesNodeInfo.inputPins[5], _speciesNodeInfo.outputPins[4], GetPinColors(PinType_Reaction));
    }

    NodeHorizontalSeparator(headerSize);

    if (NodeCollapsingHeader_InOut("Data Fields", _speciesNodeInfo.collapsingHeadersIds[1],
        _speciesNodeInfo.utilityState, 1,
        startX, headerSize,
        _speciesNodeInfo.inputPins[6], _speciesNodeInfo.outputPins[5], GetPinColors(PinType_Default),
        ImVec2(itemsWidth, 0)))
    {
        float value = _speciesNodeInfo.data->Get();
        if (NodeInputFloat_InOut("Quantity", _speciesNodeInfo.id.Get(), &value,
            itemsWidth, startX, headerSize,
            _speciesNodeInfo.inputPins[7], _speciesNodeInfo.outputPins[6], GetPinColors(PinType_Default),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            _speciesNodeInfo.data->Set(value);
        }
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

#pragma endregion

#pragma region Node Pins
void ECellEngine::Editor::Utility::NodeEditorDraw::InputPin(const NodePinData& _pinData, const ImVec4 _pinColors[], const float _size)
{
    ax::NodeEditor::BeginPin(_pinData.id, ax::NodeEditor::PinKind::Input);

    //We center on the Y axis the start position of the cursor relatively to texts within buttons in ImGui
    //We chose buttons as reference as they are used extensively in nodes.
    const ImVec2 startPos = ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 0.5f * ImGui::GetTextLineHeight());
    const ImVec2 endPos = ImVec2(startPos.x + _size, startPos.y + _size);

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

void ECellEngine::Editor::Utility::NodeEditorDraw::OutputPin(const NodePinData& _pinData, const ImVec4 _pinColors[], const float _size)
{
    ax::NodeEditor::BeginPin(_pinData.id, ax::NodeEditor::PinKind::Output);

    //We center on the Y axis the start position of the cursor relatively to texts within buttons in ImGui
    //We chose buttons as reference as they are used extensively in nodes.
    const ImVec2 startPos = ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 0.5f * ImGui::GetTextLineHeight());
    const ImVec2 endPos = ImVec2(startPos.x + _size, startPos.y + _size);

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

        // We set the cursor here to where it already is trigger the automatic addition of the Nodepadding.z (right side)
        // after the outpin. Normally, there is no reason to have anything on the right side of an outpin within a node.
        ImGui::SetCursorPosX(endPos.x);
    }
    ax::NodeEditor::EndPin();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::LinkCreation(std::size_t& _id, std::vector<ECellEngine::Editor::Utility::LinkData>& _links)
{
    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ax::NodeEditor::BeginCreate())
    {
        ax::NodeEditor::PinId inputPinId, outputPinId;
        if (ax::NodeEditor::QueryNewLink(&inputPinId, &outputPinId))
        {
            // QueryNewLink returns true if editor want to create new link between pins.
            //
            // Link can be created only for two valid pins, it is up to you to
            // validate if connection make sense. Editor is happy to make any.
            //
            // Link always goes from input to output. User may choose to drag
            // link from output pin or input pin. This determine which pin ids
            // are valid and which are not:
            //   * input valid, output invalid - user started to drag new ling from input pin
            //   * input invalid, output valid - user started to drag new ling from output pin
            //   * input valid, output valid   - user dragged link over other pin, can be validated

            if (inputPinId && outputPinId) // both are valid, let's accept link
            {
                // ax::NodeEditor::AcceptNewItem() return true when user release mouse button.
                if (ax::NodeEditor::AcceptNewItem())
                {
                    // Since we accepted new link, lets add one to our list of links.
                    _links.push_back(LinkData(_id, inputPinId, outputPinId));

                    // Draw new link.
                    //ax::NodeEditor::Link(_links.back().id, _links.back().startId, _links.back().endId);
                    ECellEngine::Editor::Utility::NodeEditorDraw::Link(_links.back());
                }

                // You may choose to reject connection between these nodes
                // by calling ax::NodeEditor::RejectNewItem(). This will allow editor to give
                // visual feedback by changing link thickness and color.
            }
        }
    }
    ax::NodeEditor::EndCreate(); // Wraps up object creation action handling.
}

void ECellEngine::Editor::Utility::NodeEditorDraw::LinkDestruction(std::vector<ECellEngine::Editor::Utility::LinkData>& _links)
{
    if (ax::NodeEditor::BeginDelete())
    {
        // There may be many links marked for deletion, let's loop over them.
        ax::NodeEditor::LinkId deletedLinkId;
        while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId))
        {
            // If you agree that link can be deleted, accept deletion.
            if (ax::NodeEditor::AcceptDeletedItem())
            {
                // Then remove link from your data.
                for (std::vector<ECellEngine::Editor::Utility::LinkData>::iterator it = _links.begin(); it != _links.end(); it++)
                {
                    if ((*it).id == deletedLinkId)
                    {
                        _links.erase(it);
                        break;
                    }
                }
            }

            // You may reject link deletion by calling:
            // ax::NodeEditor::RejectDeletedItem();
        }
    }
    ax::NodeEditor::EndDelete(); // Wrap up deletion action

}

void ECellEngine::Editor::Utility::NodeEditorDraw::Link(LinkData& linkInfo)
{
    if (ax::NodeEditor::Link(linkInfo.id, linkInfo.startIds[0], linkInfo.endIds[0])) { return; }

    if (ax::NodeEditor::Link(linkInfo.id, linkInfo.startIds[0], linkInfo.endIds[1])) { return; }

    if (ax::NodeEditor::Link(linkInfo.id, linkInfo.startIds[1], linkInfo.endIds[0])) { return; }

    ax::NodeEditor::Link(linkInfo.id, linkInfo.startIds[1], linkInfo.endIds[1]);
}
#pragma endregion

#pragma region Custom Node Widgets

bool ECellEngine::Editor::Utility::NodeEditorDraw::NodeCollapsingHeader_InOut(const char* _label, const std::size_t _id,
    unsigned char& _utilityState, const short _stateBitPos,
    const float _startX, const float _drawLength,
    const NodePinData& _inputPin, const NodePinData& _outputPin, const ImVec4 _pinColors[],
    const ImVec2& _size, const bool _hidePinsOnExpand)
{
    AlignToCenter(_startX, _drawLength, _size.x);
    ImGui::PushID(_id);
    if (ImGui::Button(_label, _size))
    {
        _utilityState ^= 1 << _stateBitPos;
    }
    ImGui::PopID();

    bool open = _utilityState >> _stateBitPos & 1;

    if (!open || !_hidePinsOnExpand)
    {
        ImGui::SameLine(); ImGui::SetCursorPosX(_startX);
        InputPin(_inputPin, _pinColors);

        ImGui::SameLine(); AlignToRight(_startX, _drawLength, GetMNBVStyle()->pinWidth);
        OutputPin(_outputPin, _pinColors);
    }

    return open;
}

bool ECellEngine::Editor::Utility::NodeEditorDraw::NodeCollapsingHeader_Out(const char* _label, const std::size_t _id,
    unsigned char& _utilityState, const short _stateBitPos,
    const float _startX, const float _drawLength,
    const NodePinData& _pin, const ImVec4 _pinColors[],
    const ImVec2& _size, const bool _hidePinsOnExpand)
{
    AlignToCenter(_startX, _drawLength, _size.x);
    ImGui::PushID(_id);
    if (ImGui::Button(_label, _size))
    {
        _utilityState ^= 1 << _stateBitPos;
    }
    ImGui::PopID();

    bool open = _utilityState >> _stateBitPos & 1;

    if (!open || !_hidePinsOnExpand)
    {
        ImGui::SameLine();
        AlignToRight(_startX, _drawLength, GetMNBVStyle()->pinWidth);
        OutputPin(_pin, _pinColors);
    }

    return open;
}

float ECellEngine::Editor::Utility::NodeEditorDraw::NodeHeader(const char* _type, const char* _name,
    const ImVec4 _colorSet[], const float _width, const short _height)
{
    const float titleSize = ImGui::CalcTextSize(_type).x + ImGui::CalcTextSize(_name).x + ImGui::GetStyle().ItemSpacing.x;
    
    const ImVec2 startPos = ImGui::GetCursorScreenPos();
    const ImVec2 endPos = ImVec2(
        startPos.x + std::max(_width + 2.f * GetPinDrawOffset(), titleSize + 2.f * ImGui::GetStyle().FramePadding.x),
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

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeHorizontalSeparator(const float _width, const float _thickness)
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

bool ECellEngine::Editor::Utility::NodeEditorDraw::NodeInputFloat_InOut(const char* _label, const std::size_t _id, float* valueBuffer,
    const float _inputFieldWidth, const float _startX, const float _drawLength,
    const NodePinData& _inputPin, const NodePinData& _outputPin, const ImVec4 _pinColors[],
    const ImGuiInputTextFlags _flags)
{
    ImGui::SetCursorPosX(_startX);

    InputPin(_inputPin, _pinColors); ImGui::SameLine();

    ImGui::PushID(_id);
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
    
    AlignToRight(_startX, _drawLength, GetMNBVStyle()->pinWidth);
    OutputPin(_outputPin, _pinColors);

    return edited;
}

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeStringListBox(NodeListBoxStringData& _nslbData,
    const float _startX, const float _drawLength,
    const float _widgetWidth, const unsigned short _itemViewHeight)
{
    ImGuiStyle& style = ImGui::GetStyle();
    AlignToCenter(_startX, _drawLength, _widgetWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));

    ImGui::BeginGroup();
    const unsigned short nbItems = _nslbData.data->size();
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
    for (int n = _nslbData.cursor; n > _nslbData.cursor - actualViewHeight; n--)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + NodeFramePadding.x);//Left Padding

        itemString = _nslbData.data->at(nbItems - n).c_str();
        if (ImGui::Selectable(itemString, false, ImGuiSelectableFlags_None, ImVec2(itemWidth, 0)))
        {
            _nslbData.selectedItem = nbItems - n;
            _nslbData.SetItemClicked();
        }

        if (ImGui::IsItemHovered())
        {
            _nslbData.hoveredItem = nbItems - n;
            _nslbData.SetItemHovered();

            if (ImGui::CalcTextSize(itemString).x > itemWidth)
            {
                ax::NodeEditor::Suspend();
                ImGui::SetTooltip("%s", itemString);
                ax::NodeEditor::Resume();
            }

            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                _nslbData.doubleClickedItem = nbItems - n;
                _nslbData.SetItemDoubleClicked();
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

        ImGui::PushID(_nslbData.scrollBarID);
        PushScrollBarStyle(style);
        ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
        ImGui::VSliderInt("##VertSliderInt", ImVec2(style.ScrollbarSize, actualViewHeight * ImGui::GetTextLineHeightWithSpacing()),
            (int*)&_nslbData.cursor, actualViewHeight, nbItems, "", ImGuiSliderFlags_None);
        if (ImGui::IsItemHovered())
        {
            _nslbData.SetScrollBarHovered();
        }
        if (ImGui::IsItemActive())
        {
            _nslbData.SetScrollBarActivated();
        }
        ImGui::PopStyleVar(); //poping ImGuiStyleVar_GrabMinSize
        PopScrollBarStyle(); //poping after PushScrollBarStyle(style);
        ImGui::PopID();
    }

    ImGui::PopStyleVar(); //poping ImGuiStyleVar_ItemSpacing
    ImGui::EndGroup();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeText_In(const char* _label, const float _startX,
    const NodePinData& _pin, const ImVec4 _pinColors[])
{
    ImGui::SetCursorPosX(_startX);
    InputPin(_pin, _pinColors);

    ImGui::SameLine();

    ImGui::AlignTextToFramePadding();
    ImGui::Text(_label);
}

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeText_InOut(const char* _label, const float _labelWidth,
    const float _startX, const float _drawLength,
    const NodePinData& _inputPin, const NodePinData& _outputPin,
    const ImVec4 _pinColors[])
{
    ImGui::SetCursorPosX(_startX);
    InputPin(_inputPin, _pinColors); ImGui::SameLine();

    ImGui::AlignTextToFramePadding();
    AlignToCenter(_startX, _drawLength, _labelWidth);
    ImGui::Text(_label);
    
    ImGui::SameLine();

    AlignToRight(_startX, _drawLength, GetMNBVStyle()->pinWidth);
    OutputPin(_outputPin, _pinColors);
}

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeText_Out(const char* _label, const float _labelWidth,
    const float _startX, const float _drawLength, const float itemSpacingX,
    const NodePinData& _pin, const ImVec4 _pinColors[])
{
    AlignToRight(_startX, _drawLength - GetMNBVStyle()->pinWidth - itemSpacingX, _labelWidth);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(_label);

    ImGui::SameLine();

    OutputPin(_pin, _pinColors);
}

#pragma endregion