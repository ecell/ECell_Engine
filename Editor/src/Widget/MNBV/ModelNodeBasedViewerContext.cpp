#include "Widget/MNBV/ModelNodeBasedViewerContext.hpp"

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::Draw(ECellEngine::Core::Simulation* _simulation)
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
				valueFloatNodes.emplace_back(Utility::MNBV::ValueFloatNodeData(0.f, ImGui::GetIO().MousePos));
				ax::NodeEditor::Suspend();
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Plots"))
		{
			if (ImGui::MenuItem("Line Plot Node"))
			{
				ax::NodeEditor::Resume();
				linePlotNodes.emplace_back(Utility::MNBV::LinePlotNodeData(1024, ImGui::GetIO().MousePos));
				ax::NodeEditor::Suspend();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Time"))
		{
			if (ImGui::MenuItem("Simulation Elapsed Time"))
			{
				ax::NodeEditor::Resume();
				simulationTimeNodes.emplace_back(Utility::MNBV::SimulationTimeNodeData(_simulation->GetTimer(), ImGui::GetIO().MousePos));
				ax::NodeEditor::Suspend();
			}
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
	ax::NodeEditor::Resume();

	for (std::vector<Utility::MNBV::AssetNodeData>::iterator it = assetNodes.begin(); it != assetNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::AssetNode(it->data->GetName(), *it);

		//If double click on species selectable in the list box, spawn the corresponding species node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Species].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = _simulation->GetDataState()->GetSpecies(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Species].GetDoubleClickedItem());
			speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species.get()));

			links.emplace_back(Utility::MNBV::LinkData(it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrSpecies].id,speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_Asset].id));
			links.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);
		}

		//If double click on simple parameter selectable in the list box, spawn the corresponding simple parameter node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_SimpleParameters].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::SimpleParameter> simpleParameter = _simulation->GetDataState()->GetSimpleParameter(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_SimpleParameters].GetDoubleClickedItem());
			simpleParameterNodes.emplace_back(Utility::MNBV::SimpleParameterNodeData(simpleParameter, _simulation->GetDependenciesDatabase()));

			links.emplace_back(Utility::MNBV::LinkData(it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrSimpleParameters].id, simpleParameterNodes.back().inputPins[Utility::MNBV::SimpleParameterNodeData::InputPin_Asset].id));
			links.back().OverrideEndFallbackPin(simpleParameterNodes.back().inputPins[Utility::MNBV::SimpleParameterNodeData::CollapsingHeader_ModelLinks].id, 1);
		}

		//If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_ComputedParameters].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::ComputedParameter> computedParameter = _simulation->GetDataState()->GetComputedParameter(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_ComputedParameters].GetDoubleClickedItem());
			computedParameterNodes.emplace_back(Utility::MNBV::ComputedParameterNodeData(computedParameter, _simulation->GetDependenciesDatabase()));

			links.emplace_back(Utility::MNBV::LinkData(it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrComputedParameters].id, computedParameterNodes.back().inputPins[Utility::MNBV::ComputedParameterNodeData::InputPin_Asset].id));
			links.back().OverrideEndFallbackPin(computedParameterNodes.back().inputPins[Utility::MNBV::ComputedParameterNodeData::InputPin_CollHdrModelLinks].id, 1);
		}

		//If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Reactions].IsAnItemDoubleClicked())
		{
			reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(
				_simulation->GetDataState()->GetReaction(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Reactions].GetDoubleClickedItem()).get()));

			links.emplace_back(Utility::MNBV::LinkData(it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrReactions].id, reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_Asset].id));
			links.back().OverrideEndFallbackPin(reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);
		}

		it->ResetNLBSDUtilityStates();
	}
	
	for (std::vector< Utility::MNBV::ComputedParameterNodeData>::iterator it = computedParameterNodes.begin(); it != computedParameterNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::ComputedParameterNode(it->data->name.c_str(), *it);
		it->ResetNLBSDUtilityStates();
	}

	for (std::vector<Utility::MNBV::LinePlotNodeData>::iterator it = linePlotNodes.begin(); it != linePlotNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::LinePlotNode(it->name, *it);
	}

	for (std::vector< Utility::MNBV::ReactionNodeData>::iterator it = reactionNodes.begin(); it != reactionNodes.end(); ++it)
	{
		Utility::MNBV::NodeEditorDraw::ReactionNode(it->data->name.c_str(), *it);
		it->ResetNLBSDUtilityStates();
	}

	for (std::vector< Utility::MNBV::SimpleParameterNodeData>::iterator it = simpleParameterNodes.begin(); it != simpleParameterNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::SimpleParameterNode(it->data->name.c_str(), *it);
		it->ResetNLBSDUtilityStates();
	}

	for (std::vector< Utility::MNBV::SimulationTimeNodeData>::iterator it = simulationTimeNodes.begin(); it != simulationTimeNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::SimulationTimeNode("Simulation Elapsed Time", *it);
	}

	for (std::vector< Utility::MNBV::SolverNodeData>::iterator it = solverNodes.begin(); it != solverNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::SolverNode(it->data->GetName(), *it);
	}

	for (std::vector< Utility::MNBV::SpeciesNodeData>::iterator it = speciesNodes.begin(); it != speciesNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::SpeciesNode(it->data->name.c_str(), *it);
		//it->ResetNLBSDUtilityStates();
	}
	
	for (std::vector< Utility::MNBV::ValueFloatNodeData>::iterator it = valueFloatNodes.begin(); it != valueFloatNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::ValueFloatNode("Float", *it);
	}

	Utility::MNBV::NodeEditorDraw::LinkCreation(links);

	for (std::vector< Utility::MNBV::LinkData>::iterator it = links.begin(); it != links.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::Link(*it);
	}
}