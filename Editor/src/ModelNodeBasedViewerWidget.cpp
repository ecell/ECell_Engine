#include "ModelExplorerWidget.hpp"//forward declaration
#include "Editor.hpp"

ECellEngine::Editor::ModelNodeBasedViewerWidget::~ModelNodeBasedViewerWidget()
{
    rootExplorer->RemoveNodeEditorContext(neCtxtIdx);
}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::AddAssetNode(const std::size_t _dataIdx)
{
    assetNodes.push_back(ECellEngine::Editor::Utility::AssetNodeData(
        ECellEngine::Editor::Utility::GetMNBVCtxtNextId(), _dataIdx, editor.engine.GetSimulationsManager()->GetSimulation(0)->GetModule(_dataIdx).get()));
}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::AddSolverNode(const std::size_t _dataIdx)
{
    solverNodes.push_back(ECellEngine::Editor::Utility::SolverNodeData(
        ECellEngine::Editor::Utility::GetMNBVCtxtNextId(), _dataIdx, editor.engine.GetSimulationsManager()->GetSimulation(0)->GetSolver(_dataIdx).get()));
}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::Awake()
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
    ax::NodeEditor::SetCurrentEditor(nullptr);

}

void ECellEngine::Editor::ModelNodeBasedViewerWidget::Draw()
{
    if (ImGui::Begin("Model Viewer"))
    {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

        ImGui::Separator();
        ax::NodeEditor::SetCurrentEditor(rootExplorer->GetNodeEditorContext(neCtxtIdx));
        ECellEngine::Editor::Utility::SetCurrentMNBVContext(rootExplorer->GetModelNodeBasedViewerContext(mnbvCtxIdx));

        // Start interaction with editor.
        ax::NodeEditor::Begin("Model Exploration Space");

        //Checks for incomming Drag & Drop payloads
        //Relevant payloads are the references to assets or solvers loaded in
        //the simulation space.
        HandleSimuDataRefDrop();
        for (std::vector<ECellEngine::Editor::Utility::AssetNodeData>::iterator it = assetNodes.begin(); it != assetNodes.end(); it++)
        {
            ECellEngine::Editor::Utility::NodeEditorDraw::AssetNode(rootExplorer->GetModelHierarchy()->GetAssetName(it->dataIdx),
                *it);

            //If double click on species selectable in the list box, spawn the corresponding species node
            if (it->speciesNLB.IsAnItemDoubleClicked())
            {
                speciesNodes.emplace_back(ECellEngine::Editor::Utility::SpeciesNodeData(
                    ECellEngine::Editor::Utility::GetMNBVCtxtNextId(), it->speciesNLB.doubleClickedItem,
                    rootExplorer->GetDataState()->GetSpecies(it->speciesNLB.data->at(it->speciesNLB.doubleClickedItem))));

                links.emplace_back(ECellEngine::Editor::Utility::LinkData(ECellEngine::Editor::Utility::GetMNBVCtxtNextId(), it->outputPins[0].id, speciesNodes.back().inputPins[1].id));
                links.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[0].id, 1);
            }

            //If double click on simple parameter selectable in the list box, spawn the corresponding simple parameter node
            if (it->simpleParametersNLB.IsAnItemDoubleClicked())
            {
                simpleParameterNodes.emplace_back(ECellEngine::Editor::Utility::SimpleParameterNodeData(
                    ECellEngine::Editor::Utility::GetMNBVCtxtNextId(), it->simpleParametersNLB.doubleClickedItem,
                    rootExplorer->GetDataState()->GetSimpleParameter(it->simpleParametersNLB.data->at(it->simpleParametersNLB.doubleClickedItem))));

                links.emplace_back(ECellEngine::Editor::Utility::LinkData(ECellEngine::Editor::Utility::GetMNBVCtxtNextId(), it->outputPins[1].id, simpleParameterNodes.back().inputPins[1].id));
                links.back().OverrideEndFallbackPin(simpleParameterNodes.back().inputPins[0].id, 1);
            }

            //If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
            if (it->computedParametersNLB.IsAnItemDoubleClicked())
            {
                computedParameterNodes.emplace_back(ECellEngine::Editor::Utility::ComputedParameterNodeData(
                    ECellEngine::Editor::Utility::GetMNBVCtxtNextId(), it->computedParametersNLB.doubleClickedItem,
                    rootExplorer->GetDataState()->GetComputedParameter(it->computedParametersNLB.data->at(it->computedParametersNLB.doubleClickedItem))));

                links.emplace_back(ECellEngine::Editor::Utility::LinkData(ECellEngine::Editor::Utility::GetMNBVCtxtNextId(), it->outputPins[2].id, computedParameterNodes.back().inputPins[1].id));
                links.back().OverrideEndFallbackPin(computedParameterNodes.back().inputPins[0].id, 1);
            }

            it->speciesNLB.ResetUtilityState();
            it->simpleParametersNLB.ResetUtilityState();
            it->computedParametersNLB.ResetUtilityState();
            it->reactionsNLB.ResetUtilityState();
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

        for (std::vector<ECellEngine::Editor::Utility::ComputedParameterNodeData>::iterator it = computedParameterNodes.begin(); it != computedParameterNodes.end(); it++)
        {
            ECellEngine::Editor::Utility::NodeEditorDraw::ComputedParameterNode((*it).data->name.c_str(), *it);
        }

        for (std::vector<ECellEngine::Editor::Utility::SimpleParameterNodeData>::iterator it = simpleParameterNodes.begin(); it != simpleParameterNodes.end(); it++)
        {
            ECellEngine::Editor::Utility::NodeEditorDraw::SimpleParameterNode((*it).data->name.c_str(), *it);
        }

        // Submit Links
        for (std::vector<ECellEngine::Editor::Utility::LinkData>::iterator it = links.begin(); it != links.end(); it++)
        {
            ECellEngine::Editor::Utility::NodeEditorDraw::Link(*it);
        }

        // End of interaction with editor.
        ax::NodeEditor::End();
        ECellEngine::Editor::Utility::SetCurrentMNBVContext(nullptr);
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