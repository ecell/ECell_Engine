#include "NodeEditorDraw.hpp"

#pragma region Nodes
void ECellEngine::Editor::Utility::NodeEditorDraw::AssetNode(const char* _name, AssetNodeData& _assetNodeInfo)
{
    PushNodeStyle(GetNodeColors(NodeType_Asset));
    ax::NodeEditor::BeginNode(_assetNodeInfo.id);

    const float headerWidth = NodeHeader("Asset:", _name, GetNodeColors(NodeType_Asset));
    const float startX = ImGui::GetCursorPosX();
    const static float pinWidth = 8.f;

    // ----- Pin and Text to connect the solver to the asset -----
    InputPin(_assetNodeInfo.inputPin, GetPinColors(PinType_Solver));
    ImGui::SameLine(); ImGui::AlignTextToFramePadding(); ImGui::Text("Solver");

    // ----- String List Box and Pin to access the species of the asset -----
    if (NodeCollapsingHeader_Out("Species", _assetNodeInfo.collapsingHeadersIds[0],
        _assetNodeInfo.utilityState, 0,
        startX, headerWidth, pinWidth,
        _assetNodeInfo.outputPins[0], GetPinColors(PinType_Species),
        ImVec2(200, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.speciesNLB, GetPinDrawOffset());
    }

    // ----- String List Box and Pin to access the simple parameters of the asset -----
    if (NodeCollapsingHeader_Out("Constant Parameters", _assetNodeInfo.collapsingHeadersIds[1],
        _assetNodeInfo.utilityState, 1,
        startX, headerWidth, pinWidth,
        _assetNodeInfo.outputPins[1], GetPinColors(PinType_Parameter),
        ImVec2(200, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.simpleParametersNLB, GetPinDrawOffset());
    }

    // ----- String List Box and Pin to access the computed parameters of the asset -----
    if (NodeCollapsingHeader_Out("Computed Parameters", _assetNodeInfo.collapsingHeadersIds[2],
        _assetNodeInfo.utilityState, 2,
        startX, headerWidth, pinWidth,
        _assetNodeInfo.outputPins[2], GetPinColors(PinType_Parameter),
        ImVec2(200, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.computedParametersNLB, GetPinDrawOffset());
    }

    // ----- String List Box and Pin to access the reactions of the asset -----
    if (NodeCollapsingHeader_Out("Reactions", _assetNodeInfo.collapsingHeadersIds[3],
        _assetNodeInfo.utilityState, 3,
        startX, headerWidth, pinWidth,
        _assetNodeInfo.outputPins[3], GetPinColors(PinType_Reaction),
        ImVec2(200, 0), false))
    {
        NodeStringListBox(_assetNodeInfo.reactionsNLB, GetPinDrawOffset());
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();

}

void ECellEngine::Editor::Utility::NodeEditorDraw::SolverNode(const char* _name, const SolverNodeData& _solverNodeInfo)
{
    PushNodeStyle(GetNodeColors(NodeType_Solver));
    ax::NodeEditor::BeginNode(_solverNodeInfo.id);

    const float headerWidth = NodeHeader("Solver:", _name, GetNodeColors(NodeType_Solver));
    const float labelWidth = ImGui::CalcTextSize("Target Asset").x;

    NodeText_Out("Target Asset", labelWidth,
        ImGui::GetCursorPosX(), headerWidth, 8.f, ImGui::GetStyle().ItemSpacing.x,
        _solverNodeInfo.outputPin, GetPinColors(PinType_Asset));

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::SimpleParameterNode(const char* _name, SimpleParameterNodeData& _parameterNodeInfo)
{
    PushNodeStyle(GetNodeColors(NodeType_Parameter));
    ax::NodeEditor::BeginNode(_parameterNodeInfo.id);

    const float headerWidth = NodeHeader("Simple Parameter:", _name, GetNodeColors(NodeType_Parameter));
    const float startX = ImGui::GetCursorPosX();

    if (NodeCollapsingHeader_InOut("Model Links", _parameterNodeInfo.collapsingHeadersIds[0],
        _parameterNodeInfo.utilityState, 0,
        startX, headerWidth, GetMNBVSyle()->pinWidth,
        _parameterNodeInfo.inputPins[0], _parameterNodeInfo.outputPins[0], GetPinColors(PinType_Default),
        ImVec2(200.f, 0.f)))
    {

        NodeText_In("Asset", startX, _parameterNodeInfo.inputPins[1], GetPinColors(PinType_Asset));

        if (NodeCollapsingHeader_InOut("Computed Parameters", _parameterNodeInfo.collapsingHeadersIds[1],
            _parameterNodeInfo.utilityState, 1,
            startX, headerWidth, GetMNBVSyle()->pinWidth,
            _parameterNodeInfo.inputPins[2], _parameterNodeInfo.outputPins[1], GetPinColors(PinType_Parameter),
            ImVec2(200.f, 0.f), false))
        {
            //NodeStringListBox(_parameterNodeInfo.nslbData[0], GetPinDrawOffset(), 200.f, 0);
            ImGui::Text("A List box will be here.");
        }

        if (NodeCollapsingHeader_InOut("Kinetic Laws", _parameterNodeInfo.collapsingHeadersIds[2],
            _parameterNodeInfo.utilityState, 2,
            startX, headerWidth, GetMNBVSyle()->pinWidth,
            _parameterNodeInfo.inputPins[3], _parameterNodeInfo.outputPins[2], GetPinColors(PinType_Parameter),
            ImVec2(200.f, 0.f), false))
        {
            //NodeStringListBox(_parameterNodeInfo.nslbData[0], GetPinDrawOffset(), 200.f, 0);
            ImGui::Text("A List box will be here.");
        }
    }

    if (NodeCollapsingHeader_InOut("Data Fields", _parameterNodeInfo.collapsingHeadersIds[3],
        _parameterNodeInfo.utilityState, 3,
        startX, headerWidth, GetMNBVSyle()->pinWidth,
        _parameterNodeInfo.inputPins[4], _parameterNodeInfo.outputPins[3], GetPinColors(PinType_Default),
        ImVec2(200, 0)))
    {
        const static float inputFieldWidth = ImGui::CalcTextSize("Quantity").x + 100.f + ImGui::GetStyle().ItemSpacing.x; //100.0f is the default length for a field
        float value = _parameterNodeInfo.data->Get();
        if (NodeInputFloat_InOut("Value", _parameterNodeInfo.id.Get(), &value,
            inputFieldWidth, startX, headerWidth, GetMNBVSyle()->pinWidth,
            _parameterNodeInfo.inputPins[5], _parameterNodeInfo.outputPins[4], GetPinColors(PinType_Default)))
        {
            _parameterNodeInfo.data->Set(value);
        }
    }

    ax::NodeEditor::EndNode();
    PopNodeStyle();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::SpeciesNode(const char* _name, SpeciesNodeData& _speciesNodeInfo)
{
    PushNodeStyle(GetNodeColors(NodeType_Species));
    ax::NodeEditor::BeginNode(_speciesNodeInfo.id);
    
    const float headerSize = NodeHeader("Species:", _name, GetNodeColors(NodeType_Species));
    const float startX = ImGui::GetCursorPosX();
    const static float pinWidth = 8.f;

    if (NodeCollapsingHeader_InOut("Model Links", _speciesNodeInfo.collapsingHeadersIds[0],
        _speciesNodeInfo.utilityState, 0,
        startX, headerSize, pinWidth,
        _speciesNodeInfo.inputPins[0], _speciesNodeInfo.outputPins[0], GetPinColors(PinType_Default),
        ImVec2(200, 0)))
    {
        const static float icpTextWidth = ImGui::CalcTextSize("In Computed Parameters").x;
        const static float irTextWidth = ImGui::CalcTextSize("Is Reactant").x;
        const static float ipTextWidth = ImGui::CalcTextSize("Is Product").x;
        const static float iklTextWidth = ImGui::CalcTextSize("In Kinetic Law").x;

        NodeText_In("Asset", startX, _speciesNodeInfo.inputPins[1], GetPinColors(PinType_Asset));

        NodeText_InOut("In Computed Parameters", icpTextWidth,
            startX, headerSize, 8.f,
            _speciesNodeInfo.inputPins[2], _speciesNodeInfo.outputPins[1], GetPinColors(PinType_Parameter));

        NodeText_InOut("Is Reactant", irTextWidth,
            startX, headerSize, 8.f,
            _speciesNodeInfo.inputPins[3], _speciesNodeInfo.outputPins[2], GetPinColors(PinType_Reaction));

        NodeText_InOut("Is Product", ipTextWidth,
            startX, headerSize, 8.f,
            _speciesNodeInfo.inputPins[4], _speciesNodeInfo.outputPins[3], GetPinColors(PinType_Reaction));

        NodeText_InOut("In Kinetic Law", iklTextWidth,
            startX, headerSize, 8.f,
            _speciesNodeInfo.inputPins[5], _speciesNodeInfo.outputPins[4], GetPinColors(PinType_Reaction));
    }

    if (NodeCollapsingHeader_InOut("Data Fields", _speciesNodeInfo.collapsingHeadersIds[1],
        _speciesNodeInfo.utilityState, 1,
        startX, headerSize, pinWidth,
        _speciesNodeInfo.inputPins[6], _speciesNodeInfo.outputPins[5], GetPinColors(PinType_Default),
        ImVec2(200, 0)))
    {
        const static float inputFieldWidth = ImGui::CalcTextSize("Quantity").x + 100.f + ImGui::GetStyle().ItemSpacing.x; //100.0f is the default length for a field
        float value = _speciesNodeInfo.data->Get();
        if (NodeInputFloat_InOut("Quantity", _speciesNodeInfo.id.Get(), &value,
            inputFieldWidth, startX, headerSize, pinWidth,
            _speciesNodeInfo.inputPins[7], _speciesNodeInfo.outputPins[6], GetPinColors(PinType_Default)))
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
    if (ImGui::ItemAdd(bb, 0))
    {
        ax::NodeEditor::PinRect(bb.Min, bb.Max);
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
    if (ImGui::ItemAdd(bb, 0))
    {
        ax::NodeEditor::PinRect(bb.Min, bb.Max);
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
    const float _startX, const float _drawLength, const float _pinWidth,
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

        ImGui::SameLine(); AlignToRight(_startX, _drawLength, _pinWidth);
        OutputPin(_outputPin, _pinColors);
    }

    return open;
}

bool ECellEngine::Editor::Utility::NodeEditorDraw::NodeCollapsingHeader_Out(const char* _label, const std::size_t _id,
    unsigned char& _utilityState, const short _stateBitPos,
    const float _startX, const float _drawLength, const float _pinWidth,
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
        AlignToRight(_startX, _drawLength, _pinWidth);
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
    const ImRect bb(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + _width, ImGui::GetCursorScreenPos().y + _thickness));
    if (ImGui::ItemAdd(bb, 0))
    {
        ImGui::ItemSize(ImVec2(0.0f, _thickness));
        ImGui::GetWindowDrawList()->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y), ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Separator)));
    }
}

bool ECellEngine::Editor::Utility::NodeEditorDraw::NodeInputFloat_InOut(const char* _label, const std::size_t _id, float* valueBuffer,
    const float _inputFieldWidth, const float _startX, const float _drawLength, const float _pinWidth,
    const NodePinData& _inputPin, const NodePinData& _outputPin, const ImVec4 _pinColors[])
{
    ImGui::SetCursorPosX(_startX);

    InputPin(_inputPin, _pinColors); ImGui::SameLine();

    ImGui::PushID(_id);
    ImGui::AlignTextToFramePadding(); AlignToCenter(_startX, _drawLength, _inputFieldWidth);
    
    ImGui::Text(_label); ImGui::SameLine();
    ImGui::SetNextItemWidth(100.f);
    bool edited = ImGui::InputFloat("##quantity", valueBuffer, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsScientific);
    if (ImGui::IsItemActive())
    {
        ax::NodeEditor::Suspend();
        ImGui::SetTooltip("Press ENTER to confirm.");
        ax::NodeEditor::Resume();
    }
    ImGui::PopID();

    ImGui::SameLine();
    
    AlignToRight(_startX, _drawLength, _pinWidth);
    OutputPin(_outputPin, _pinColors);

    return edited;
}

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeStringListBox(NodeListBoxStringData& _lbsData,
    const float _xOffset, const float _widgetWidth, const short _itemViewHeight)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + _xOffset);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f,0.f));
    ImGui::BeginGroup();

    //The list of items to display
    ImGui::BeginGroup();
    const float itemSize = _widgetWidth - style.ScrollbarSize - style.ItemSpacing.x;
    const char* itemString;
    for (int n = _lbsData.cursor; n > _lbsData.cursor - _itemViewHeight; n--)
    {
        itemString = _lbsData.data->at(_lbsData.data->size() - n).c_str();
        if (ImGui::Selectable(itemString, false, ImGuiSelectableFlags_None, ImVec2(itemSize, 0)))
        {
            _lbsData.selectedItem = _lbsData.data->size() - n;
            _lbsData.SetItemClicked();
        }

        if (ImGui::IsItemHovered())
        {
            _lbsData.hoveredItem = _lbsData.data->size() - n;
            _lbsData.SetItemHovered();

            if (ImGui::CalcTextSize(itemString).x > itemSize)
            {
                ax::NodeEditor::Suspend();
                ImGui::SetTooltip("%s", itemString);
                ax::NodeEditor::Resume();
            }

            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                _lbsData.doubleClickedItem = _lbsData.data->size() - n;
                _lbsData.SetItemDoubleClicked();
            }
        }

    }
    ImGui::EndGroup();

    ImGui::SameLine();

    //The vertical slider to control the list of items
    ImGui::PushID(_lbsData.scrollBarID);
    PushScrollBarStyle(style);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
    ImGui::VSliderInt("##VertSliderInt", ImVec2(style.ScrollbarSize, _itemViewHeight * ImGui::GetTextLineHeightWithSpacing()),
                        (int*)&_lbsData.cursor, _itemViewHeight, _lbsData.data->size(), "", ImGuiSliderFlags_None);
    if (ImGui::IsItemHovered())
    {
        _lbsData.SetScrollBarHovered();
    }
    if (ImGui::IsItemActive())
    {
        _lbsData.SetScrollBarActivated();
    }
    ImGui::PopStyleVar(2); //poping ImGuiStyleVar_GrabMinSize & ImGuiStyleVar_ItemSpacing
    PopScrollBarStyle(); //poping after PushScrollBarStyle(style);
    ImGui::PopID();

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
    const float _startX, const float _drawLength, const float _pinWidth,
    const NodePinData& _inputPin, const NodePinData& _outputPin,
    const ImVec4 _pinColors[])
{
    ImGui::SetCursorPosX(_startX);
    InputPin(_inputPin, _pinColors); ImGui::SameLine();

    ImGui::AlignTextToFramePadding();
    AlignToCenter(_startX, _drawLength, _labelWidth);
    ImGui::Text(_label);
    
    ImGui::SameLine();

    AlignToRight(_startX, _drawLength, _pinWidth);
    OutputPin(_outputPin, _pinColors);
}

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeText_Out(const char* _label, const float _labelWidth,
    const float _startX, const float _drawLength, const float _pinWidth, const float itemSpacingX,
    const NodePinData& _pin, const ImVec4 _pinColors[])
{
    AlignToRight(_startX, _drawLength - _pinWidth - itemSpacingX, _labelWidth);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(_label);

    ImGui::SameLine();

    OutputPin(_pin, _pinColors);
}

#pragma endregion