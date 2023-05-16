#include "Widget/ModelExplorerWidget.hpp"//forward declaration
#include "Editor.hpp"

ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::~ModelNodeBasedViewerWidget()
{
    rootExplorer->RemoveNodeEditorContext(neCtxtIdx);
}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::Awake()
{
    ax::NodeEditor::Config nodeConfig;

    //Create a new Editor context and add it to the list of all Editor contexts managed
    //by the Model Explorer.
    rootExplorer->AddNodeEditorContext(ax::NodeEditor::CreateEditor(&nodeConfig));
    neCtxtIdx = rootExplorer->CountEditorContexts() - 1;

    //Create the style data customized for the nodes specific to our use case.
    rootExplorer->AddModelNodeBasedViewerContext();
    mnbvCtxIdx = rootExplorer->CountModelNodeBasedViewerContext() - 1;

    //Updates global style values that we want to be applied to everything in our use case.
    //We don't use the Push/Pop API on purpose because we will not change those values in the
    //future.
    ax::NodeEditor::SetCurrentEditor(rootExplorer->GetNodeEditorContext(neCtxtIdx));
    ax::NodeEditor::Style& style = ax::NodeEditor::GetStyle();
    style.NodeRounding = 6; //instead of 12 by default.
    style.NodeBorderWidth = 3; //instead of 2 by default.
    style.HoveredNodeBorderWidth = 6; //instead of 3 by default.
    style.SelectedNodeBorderWidth = 6; //instead of 3 by default.
    style.PinBorderWidth = 3; //instead of 0 by default.
    style.PinRounding = 0; //instead of 4 by default.

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

    ax::NodeEditor::SetCurrentEditor(nullptr);
}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::Draw()
{
    if (ImGui::Begin("Model Viewer"))
    {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

        ImGui::Separator();
        ax::NodeEditor::SetCurrentEditor(rootExplorer->GetNodeEditorContext(neCtxtIdx));
        SetCurrentMNBVContext(rootExplorer->GetModelNodeBasedViewerContext(mnbvCtxIdx));

        // Start interaction with editor.
        ax::NodeEditor::Begin("Model Exploration Space");

        //Checks for incomming Drag & Drop payloads
        //Relevant payloads are the references to assets or solvers loaded in
        //the simulation space.
        HandleSimuDataRefDrop();

        CurrentMNBVContextDraw(editor.engine.GetSimulationsManager()->GetSimulation(0));

        SendEngineTASToMCmd("0", editor.engine.GetCommandsManager());

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
            AddAssetNode(editor.engine.GetSimulationsManager()->GetSimulation(0)->GetModule(dataIdx).get());
        }

        if(const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("DND_SOLVER"))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::size_t));
            const std::size_t dataIdx = *(const std::size_t*)payload->Data;
            AddSolverNode(editor.engine.GetSimulationsManager()->GetSimulation(0)->GetSolver(dataIdx).get());
        }

        ImGui::EndDragDropTarget();
    }
}