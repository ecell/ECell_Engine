#include "NodeEditorDraw.hpp"

#pragma region Nodes
void ECellEngine::Editor::Utility::NodeEditorDraw::AssetNode(const char* _name, AssetNodeData& _assetNodeInfo,
    const ImVec4 _assetNodeColors[], const ImVec4 _solverPinColors[],
    const ImVec4 _parameterPinColors[],
    const ImVec4 _reactionPinColors[], const ImVec4 _speciesPinColors[])
{
    PushNodeStyle(_assetNodeColors);
    ax::NodeEditor::BeginNode(_assetNodeInfo.id);

    NodeHeader("Asset:", _name, _assetNodeColors);

    // ----- Pin and Text to connect the solver to the asset -----
    ECellEngine::Editor::Utility::NodeEditorDraw::InputPin(_assetNodeInfo.inputPin, _solverPinColors);
    ImGui::SameLine(); ImGui::AlignTextToFramePadding(); ImGui::Text("Solver");

    // ----- String List Box and Pin to access the species of the asset -----
    ImGui::BeginGroup(); ApplyPinDrawOffset();
    if (NodeCollapsingHeader("Species", _assetNodeInfo.utilityState, 0, ImVec2(200, 0)))
    {
        NodeStringListBox("1", _assetNodeInfo.speciesNLB, GetPinDrawOffset());
    }
    ImGui::EndGroup(); ImGui::SameLine();
    ECellEngine::Editor::Utility::NodeEditorDraw::OutputPin(_assetNodeInfo.outputPins[0], _speciesPinColors);

    // ----- String List Box and Pin to access the simple parameters of the asset -----
    ImGui::BeginGroup(); ApplyPinDrawOffset();
    if (NodeCollapsingHeader("Constant Parameters", _assetNodeInfo.utilityState, 1, ImVec2(200, 0)))
    {
        NodeStringListBox("2", _assetNodeInfo.simpleParametersNLB);
    }
    ImGui::EndGroup(); ImGui::SameLine();
    ECellEngine::Editor::Utility::NodeEditorDraw::OutputPin(_assetNodeInfo.outputPins[1], _parameterPinColors);

    // ----- String List Box and Pin to access the computed parameters of the asset -----
    ImGui::BeginGroup(); ApplyPinDrawOffset();
    if (NodeCollapsingHeader("Computed Parameters", _assetNodeInfo.utilityState, 2, ImVec2(200, 0)))
    {
        NodeStringListBox("3", _assetNodeInfo.computedParametersNLB);
    }
    ImGui::EndGroup(); ImGui::SameLine();
    ECellEngine::Editor::Utility::NodeEditorDraw::OutputPin(_assetNodeInfo.outputPins[2], _parameterPinColors);

    // ----- String List Box and Pin to access the reactions of the asset -----
    ImGui::BeginGroup(); ApplyPinDrawOffset();
    if (NodeCollapsingHeader("Reactions", _assetNodeInfo.utilityState, 3, ImVec2(200, 0)))
    {
        NodeStringListBox("4", _assetNodeInfo.reactionsNLB);
    }
    ImGui::EndGroup(); ImGui::SameLine();
    ECellEngine::Editor::Utility::NodeEditorDraw::OutputPin(_assetNodeInfo.outputPins[3], _reactionPinColors);

    ax::NodeEditor::EndNode();
    PopNodeStyle();

}

void ECellEngine::Editor::Utility::NodeEditorDraw::SolverNode(const char* _name, const SolverNodeData& _solverNodeInfo)
{

    ax::NodeEditor::BeginNode(_solverNodeInfo.id);

    ImGui::Text(_name);

    ECellEngine::Editor::Utility::NodeEditorDraw::BeginColumn();

    ECellEngine::Editor::Utility::NodeEditorDraw::NextColumn();

    for (std::size_t i = 0; i < _solverNodeInfo.outputPins.size(); i++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::OutputPin(_solverNodeInfo.outputPins[i]);
    }

    ECellEngine::Editor::Utility::NodeEditorDraw::EndColumn();

    ax::NodeEditor::EndNode();
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
        PinStyleColor bgColor = PinStyleColor_BgInactivated;
        if (_pinData.isUsed || ImGui::IsItemHovered())
        {
            bgColor = PinStyleColor_BgActivated;
        }

        //Drawing the center of the pin
        ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, ImColor(_pinColors[bgColor]));
        //Going back to start position
        ImGui::SetCursorPos(startPos);
        //Drawing the border of the pin on top of the center.
        ImGui::GetWindowDrawList()->AddRect(bb.Min, bb.Max, ImColor(_pinColors[PinStyleColor_Border]));

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
        PinStyleColor bgColor = PinStyleColor_BgInactivated;
        if (_pinData.isUsed || ImGui::IsItemHovered())
        {
            bgColor = PinStyleColor_BgActivated;
        }

        //Drawing the center of the pin
        ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, ImColor(_pinColors[bgColor]));
        //Going back to start position
        ImGui::SetCursorPos(startPos);
        //Drawing the border of the pin on top of the center.
        ImGui::GetWindowDrawList()->AddRect(bb.Min, bb.Max, ImColor(_pinColors[PinStyleColor_Border]));

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
                    ax::NodeEditor::Link(_links.back().id, _links.back().startId, _links.back().endId);
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
#pragma endregion

#pragma region Custom Node Widgets

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

        ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, ImColor(_colorSet[NodeStyleColor_HeaderBg]), ax::NodeEditor::GetStyle().NodeRounding);
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

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeStringListBox(const char* _id, NodeListBoxStringData& _lbsData,
    const float _xOffset, const float _widgetWidth, const short _itemViewHeight)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + _xOffset);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f,0.f));
    ImGui::BeginGroup();

    //The list of items to display
    ImGui::BeginGroup();
    float itemSize = _widgetWidth - style.ScrollbarSize - style.ItemSpacing.x;
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
    ImGui::PushID(_id);
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

#pragma endregion