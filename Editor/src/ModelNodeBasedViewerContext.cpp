#include "ModelNodeBasedViewerContext.hpp"

void ECellEngine::Editor::Utility::ModelNodeBasedViewerContext::Draw(ECellEngine::Data::DataState* _dataState)
{
    ax::NodeEditor::Suspend();
    if (ax::NodeEditor::ShowBackgroundContextMenu())
    {
        ImGui::OpenPopup("Create New Node");
    }

    if (ImGui::BeginPopup("Create New Node"))
    {
        if (ImGui::BeginMenu("Plots"))
        {
            if (ImGui::MenuItem("Line Plot Node"))
            {
                ax::NodeEditor::Resume();
                linePlotNodes.emplace_back(ECellEngine::Editor::Utility::LinePlotNodeData(1024, ImGui::GetIO().MousePos));
                ax::NodeEditor::Suspend();
            }
            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
    ax::NodeEditor::Resume();

    for (std::vector<ECellEngine::Editor::Utility::AssetNodeData>::iterator it = assetNodes.begin(); it != assetNodes.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::AssetNode(it->data->GetName(), *it);

        //If double click on species selectable in the list box, spawn the corresponding species node
        if (it->speciesNLB.IsAnItemDoubleClicked())
        {
           speciesNodes.emplace_back(ECellEngine::Editor::Utility::SpeciesNodeData(
                _dataState->GetSpecies(it->speciesNLB.data->at(it->speciesNLB.doubleClickedItem))));

            links.emplace_back(ECellEngine::Editor::Utility::LinkData(it->outputPins[0].id,speciesNodes.back().inputPins[1].id));
            links.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[0].id, 1);
        }

        //If double click on simple parameter selectable in the list box, spawn the corresponding simple parameter node
        if (it->simpleParametersNLB.IsAnItemDoubleClicked())
        {
           simpleParameterNodes.emplace_back(ECellEngine::Editor::Utility::SimpleParameterNodeData(
                _dataState->GetSimpleParameter(it->simpleParametersNLB.data->at(it->simpleParametersNLB.doubleClickedItem))));

            links.emplace_back(ECellEngine::Editor::Utility::LinkData(it->outputPins[1].id,simpleParameterNodes.back().inputPins[1].id));
            links.back().OverrideEndFallbackPin(simpleParameterNodes.back().inputPins[0].id, 1);
        }

        //If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
        if (it->computedParametersNLB.IsAnItemDoubleClicked())
        {
           computedParameterNodes.emplace_back(ECellEngine::Editor::Utility::ComputedParameterNodeData(
                _dataState->GetComputedParameter(it->computedParametersNLB.data->at(it->computedParametersNLB.doubleClickedItem))));

            links.emplace_back(ECellEngine::Editor::Utility::LinkData(it->outputPins[2].id,computedParameterNodes.back().inputPins[1].id));
            links.back().OverrideEndFallbackPin(computedParameterNodes.back().inputPins[0].id, 1);
        }

        //If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
        if (it->reactionsNLB.IsAnItemDoubleClicked())
        {
           reactionNodes.emplace_back(ECellEngine::Editor::Utility::ReactionNodeData(
                _dataState->GetReaction(it->reactionsNLB.data->at(it->reactionsNLB.doubleClickedItem))));

            links.emplace_back(ECellEngine::Editor::Utility::LinkData(it->outputPins[3].id,reactionNodes.back().inputPins[1].id));
            links.back().OverrideEndFallbackPin(reactionNodes.back().inputPins[0].id, 1);
        }

        it->speciesNLB.ResetUtilityState();
        it->simpleParametersNLB.ResetUtilityState();
        it->computedParametersNLB.ResetUtilityState();
        it->reactionsNLB.ResetUtilityState();
    }
    
    for (std::vector< ECellEngine::Editor::Utility::ComputedParameterNodeData>::iterator it = computedParameterNodes.begin(); it != computedParameterNodes.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::ComputedParameterNode(it->data->name.c_str(), *it);
    }

    for (std::vector<ECellEngine::Editor::Utility::LinePlotNodeData>::iterator it = linePlotNodes.begin(); it != linePlotNodes.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::LinePlotNode(it->name, *it);
    }

    for (std::vector< ECellEngine::Editor::Utility::ReactionNodeData>::iterator it = reactionNodes.begin(); it != reactionNodes.end(); ++it)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::ReactionNode(it->data->name.c_str(), *it);
    }

    for (std::vector< ECellEngine::Editor::Utility::SimpleParameterNodeData>::iterator it = simpleParameterNodes.begin(); it != simpleParameterNodes.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::SimpleParameterNode(it->data->name.c_str(), *it);
    }

    for (std::vector< ECellEngine::Editor::Utility::SolverNodeData>::iterator it = solverNodes.begin(); it != solverNodes.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::SolverNode(it->data->GetName(), *it);
    }

    for (std::vector< ECellEngine::Editor::Utility::SpeciesNodeData>::iterator it = speciesNodes.begin(); it != speciesNodes.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::SpeciesNode(it->data->name.c_str(), *it);
    }

    ECellEngine::Editor::Utility::NodeEditorDraw::LinkCreation(links);

    for (std::vector< ECellEngine::Editor::Utility::LinkData>::iterator it = links.begin(); it != links.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::Link(*it);
    }
}