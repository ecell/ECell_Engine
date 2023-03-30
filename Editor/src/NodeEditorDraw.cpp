#include "NodeEditorDraw.hpp"

#pragma region Nodes

void ECellEngine::Editor::Utility::NodeEditorDraw::AssetNode(const char* _name, AssetNodeData& _assetNodeInfo)
{
    ax::NodeEditor::BeginNode(_assetNodeInfo.id);

    ImGui::Text(_name);

    ECellEngine::Editor::Utility::NodeEditorDraw::BeginColumn();
    if (NodeCollapsingHeader("Species", _assetNodeInfo.utilityState, 0, ImVec2(200, 0)))
    {
        NodeStringListBox("1", _assetNodeInfo.speciesNLB);

        if (_assetNodeInfo.speciesNLB.IsAnItemHovered())
        {
            ax::NodeEditor::Suspend();
            ImGui::SetTooltip("%s", _assetNodeInfo.speciesNLB.data->at(_assetNodeInfo.speciesNLB.hoveredItem).c_str());
            ax::NodeEditor::Resume();
        }
    }
    ECellEngine::Editor::Utility::NodeEditorDraw::NextColumn();
    ECellEngine::Editor::Utility::NodeEditorDraw::OutputPin(_assetNodeInfo.outputPins[0]);
    ECellEngine::Editor::Utility::NodeEditorDraw::EndColumn();

    if (NodeCollapsingHeader("Constant Parameters", _assetNodeInfo.utilityState, 1, ImVec2(200, 0)))
    {
        NodeStringListBox("2", _assetNodeInfo.simpleParametersNLB);
    }

    if (NodeCollapsingHeader("Computed Parameters", _assetNodeInfo.utilityState, 2, ImVec2(200, 0)))
    {
        NodeStringListBox("3", _assetNodeInfo.computedParametersNLB);
    }

    if (NodeCollapsingHeader("Reactions", _assetNodeInfo.utilityState, 3, ImVec2(200, 0)))
    {
        NodeStringListBox("4", _assetNodeInfo.reactionsNLB);
    }

    ax::NodeEditor::EndNode();
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
void ECellEngine::Editor::Utility::NodeEditorDraw::InputPin(const NodePinData& _pinData)
{
    ax::NodeEditor::BeginPin(_pinData.id, ax::NodeEditor::PinKind::Input);
    ImGui::Text("->");
    ImGui::SameLine();
    ImGui::Text(_pinData.name);
    ax::NodeEditor::EndPin();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::OutputPin(const NodePinData& _pinData)
{
    ax::NodeEditor::BeginPin(_pinData.id, ax::NodeEditor::PinKind::Output);
    ImGui::Text(_pinData.name);
    ImGui::SameLine();
    ImGui::Text("->");
    ax::NodeEditor::EndPin();
}



void ECellEngine::Editor::Utility::NodeEditorDraw::LinkCreation(int& _id, std::vector<ECellEngine::Editor::Utility::LinkData>& _links)
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
                    _links.push_back({ ax::NodeEditor::LinkId(_id++), inputPinId, outputPinId });

                    // Draw new link.
                    ax::NodeEditor::Link(_links.back().id, _links.back().inputId, _links.back().outputId);
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

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeHeader(const char* _type, const char* _name,
    const float _width, const short _height)
{
    const ImVec2 typeTextSize = ImGui::CalcTextSize(_type);
    const ImVec2 nameTextSize = ImGui::CalcTextSize(_name);

    ImGui::Text(_type); ImGui::SameLine(); ImGui::Text(_name);

    NodeHorizontalSeparator(typeTextSize.x + nameTextSize.x + ImGui::GetStyle().ItemSpacing.x);
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
    const float _widgetWidth, const short _itemViewHeight)
{
    ImGui::BeginGroup();

    //The list of items to display
    ImGui::BeginGroup();
    for (int n = _lbsData.cursor; n > _lbsData.cursor - _itemViewHeight; n--)
    {
        if (ImGui::Selectable(_lbsData.data->at(_lbsData.data->size() - n).c_str(), false, ImGuiSelectableFlags_None, ImVec2(_widgetWidth - 20, 0)))
        {
            _lbsData.selectedItem = _lbsData.data->size() - n;
            _lbsData.SetItemClicked();
        }

        if (ImGui::IsItemHovered())
        {
            _lbsData.hoveredItem = _lbsData.data->size() - n;
            _lbsData.SetItemHovered();

            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                //ECellEngine::Logging::Logger::GetSingleton().LogDebug("Item: " + _lbsData.data->at(_lbsData.data->size() - n) + " was double clicked!");
                _lbsData.doubleClickedItem = _lbsData.data->size() - n;
                _lbsData.SetItemDoubleClicked();
            }
        }

    }
    ImGui::EndGroup();

    ImGui::SameLine();

    //The vertical slider to control the list of items
    ImGui::PushID(_id);
    ImGuiStyle& style = ImGui::GetStyle();
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
    ImGui::PopStyleVar(); //poping ImGuiStyleVar_GrabMinSize
    PopScrollBarStyle(); //poping after PushScrollBarStyle(style);
    ImGui::PopID();

    ImGui::EndGroup();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::PushScrollBarStyle(ImGuiStyle& _style)
{
    //Style the vertical slider like a scroll bar
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, _style.ScrollbarRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, _style.ScrollbarRounding);
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarGrab));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
}

void ECellEngine::Editor::Utility::NodeEditorDraw::PopScrollBarStyle()
{
    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);
}

#pragma endregion