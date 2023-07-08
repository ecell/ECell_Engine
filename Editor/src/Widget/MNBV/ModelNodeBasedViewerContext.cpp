#include "Widget/MNBV/ModelNodeBasedViewerContext.hpp"

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext::ConserveLinkDataIntegrity()
{
	for (std::vector<Utility::MNBV::LinkData>::iterator it = dynamicLinks.begin(); it != dynamicLinks.end(); it++)
	{
		it->Refresh();
	}
}

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
				ConserveLinkDataIntegrity();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Events"))
		{
			if (ImGui::MenuItem("Modify Value in DataState Event Node"))
			{
				ax::NodeEditor::Resume();
				//TODO: Use a command to add th event
				modifyDataStateValueEventNodes.emplace_back(Utility::MNBV::ModifyDataStateValueEventNodeData(_simulation->GetDataState()->AddModifyDataStateValueEvent(), ImGui::GetIO().MousePos));
				ax::NodeEditor::Suspend();
				ConserveLinkDataIntegrity();
			}
			
			if (ImGui::MenuItem("Trigger Node"))
			{
				ax::NodeEditor::Resume();
				//TODO: Use a command to add a trigger
				triggerNodes.emplace_back(Utility::MNBV::TriggerNodeData(_simulation->GetDataState()->AddTrigger(), ImGui::GetIO().MousePos));
				ax::NodeEditor::Suspend();
				ConserveLinkDataIntegrity();
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Plots"))
		{
			if (ImGui::MenuItem("Line Plot Node"))
			{
				ax::NodeEditor::Resume();
				linePlotNodes.emplace_back(Utility::MNBV::LinePlotNodeData(ImGui::GetIO().MousePos));
				ax::NodeEditor::Suspend();
				ConserveLinkDataIntegrity();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Solvers"))
		{
			if (ImGui::MenuItem("Gillespie Next Reaction Method"))
			{
				ax::NodeEditor::Resume();
				//TODO: Use a command to add a solver
				solverNodes.emplace_back(Utility::MNBV::SolverNodeData(_simulation->AddSolver("GillespieNRMRSolver"), ImGui::GetIO().MousePos));
				ax::NodeEditor::Suspend();
				ConserveLinkDataIntegrity();
			}
			
			if (ImGui::MenuItem("ODE Runge-Kutta 4"))
			{
				ax::NodeEditor::Resume();
				//TODO: Use a command to add a solver
				solverNodes.emplace_back(Utility::MNBV::SolverNodeData(_simulation->AddSolver("GeneralizedExplicitRK"), ImGui::GetIO().MousePos));
				ax::NodeEditor::Suspend();
				ConserveLinkDataIntegrity();
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
				ConserveLinkDataIntegrity();
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
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Parameters].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Parameter> parameter = _simulation->GetDataState()->GetParameter(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Parameters].GetDoubleClickedItem());
			parameterNodes.emplace_back(Utility::MNBV::ParameterNodeData(parameter, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrParameters], &parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::InputPin_Asset]));
			staticLinks.back().OverrideEndFallbackPin(parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::CollapsingHeader_ModelLinks].id, 1);
		}

		//If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Equations].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Maths::Equation> equation = _simulation->GetDataState()->GetEquation(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Equations].GetDoubleClickedItem());
			equationNodes.emplace_back(Utility::MNBV::EquationNodeData(equation, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrEquations], &equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::InputPin_Asset]));
			staticLinks.back().OverrideEndFallbackPin(equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::InputPin_CollHdrModelLinks].id, 1);
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
	
	for (std::vector< Utility::MNBV::EquationNodeData>::iterator it = equationNodes.begin(); it != equationNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::EquationNode(it->data->GetOperand()->name.c_str(), *it);

		//------------------- Interaction with List Box for Is-Part-Of Dependencies

		//If double click on Equation dependency selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsDataEqDep.IsAnItemDoubleClicked())
		{
			//Here, we need to be carefull how we add the new computed parameter node because we are
			//currently going through the list that contains them. So, if we add a new CompParamData
			//and it triggers aresize of the vector, then the iterator will be invalidated.
			//To solve this, we do not emplace_back() the new data directly but create a buffer
			//(equationNodeData) which we push_back() at the end. Finally, we know that
			//there won't be another doubleclick within this frame so we skip the rest of the loop and
			//set it to equationNodes.end() (it is valid again).

			std::shared_ptr<ECellEngine::Maths::Equation> equation = it->nlbsDataEqDep.GetDoubleClickedItem().lock();
			Utility::MNBV::EquationNodeData equationNodeData = Utility::MNBV::EquationNodeData(equation, &_simulation->GetDependenciesDatabase());

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::EquationNodeData::OutputPin_CollHdrEquations], &equationNodeData.inputPins[Utility::MNBV::EquationNodeData::InputPin_NLBSEquations]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(equationNodeData.inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of the new node

			it->ResetNLBSDUtilityStates();
			equationNodes.push_back(equationNodeData);
			it = equationNodes.end() - 1;
		}

		//If double click on reaction kinetic law dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRKLDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRKLDep.GetDoubleClickedItem().lock();;
			reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction.get()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::EquationNodeData::OutputPin_CollHdrKineticLaws], &reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_NLBSEquations]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of the new node
		}

		//------------------- Interaction with List Box for Contains Dependencies (Operands of the operation)

		//If double click on Equation operand selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_EquationOperands].IsAnItemDoubleClicked())
		{
			//Here, we need to be carefull how we add the new computed parameter node because we are
			//currently going through the list that contains them. So, if we add a new CompParamData
			//and it triggers aresize of the vector, then the iterator will be invalidated.
			//To solve this, we do not emplace_back() the new data directly but create a buffer
			//(equationNodeData) which we push_back() at the end. Finally, we know that
			//there won't be another doubleclick within this frame so we skip the rest of the loop and
			//set it to equationNodes.end() (it is valid again).

			std::shared_ptr<ECellEngine::Maths::Equation> equation = _simulation->GetDataState()->GetEquation(it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_EquationOperands].GetDoubleClickedItem());
			Utility::MNBV::EquationNodeData equationNodeData = Utility::MNBV::EquationNodeData(equation, &_simulation->GetDependenciesDatabase());

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::EquationNodeData::OutputPin_NLBSEquations], &equationNodeData.inputPins[Utility::MNBV::EquationNodeData::InputPin_CollHdrEquations]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(equationNodeData.inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
			
			it->ResetNLBSDUtilityStates();
			equationNodes.push_back(equationNodeData);
			it = equationNodes.end()-1;
		}

		//If double click on Parameter operand selectable in the list box, spawn the corresponding simple parameter node.
		if (it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_ParameterOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Parameter> parameter = _simulation->GetDataState()->GetParameter(it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_ParameterOperands].GetDoubleClickedItem());
			parameterNodes.emplace_back(Utility::MNBV::ParameterNodeData(parameter, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::EquationNodeData::OutputPin_NLBSParameters], &parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::InputPin_CollHdrEquations]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on Species operand selectable in the list box, spawn the corresponding species node.
		if (it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_SpeciesOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = _simulation->GetDataState()->GetSpecies(it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_SpeciesOperands].GetDoubleClickedItem());
			speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::EquationNodeData::OutputPin_NLBSSpecies], &speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_CollHdrInEquation]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		it->ResetNLBSDUtilityStates();
	}

	for (std::vector<Utility::MNBV::LinePlotNodeData>::iterator it = linePlotNodes.begin(); it != linePlotNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::LinePlotNode(*it);
	}

	for (std::vector<Utility::MNBV::ModifyDataStateValueEventNodeData>::iterator it = modifyDataStateValueEventNodes.begin(); it != modifyDataStateValueEventNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::ModifyDataStateValueEventNode(*it);
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

		//If double click on Equation operand selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_EquationOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Maths::Equation> equation = _simulation->GetDataState()->GetEquation(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_EquationOperands].GetDoubleClickedItem());
			equationNodes.emplace_back(Utility::MNBV::EquationNodeData(equation, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_NLBSEquations], &equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::InputPin_CollHdrKineticLaws]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on Parameter operand selectable in the list box, spawn the corresponding simple parameter node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_ParameterOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Parameter> parameter = _simulation->GetDataState()->GetParameter(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_ParameterOperands].GetDoubleClickedItem());
			parameterNodes.emplace_back(Utility::MNBV::ParameterNodeData(parameter, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_NLBSParameters], &parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::InputPin_CollHdrKineticLaws]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
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

	for (std::vector< Utility::MNBV::ParameterNodeData>::iterator it = parameterNodes.begin(); it != parameterNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::ParameterNode(it->data->name.c_str(), *it);

		//------------------- Interaction with List Box for Is-Part-Of Dependencies

		//If double click on Equation dependency selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsDataEqDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Maths::Equation> equation = it->nlbsDataEqDep.GetDoubleClickedItem().lock();
			equationNodes.emplace_back(Utility::MNBV::EquationNodeData(equation, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ParameterNodeData::OutputPin_CollHdrEquations], &equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::InputPin_NLBSParameters]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of the new node
		}

		//If double click on reaction kinetic law dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRKLDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRKLDep.GetDoubleClickedItem().lock();;
			reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction.get()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ParameterNodeData::OutputPin_CollHdrKineticLaws], &reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_NLBSParameters]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
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
		Utility::MNBV::NodeEditorDraw::SolverNode(it->data->GetName().c_str(), *it);
	}

	for (std::vector< Utility::MNBV::SpeciesNodeData>::iterator it = speciesNodes.begin(); it != speciesNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::SpeciesNode(it->data->name.c_str(), *it);

		//If double click on Equation dependency selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsDataEqDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Maths::Equation> equation = it->nlbsDataEqDep.GetDoubleClickedItem().lock();
			equationNodes.emplace_back(Utility::MNBV::EquationNodeData(equation, &_simulation->GetDependenciesDatabase()));

			staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SpeciesNodeData::OutputPin_CollHdrInEquation], &equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::InputPin_NLBSSpecies]));
			staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			staticLinks.back().OverrideEndFallbackPin(equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of the new node
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

	for (std::vector< Utility::MNBV::TriggerNodeData>::iterator it = triggerNodes.begin(); it != triggerNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::TriggerNode(*it);
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