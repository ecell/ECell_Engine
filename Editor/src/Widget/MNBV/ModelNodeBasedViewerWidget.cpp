#include "Widget/ModelExplorerWidget.hpp"//forward declaration
#include "Editor.hpp"

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::Awake()
{
    //Create the style data customized for the nodes specific to our use case.
    rootExplorer->AddModelNodeBasedViewerContext();
    mnbvCtxIdx = rootExplorer->CountModelNodeBasedViewerContext() - 1;

    //BUGFIX: Cycle through the canvas once to give it the opportunity to initialize its size.
    //This is work-around for a badly understood bug.
    //If the ax::NodeEditor::Begin()/ax::NodeEditor::End is not implemented in this Awake function,
    //the ax::NodeEditor::Begin("Model Exploration Space") in the ECellEngine::Editor::Widget::ModelNodeBasedViewerWidget::Draw()
    //fails. It apparently comes from the initialization cycle (Begin/End call) for the ImGuiEx::Canvas in
    //ax::NodeEditor::EditorContext::Begin(). The Begin of the Canvas exits early because the Canvas is detected to be clipped.
    //So, as Begin didn't finish, the ASSERT in the End() of the Canvas that checks whether Begin finished correctly is broken.
    //Strangely, calling ax::NodeEditor::Begin()/ax::NodeEditor::End here (Awake) doesn't raise any issue and even fixes the issue
    //in the Draw function.
    ax::NodeEditor::Begin("Model Exploration Space");
    ax::NodeEditor::End();
}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::Draw()
{
    if (ImGui::Begin("Model Viewer"))
    {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

        ImGui::Separator();
        ax::NodeEditor::SetCurrentEditor(editorCtxt);
        SetCurrentMNBVContext(rootExplorer->GetModelNodeBasedViewerContext(mnbvCtxIdx));

        // Start interaction with editor.
        ax::NodeEditor::Begin("Model Exploration Space");

        //Checks for incomming Drag & Drop payloads
        //Relevant payloads are the references to assets or solvers loaded in
        //the simulation space.
        HandleSimuDataRefDrop();

        CurrentMNBVContextDraw(ECellEngine::Core::SimulationsManager::GetSingleton().GetSimulation(0));

        // End of interaction with editor.
        ax::NodeEditor::End();
        SetCurrentMNBVContext(nullptr);
        ax::NodeEditor::SetCurrentEditor(nullptr);

        ImGui::End();
    }
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