#include "NodeEditorDraw.hpp"

#pragma region Nodes

void ECellEngine::Editor::Utility::NodeEditorDraw::AssetNode(const char* _name, AssetNodeData& _assetNodeInfo)
{
    ax::NodeEditor::BeginNode(_assetNodeInfo.id);

    ImGui::Text(_name);

    if (NodeCollapsingHeader("Species", _assetNodeInfo.utilityState, 0, ImVec2(200, 0)))
    {
        NodeStringListBox("1", _assetNodeInfo.data->GetAllSpecies(),
            _assetNodeInfo.speciesNLB.selectedItemIdx, _assetNodeInfo.speciesNLB.cursor);
    }

    if (NodeCollapsingHeader("Constant Parameters", _assetNodeInfo.utilityState, 1, ImVec2(200, 0)))
    {
        NodeStringListBox("2", _assetNodeInfo.data->GetAllSimpleParameter(),
            _assetNodeInfo.simpleParametersNLB.selectedItemIdx, _assetNodeInfo.simpleParametersNLB.cursor);
    }

    if (NodeCollapsingHeader("Computed Parameters", _assetNodeInfo.utilityState, 2, ImVec2(200, 0)))
    {
        NodeStringListBox("3", _assetNodeInfo.data->GetAllComputedParameter(),
            _assetNodeInfo.computedParametersNLB.selectedItemIdx, _assetNodeInfo.computedParametersNLB.cursor);
    }

    if (NodeCollapsingHeader("Reactions", _assetNodeInfo.utilityState, 3, ImVec2(200, 0)))
    {
        NodeStringListBox("4", _assetNodeInfo.data->GetAllReaction(),
            _assetNodeInfo.reactionsNLB.selectedItemIdx, _assetNodeInfo.reactionsNLB.cursor);
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

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeScrollBar(const char* _id, std::size_t& _cursor,
    const float _height, const short _min, const short _max)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushID(_id);

    //Style the vertical slider like a scroll bar
    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.ScrollbarRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, style.ScrollbarRounding);
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarGrab));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarBg));

    ImGui::VSliderInt("##VertSliderInt", ImVec2(style.ScrollbarSize, _height),
        (int*)&_cursor, _min, _max, "", ImGuiSliderFlags_None);
    
    //usefull only if the list is sorted
    /*if (ImGui::IsItemActive())
    {
        ax::NodeEditor::Suspend();
        ImGui::SetTooltip("%X - %X", _items[_items.size() - _cursor][0], _items[_items.size() - _cursor + _itemViewHeight - 1][0]);
        ax::NodeEditor::Resume();
    }*/

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(3);
    ImGui::PopID();
}

void ECellEngine::Editor::Utility::NodeEditorDraw::NodeStringListBox(const char* _id, const std::vector<std::string>& _items,
    std::size_t& _selectedItemIdx, std::size_t& _cursor,
    const float _widgetWidth, const short _itemViewHeight)
{
    ImGui::BeginGroup();
    
    //The list of items to display
    ImGui::BeginGroup();
    for (int n = _cursor; n > _cursor - _itemViewHeight; n--)
    {
        const bool is_selected = (_selectedItemIdx == _items.size() - n);
        if (ImGui::Selectable(_items.at(_items.size() - n).c_str(), is_selected, ImGuiSelectableFlags_None, ImVec2(_widgetWidth - 20, 0)))
        {
            _selectedItemIdx = _items.size() - n;
        }
    }
    ImGui::EndGroup();

    ImGui::SameLine();

    //The vertical slider to control the list of items
    ImGui::BeginGroup();
    NodeScrollBar(_id, _cursor, _itemViewHeight * ImGui::GetTextLineHeightWithSpacing(), _itemViewHeight, _items.size());
    ImGui::EndGroup();

    ImGui::EndGroup();
}

#pragma endregion