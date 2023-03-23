#include "ModelExplorerWidget.hpp"//forward declaration

void ECellEngine::Editor::ModelNodeBasedViewerWidget::AddAssetNode(const char* _name)
{
    assetNodes.push_back(ECellEngine::Editor::Utility::AssetNodeInfo(uniqueIdx, _name));

}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::Awake()
{
    ax::NodeEditor::Config nodeConfig;
    nodeEditorCtxt = ax::NodeEditor::CreateEditor(&nodeConfig);
}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::Draw()
{
    if (ImGui::Begin("Model Viewer"))
    {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

        ImGui::Separator();

        ax::NodeEditor::SetCurrentEditor(nodeEditorCtxt);

        // Start interaction with editor.
        ax::NodeEditor::Begin("Model Exploration Space", ImVec2(0.0, 0.0f));

        // Submit Links
        for (ECellEngine::Editor::Utility::LinkInfo& linkInfo : links)
        {
            ax::NodeEditor::Link(linkInfo.id, linkInfo.inputId, linkInfo.outputId);
        }

        // End of interaction with editor.
        ax::NodeEditor::End();
        ax::NodeEditor::SetCurrentEditor(nullptr);

        ImGui::End();
    }
    
}