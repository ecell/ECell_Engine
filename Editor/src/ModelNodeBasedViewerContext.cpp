#include "ModelNodeBasedViewerContext.hpp"

void ECellEngine::Editor::Utility::ModelNodeBasedViewerContext::Draw(ECellEngine::Core::Simulation* _simulation)
{
    ax::NodeEditor::Suspend();
    if (ax::NodeEditor::ShowBackgroundContextMenu())
    {
        ImGui::OpenPopup("Create New Node");
    }

    if (ImGui::BeginPopup("Create New Node"))
    {
        if (ImGui::BeginMenu("Data"))
        {
            if (ImGui::MenuItem("Value Float Node"))
            {
                ax::NodeEditor::Resume();
                valueFloatNodes.emplace_back(ECellEngine::Editor::Utility::ValueFloatNodeData(0.f, ImGui::GetIO().MousePos));
                ax::NodeEditor::Suspend();
            }
            ImGui::EndMenu();
        }
        
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

        if (ImGui::BeginMenu("Time"))
        {
            if (ImGui::MenuItem("Simulation Elapsed Time"))
            {
                ax::NodeEditor::Resume();
                simulationTimeNodes.emplace_back(ECellEngine::Editor::Utility::SimulationTimeNodeData(_simulation->GetTimer(), ImGui::GetIO().MousePos));
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
        if (it->nlbsData[AssetNodeData::NodeListBoxString_Species].IsAnItemDoubleClicked())
        {
           speciesNodes.emplace_back(ECellEngine::Editor::Utility::SpeciesNodeData(
                _simulation->GetDataState()->GetSpecies(it->nlbsData[AssetNodeData::NodeListBoxString_Species].data->at(it->nlbsData[AssetNodeData::NodeListBoxString_Species].doubleClickedItem))));

            links.emplace_back(ECellEngine::Editor::Utility::LinkData(it->outputPins[AssetNodeData::OutputPin_CollHdrSpecies].id,speciesNodes.back().inputPins[1].id));
            links.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[0].id, 1);
        }

        //If double click on simple parameter selectable in the list box, spawn the corresponding simple parameter node
        if (it->nlbsData[AssetNodeData::NodeListBoxString_SimpleParameters].IsAnItemDoubleClicked())
        {
            simpleParameterNodes.emplace_back(ECellEngine::Editor::Utility::SimpleParameterNodeData(
                _simulation->GetDataState()->GetSimpleParameter(it->nlbsData[AssetNodeData::NodeListBoxString_SimpleParameters].data->at(it->nlbsData[AssetNodeData::NodeListBoxString_SimpleParameters].doubleClickedItem))));

            links.emplace_back(ECellEngine::Editor::Utility::LinkData(it->outputPins[AssetNodeData::OutputPin_CollHdrSimpleParameters].id, simpleParameterNodes.back().inputPins[SimpleParameterNodeData::InputPin_Asset].id));
            links.back().OverrideEndFallbackPin(simpleParameterNodes.back().inputPins[SimpleParameterNodeData::CollapsingHeader_ModelLinks].id, 1);
        }

        //If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
        if (it->nlbsData[AssetNodeData::NodeListBoxString_ComputedParameters].IsAnItemDoubleClicked())
        {
            computedParameterNodes.emplace_back(ECellEngine::Editor::Utility::ComputedParameterNodeData(
                _simulation->GetDataState()->GetComputedParameter(it->nlbsData[AssetNodeData::NodeListBoxString_ComputedParameters].data->at(it->nlbsData[AssetNodeData::NodeListBoxString_ComputedParameters].doubleClickedItem))));

            links.emplace_back(ECellEngine::Editor::Utility::LinkData(it->outputPins[AssetNodeData::OutputPin_CollHdrComputedParameters].id, computedParameterNodes.back().inputPins[ComputedParameterNodeData::InputPin_Asset].id));
            links.back().OverrideEndFallbackPin(computedParameterNodes.back().inputPins[ComputedParameterNodeData::InputPin_CollHdrModelLinks].id, 1);
        }

        //If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
        if (it->nlbsData[AssetNodeData::NodeListBoxString_Reactions].IsAnItemDoubleClicked())
        {
            reactionNodes.emplace_back(ECellEngine::Editor::Utility::ReactionNodeData(
                _simulation->GetDataState()->GetReaction(it->nlbsData[AssetNodeData::NodeListBoxString_Reactions].data->at(it->nlbsData[AssetNodeData::NodeListBoxString_Reactions].doubleClickedItem))));

            links.emplace_back(ECellEngine::Editor::Utility::LinkData(it->outputPins[AssetNodeData::OutputPin_CollHdrReactions].id, reactionNodes.back().inputPins[ReactionNodeData::InputPin_Asset].id));
            links.back().OverrideEndFallbackPin(reactionNodes.back().inputPins[ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);
        }

        it->nlbsData[AssetNodeData::NodeListBoxString_Species].ResetUtilityState();
        it->nlbsData[AssetNodeData::NodeListBoxString_SimpleParameters].ResetUtilityState();
        it->nlbsData[AssetNodeData::NodeListBoxString_ComputedParameters].ResetUtilityState();
        it->nlbsData[AssetNodeData::NodeListBoxString_Reactions].ResetUtilityState();
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

    for (std::vector< ECellEngine::Editor::Utility::SimulationTimeNodeData>::iterator it = simulationTimeNodes.begin(); it != simulationTimeNodes.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::SimulationTimeNode("Simulation Elapsed Time", *it);
    }

    for (std::vector< ECellEngine::Editor::Utility::SolverNodeData>::iterator it = solverNodes.begin(); it != solverNodes.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::SolverNode(it->data->GetName(), *it);
    }

    for (std::vector< ECellEngine::Editor::Utility::SpeciesNodeData>::iterator it = speciesNodes.begin(); it != speciesNodes.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::SpeciesNode(it->data->name.c_str(), *it);
    }
    
    for (std::vector< ECellEngine::Editor::Utility::ValueFloatNodeData>::iterator it = valueFloatNodes.begin(); it != valueFloatNodes.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::ValueFloatNode("Float", *it);
    }

    ECellEngine::Editor::Utility::NodeEditorDraw::LinkCreation(links);

    for (std::vector< ECellEngine::Editor::Utility::LinkData>::iterator it = links.begin(); it != links.end(); it++)
    {
        ECellEngine::Editor::Utility::NodeEditorDraw::Link(*it);
    }
}