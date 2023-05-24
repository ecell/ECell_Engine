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
			speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrSpecies],&speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_Asset]));
			staticLinks.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);
		}

		//If double click on simple parameter selectable in the list box, spawn the corresponding simple parameter node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_SimpleParameters].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::SimpleParameter> simpleParameter = _simulation->GetDataState()->GetSimpleParameter(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_SimpleParameters].GetDoubleClickedItem());
			simpleParameterNodes.emplace_back(Utility::MNBV::SimpleParameterNodeData(simpleParameter, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrSimpleParameters], &simpleParameterNodes.back().inputPins[Utility::MNBV::SimpleParameterNodeData::InputPin_Asset]));
			staticLinks.back().OverrideEndFallbackPin(simpleParameterNodes.back().inputPins[Utility::MNBV::SimpleParameterNodeData::CollapsingHeader_ModelLinks].id, 1);
		}

		//If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_ComputedParameters].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::ComputedParameter> computedParameter = _simulation->GetDataState()->GetComputedParameter(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_ComputedParameters].GetDoubleClickedItem());
			computedParameterNodes.emplace_back(Utility::MNBV::ComputedParameterNodeData(computedParameter, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrComputedParameters], &computedParameterNodes.back().inputPins[Utility::MNBV::ComputedParameterNodeData::InputPin_Asset]));
			staticLinks.back().OverrideEndFallbackPin(computedParameterNodes.back().inputPins[Utility::MNBV::ComputedParameterNodeData::InputPin_CollHdrModelLinks].id, 1);
		}

		//If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Reactions].IsAnItemDoubleClicked())
		{
			reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(
				_simulation->GetDataState()->GetReaction(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Reactions].GetDoubleClickedItem()).get()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrReactions], &reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_Asset]));
			staticLinks.back().OverrideEndFallbackPin(reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);
		}

		it->ResetNLBSDUtilityStates();
	}
	
	for (std::vector< Utility::MNBV::ComputedParameterNodeData>::iterator it = computedParameterNodes.begin(); it != computedParameterNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::ComputedParameterNode(it->data->name.c_str(), *it);

		//------------------- Interaction with List Box for Is-Part-Of Dependencies

		//If double click on ComputedParameter dependency selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsDataCPDep.IsAnItemDoubleClicked())
		{
			//Here, we need to be carefull how we add the new computed parameter node because we are
			//currently going through the list that contains them. So, if we add a new CompParamData
			//and it triggers aresize of the vector, then the iterator will be invalidated.
			//To solve this, we do not emplace_back() the new data directly but create a buffer
			//(computedParameterNodeData) which we push_back() at the end. Finally, we know that
			//there won't be another doubleclick within this frame so we skip the rest of the loop and
			//set it to computedParameterNodes.end() (it is valid again).

			std::shared_ptr<ECellEngine::Data::ComputedParameter> computedParameter = it->nlbsDataCPDep.GetDoubleClickedItem().lock();
			Utility::MNBV::ComputedParameterNodeData computedParameterNodeData = Utility::MNBV::ComputedParameterNodeData(computedParameter, &_simulation->GetDependenciesDatabase());

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ComputedParameterNodeData::OutputPin_CollHdrComputedParameters], &computedParameterNodeData.inputPins[Utility::MNBV::ComputedParameterNodeData::InputPin_NLBSComputedParameters]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ComputedParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(computedParameterNodeData.inputPins[Utility::MNBV::ComputedParameterNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of the new node

			it->ResetNLBSDUtilityStates();
			computedParameterNodes.push_back(computedParameterNodeData);
			it = computedParameterNodes.end() - 1;
		}

		//If double click on reaction kinetic law dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRKLDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRKLDep.GetDoubleClickedItem().lock();;
			reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction.get()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ComputedParameterNodeData::OutputPin_CollHdrKineticLaws], &reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_NLBSComputedParameters]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ComputedParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of the new node
		}

		//------------------- Interaction with List Box for Contains Dependencies (Operands of the operation)

		//If double click on ComputedParameter operand selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsData[Utility::MNBV::ComputedParameterNodeData::NodeListBoxString_ComputedParameterOperands].IsAnItemDoubleClicked())
		{
			//Here, we need to be carefull how we add the new computed parameter node because we are
			//currently going through the list that contains them. So, if we add a new CompParamData
			//and it triggers aresize of the vector, then the iterator will be invalidated.
			//To solve this, we do not emplace_back() the new data directly but create a buffer
			//(computedParameterNodeData) which we push_back() at the end. Finally, we know that
			//there won't be another doubleclick within this frame so we skip the rest of the loop and
			//set it to computedParameterNodes.end() (it is valid again).

			std::shared_ptr<ECellEngine::Data::ComputedParameter> computedParameter = _simulation->GetDataState()->GetComputedParameter(it->nlbsData[Utility::MNBV::ComputedParameterNodeData::NodeListBoxString_ComputedParameterOperands].GetDoubleClickedItem());
			Utility::MNBV::ComputedParameterNodeData computedParameterNodeData = Utility::MNBV::ComputedParameterNodeData(computedParameter, &_simulation->GetDependenciesDatabase());

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ComputedParameterNodeData::OutputPin_NLBSComputedParameters], &computedParameterNodeData.inputPins[Utility::MNBV::ComputedParameterNodeData::InputPin_CollHdrComputedParameters]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ComputedParameterNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(computedParameterNodeData.inputPins[Utility::MNBV::ComputedParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
			
			it->ResetNLBSDUtilityStates();
			computedParameterNodes.push_back(computedParameterNodeData);
			it = computedParameterNodes.end()-1;
		}

		//If double click on SimpleParameter operand selectable in the list box, spawn the corresponding simple parameter node.
		if (it->nlbsData[Utility::MNBV::ComputedParameterNodeData::NodeListBoxString_SimpleParameterOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::SimpleParameter> simpleParameter = _simulation->GetDataState()->GetSimpleParameter(it->nlbsData[Utility::MNBV::ComputedParameterNodeData::NodeListBoxString_SimpleParameterOperands].GetDoubleClickedItem());
			simpleParameterNodes.emplace_back(Utility::MNBV::SimpleParameterNodeData(simpleParameter, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ComputedParameterNodeData::OutputPin_NLBSSimpleParameters], &simpleParameterNodes.back().inputPins[Utility::MNBV::SimpleParameterNodeData::InputPin_CollHdrComputedParameters]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ComputedParameterNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(simpleParameterNodes.back().inputPins[Utility::MNBV::SimpleParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on Species operand selectable in the list box, spawn the corresponding species node.
		if (it->nlbsData[Utility::MNBV::ComputedParameterNodeData::NodeListBoxString_SpeciesOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = _simulation->GetDataState()->GetSpecies(it->nlbsData[Utility::MNBV::ComputedParameterNodeData::NodeListBoxString_SpeciesOperands].GetDoubleClickedItem());
			speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ComputedParameterNodeData::OutputPin_NLBSSpecies], &speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_CollHdrInComputedParameter]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ComputedParameterNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		it->ResetNLBSDUtilityStates();
	}

	for (std::vector<Utility::MNBV::LinePlotNodeData>::iterator it = linePlotNodes.begin(); it != linePlotNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::LinePlotNode(it->name, *it);
	}

	for (std::vector< Utility::MNBV::ReactionNodeData>::iterator it = reactionNodes.begin(); it != reactionNodes.end(); ++it)
	{
		Utility::MNBV::NodeEditorDraw::ReactionNode(it->data->name.c_str(), *it);

		//------------------- Interaction with List Box for Contains Dependencies (Operands of the kinetic law)

		//If double click on Species operand selectable in the list box of products, spawn the corresponding species node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_Products].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = _simulation->GetDataState()->GetSpecies(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_Products].GetDoubleClickedItem());
			speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_CollHdrProducts], &speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_CollHdrAsProduct]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on Species operand selectable in the list box of reactants, spawn the corresponding species node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_Reactants].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = _simulation->GetDataState()->GetSpecies(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_Reactants].GetDoubleClickedItem());
			speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_CollHdrReactants], &speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_CollHdrAsReactant]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on ComputedParameter operand selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_ComputedParameterOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::ComputedParameter> computedParameter = _simulation->GetDataState()->GetComputedParameter(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_ComputedParameterOperands].GetDoubleClickedItem());
			computedParameterNodes.emplace_back(Utility::MNBV::ComputedParameterNodeData(computedParameter, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_NLBSComputedParameters], &computedParameterNodes.back().inputPins[Utility::MNBV::ComputedParameterNodeData::InputPin_CollHdrKineticLaws]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(computedParameterNodes.back().inputPins[Utility::MNBV::ComputedParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on SimpleParameter operand selectable in the list box, spawn the corresponding simple parameter node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_SimpleParameterOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::SimpleParameter> simpleParameter = _simulation->GetDataState()->GetSimpleParameter(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_SimpleParameterOperands].GetDoubleClickedItem());
			simpleParameterNodes.emplace_back(Utility::MNBV::SimpleParameterNodeData(simpleParameter, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_NLBSSimpleParameters], &simpleParameterNodes.back().inputPins[Utility::MNBV::SimpleParameterNodeData::InputPin_CollHdrKineticLaws]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(simpleParameterNodes.back().inputPins[Utility::MNBV::SimpleParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on Species operand selectable in the list box, spawn the corresponding species node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_SpeciesOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = _simulation->GetDataState()->GetSpecies(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_SpeciesOperands].GetDoubleClickedItem());
			speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_NLBSSpecies], &speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_CollHdrInKineticLaw]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		it->ResetNLBSDUtilityStates();
	}

	for (std::vector< Utility::MNBV::SimpleParameterNodeData>::iterator it = simpleParameterNodes.begin(); it != simpleParameterNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::SimpleParameterNode(it->data->name.c_str(), *it);

		//------------------- Interaction with List Box for Is-Part-Of Dependencies

		//If double click on ComputedParameter dependency selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsDataCPDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::ComputedParameter> computedParameter = it->nlbsDataCPDep.GetDoubleClickedItem().lock();
			computedParameterNodes.emplace_back(Utility::MNBV::ComputedParameterNodeData(computedParameter, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SimpleParameterNodeData::OutputPin_CollHdrComputedParameters], &computedParameterNodes.back().inputPins[Utility::MNBV::ComputedParameterNodeData::InputPin_NLBSSimpleParameters]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SimpleParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(computedParameterNodes.back().inputPins[Utility::MNBV::ComputedParameterNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of the new node
		}

		//If double click on reaction kinetic law dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRKLDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRKLDep.GetDoubleClickedItem().lock();;
			reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction.get()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SimpleParameterNodeData::OutputPin_CollHdrKineticLaws], &reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_NLBSSimpleParameters]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SimpleParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of the new node
		}

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

		//If double click on ComputedParameter dependency selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsDataCPDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::ComputedParameter> computedParameter = it->nlbsDataCPDep.GetDoubleClickedItem().lock();
			computedParameterNodes.emplace_back(Utility::MNBV::ComputedParameterNodeData(computedParameter, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SpeciesNodeData::OutputPin_CollHdrInComputedParameter], &computedParameterNodes.back().inputPins[Utility::MNBV::ComputedParameterNodeData::InputPin_NLBSSpecies]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(computedParameterNodes.back().inputPins[Utility::MNBV::ComputedParameterNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of the new node
		}

		//If double click on reaction products dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRPDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRPDep.GetDoubleClickedItem().lock();;
			reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction.get()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SpeciesNodeData::OutputPin_CollHdrAsProduct], &reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_CollHdrProducts]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on reaction reactants dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRRDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRRDep.GetDoubleClickedItem().lock();;
			reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction.get()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SpeciesNodeData::OutputPin_CollHdrAsReactant], &reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_CollHdrReactants]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on reaction kinetic law dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRKLDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRKLDep.GetDoubleClickedItem().lock();;
			reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction.get()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SpeciesNodeData::OutputPin_CollHdrInKineticLaw], &reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_NLBSSpecies]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of the new node
		}

		it->ResetNLBSDUtilityStates();
	}
	
	for (std::vector< Utility::MNBV::ValueFloatNodeData>::iterator it = valueFloatNodes.begin(); it != valueFloatNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::ValueFloatNode("Float", *it);
	}

	for (std::vector< Utility::MNBV::LinkData>::iterator it = staticLinks.begin(); it != staticLinks.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::Link(*it);
	}
	
	for (std::vector< Utility::MNBV::LinkData>::iterator it = dynamicLinks.begin(); it != dynamicLinks.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::Link(*it);
	}

	Utility::MNBV::NodeEditorDraw::NodeDestruction();

	Utility::MNBV::NodeEditorDraw::DynamicLinkDestruction(dynamicLinks);

	Utility::MNBV::NodeEditorDraw::StaticLinkDestruction(staticLinks);

	Utility::MNBV::NodeEditorDraw::LinkCreation(dynamicLinks);
}