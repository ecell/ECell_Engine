#include "NodeEditorDraw.hpp"

void ECellEngine::Editor::Utility::NodeEditorDraw::AssetNode(const char* _name, const AssetNodeData& _assetNodeInfo)
{
    ax::NodeEditor::BeginNode(_assetNodeInfo.id);

    ImGui::Text(_name);
    ImGui::Separator();

    ECellEngine::Editor::Utility::NodeEditorDraw::BeginColumn();
    for (std::size_t i = 0; i < _assetNodeInfo.inputPins.size(); i++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::InputPin(_assetNodeInfo.inputPins[i]);
    }

    ECellEngine::Editor::Utility::NodeEditorDraw::NextColumn();
    for (std::size_t i = 0; i < _assetNodeInfo.outputPins.size(); i++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::OutputPin(_assetNodeInfo.outputPins[i]);
    }

    ECellEngine::Editor::Utility::NodeEditorDraw::EndColumn();

    ax::NodeEditor::EndNode();
}

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