#include "ModelExplorerWidget.hpp"//forward declaration
#include "Editor.hpp"

ECellEngine::Editor::ModelNodeBasedViewerWidget::~ModelNodeBasedViewerWidget()
{
    rootExplorer->RemoveNodeEditorContext(ctxtIndex);
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

    //Create a new Editor context and add it to the list of all Editor contexts managed
    //by the Model Explorer
    rootExplorer->AddNodeEditorContext(ax::NodeEditor::CreateEditor(&nodeConfig));
    ctxtIndex = rootExplorer->CountEditorContexts()-1;

    //Create the style data customized for the nodes specific to our use case.
    rootExplorer->AddNodeEditorStyle(rootExplorer->GetNodeEditorContext(ctxtIndex));
    styleIndex = rootExplorer->CountEditorStyles()-1;

    //Updates global style values that we want to be applied to everything in our use case.
    //We don't use the Push/Pop API on purpose because we will not change those values in the
    //future.
    ax::NodeEditor::SetCurrentEditor(rootExplorer->GetNodeEditorContext(ctxtIndex));
    ax::NodeEditor::Style& style = ax::NodeEditor::GetStyle();
    style.NodeRounding = 6; //instead of 12 by default.
    style.NodeBorderWidth = 3; //instead of 2 by default.
    style.HoveredNodeBorderWidth = 6; //instead of 3 by default.
    style.SelectedNodeBorderWidth = 6; //instead of 3 by default.
    style.PinBorderWidth = 3; //instead of 0 by default.
    style.PinRounding = 0; //instead of 4 by default.
    ax::NodeEditor::SetCurrentEditor(nullptr);

}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::Draw()
{
    if (ImGui::Begin("Model Viewer"))
    {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

        ImGui::Separator();
        ax::NodeEditor::SetCurrentEditor(rootExplorer->GetNodeEditorContext(ctxtIndex));

        // Start interaction with editor.
        ax::NodeEditor::Begin("Model Exploration Space");

        //Checks for incomming Drag & Drop payloads
        //Relevant payloads are the references to assets or solvers loaded in
        //the simulation space.
        HandleSimuDataRefDrop();
        ECellEngine::Editor::Utility::SetCurrentStyle(rootExplorer->GetNodeEditorStyle(styleIndex));
        for (std::vector<ECellEngine::Editor::Utility::AssetNodeData>::iterator it = assetNodes.begin(); it != assetNodes.end(); it++)
        {
            ECellEngine::Editor::Utility::NodeEditorDraw::AssetNode(rootExplorer->GetModelHierarchy()->GetAssetName(it->dataIdx),
                *it);

            if ((*it).speciesNLB.IsAnItemDoubleClicked())
            {
                speciesNodes.push_back(ECellEngine::Editor::Utility::SpeciesNodeData(
                    uniqueIdx, (*it).speciesNLB.doubleClickedItem,
                    rootExplorer->GetDataState()->GetSpecies((*it).speciesNLB.data->at((*it).speciesNLB.doubleClickedItem))));

                links.push_back(ECellEngine::Editor::Utility::LinkData(uniqueIdx, (*it).outputPins[0].id, speciesNodes.back().inputPins[1].id));
                links.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[0].id, 1);
            }

            (*it).speciesNLB.ResetUtilityState();
            (*it).simpleParametersNLB.ResetUtilityState();
            (*it).computedParametersNLB.ResetUtilityState();
            (*it).reactionsNLB.ResetUtilityState();
        }

        for (std::vector<ECellEngine::Editor::Utility::SolverNodeData>::iterator it = solverNodes.begin(); it != solverNodes.end(); it++)
        {
            ECellEngine::Editor::Utility::NodeEditorDraw::SolverNode(rootExplorer->GetModelHierarchy()->GetSolverName(it->dataIdx),
                *it);
        }

        for (std::vector<ECellEngine::Editor::Utility::SpeciesNodeData>::iterator it = speciesNodes.begin(); it != speciesNodes.end(); it++)
        {
            ECellEngine::Editor::Utility::NodeEditorDraw::SpeciesNode((*it).data->name.c_str(), *it);
        }

        // Submit Links
        for (std::vector<ECellEngine::Editor::Utility::LinkData>::iterator it = links.begin(); it != links.end(); it++)
        {
            ECellEngine::Editor::Utility::NodeEditorDraw::Link(*it);
        }

        // End of interaction with editor.
        ax::NodeEditor::End();
        ECellEngine::Editor::Utility::SetCurrentStyle(nullptr);
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