#include "Widget/ModelExplorerWidget.hpp"//forward declaration
#include "Editor.hpp"

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::Awake()
{
    //Updates global style values that we want to be applied to every MNBV by default.
    ax::NodeEditor::SetCurrentEditor(editorCtxt);

    ax::NodeEditor::Style& axStyle = ax::NodeEditor::GetStyle();
    Style::NodeEditorStyle& neStyle = Style::EditorStyle::GetMNBVStyle();
    axStyle.NodeRounding = neStyle.nodeRounding;
    axStyle.NodeBorderWidth = neStyle.nodeBorderWidth;
    axStyle.HoveredNodeBorderWidth = neStyle.hoveredNodeBorderWidth;
    axStyle.SelectedNodeBorderWidth = neStyle.selectedNodeBorderWidth;
    axStyle.PinBorderWidth = neStyle.pinBorderWidth;
    axStyle.PinRounding = neStyle.pinRounding;

    ax::NodeEditor::SetCurrentEditor(nullptr);
}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::Draw()
{
    if (ImGui::Begin("Model Viewer"))
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
        ImGui::Separator();

        ax::NodeEditor::SetCurrentEditor(editorCtxt);
        //SetCurrentMNBVContext(rootExplorer->GetModelNodeBasedViewerContext(mnbvCtxIdx));

        // Start interaction with editor.
        ax::NodeEditor::Begin("Model Exploration Space");

        //Checks for incomming Drag & Drop payloads
        //Relevant payloads are the references to assets or solvers loaded in
        //the simulation space.
        HandleSimuDataRefDrop();

        CurrentMNBVContextDraw(ECellEngine::Core::SimulationsManager::GetSingleton().GetSimulation(0));

        // End of interaction with editor.
        ax::NodeEditor::End();
        //SetCurrentMNBVContext(nullptr);
        ax::NodeEditor::SetCurrentEditor(nullptr);

    }
    ImGui::End();
}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::HandleSimuDataRefDrop()
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_ASSET"))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::size_t));
            const std::size_t dataIdx = *(const std::size_t*)payload->Data;
            AddAssetNode(ECellEngine::Core::SimulationsManager::GetSingleton().GetSimulation(0)->GetModule(dataIdx).get());
        }

        ImGui::EndDragDropTarget();
    }
}