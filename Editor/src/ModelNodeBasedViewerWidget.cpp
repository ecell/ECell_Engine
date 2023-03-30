#include "ModelExplorerWidget.hpp"//forward declaration
#include "Editor.hpp"

ECellEngine::Editor::ModelNodeBasedViewerWidget::~ModelNodeBasedViewerWidget()
{
    rootExplorer->RemoveNodeEditorContext(ctxIndex);
}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::AddAssetNode(const std::size_t _dataIdx)
{
    assetNodes.push_back(ECellEngine::Editor::Utility::AssetNodeData(
        uniqueIdx, _dataIdx, editor.engine.GetSimulationsManager()->GetSimulation(0)->GetModule(_dataIdx).get()));
}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::AddSolverNode(const std::size_t _dataIdx)
{
    solverNodes.push_back(ECellEngine::Editor::Utility::SolverNodeData(
        uniqueIdx, _dataIdx, editor.engine.GetSimulationsManager()->GetSimulation(0)->GetSolver(_dataIdx).get()));
}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::Awake()
{
    ax::NodeEditor::Config nodeConfig;
    rootExplorer->AddNodeEditorContext(ax::NodeEditor::CreateEditor(&nodeConfig));
    ctxIndex = rootExplorer->CountEditorContexts()-1;
}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::Draw()
{
    if (ImGui::Begin("Model Viewer"))
    {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

        ImGui::Separator();
        ax::NodeEditor::SetCurrentEditor(rootExplorer->GetNodeEditorContext(ctxIndex));

        // Start interaction with editor.
        ax::NodeEditor::Begin("Model Exploration Space", ImVec2(0.0, 0.0f));

        //Checks for incomming Drag & Drop payloads
        //Relevant payloads are the references to assets or solvers loaded in
        //the simulation space.
        HandleSimuDataRefDrop();

        for (std::vector<ECellEngine::Editor::Utility::AssetNodeData>::iterator it = assetNodes.begin(); it != assetNodes.end(); it++)
        {
            ECellEngine::Editor::Utility::NodeEditorDraw::AssetNode(rootExplorer->GetModelHierarchy()->GetAssetName(it->dataIdx),
                *it);
        }

        for (std::vector<ECellEngine::Editor::Utility::SolverNodeData>::iterator it = solverNodes.begin(); it != solverNodes.end(); it++)
        {
            ECellEngine::Editor::Utility::NodeEditorDraw::SolverNode(rootExplorer->GetModelHierarchy()->GetSolverName(it->dataIdx),
                *it);
        }

        // Submit Links
        for (ECellEngine::Editor::Utility::LinkData& linkInfo : links)
        {
            ax::NodeEditor::Link(linkInfo.id, linkInfo.inputId, linkInfo.outputId);
        }

        // End of interaction with editor.
        ax::NodeEditor::End();
        ax::NodeEditor::SetCurrentEditor(nullptr);

        ImGui::End();
    }
    
}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::HandleSimuDataRefDrop()
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_ASSET"))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::size_t));

            AddAssetNode(*(const std::size_t*)payload->Data);
        }

        if(const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("DND_SOLVER"))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::size_t));

            AddSolverNode(*(const std::size_t*)payload->Data);
        }

        ImGui::EndDragDropTarget();
    }
}