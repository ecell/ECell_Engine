#include "Util/BitwiseOperationsUtility.hpp"
#include "Utility/DragAndDrop.hpp"
#include "Widget/ModelExplorerWidget.hpp"//forward declaration
#include "Editor.hpp"

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::Awake()
{
	//Updates global style values that we want to be applied to every MNBV by default.
	ax::NodeEditor::SetCurrentEditor(editorCtxt);

	ax::NodeEditor::Style& axStyle = ax::NodeEditor::GetStyle();
	Style::NodeEditorStyle& neStyle = Style::EditorStyle::GetMNBVStyle();
	axStyle.NodeRounding = neStyle.nodeRounding;
	axStyle.NodeBorderWidth = neStyle.nodeBorderWidth;
	axStyle.HoveredNodeBorderWidth = neStyle.hoveredNodeBorderWidth;
	axStyle.SelectedNodeBorderWidth = neStyle.selectedNodeBorderWidth;
	axStyle.PinBorderWidth = neStyle.pinBorderWidth;
	axStyle.PinRounding = neStyle.pinRounding;

	ax::NodeEditor::SetCurrentEditor(nullptr);

	currentMNBVContext = GetCurrentMNBVContext();

	//If the current MNBV context doesn't have any commands manager at Awake time,
	//we set the commands manager defined in the engine at startup by default.
	if (currentMNBVContext->engineCommandsManager == nullptr)
	{
		currentMNBVContext->SetEngineCommandsManager(&editor->engine.GetCommandsManager());
	}

	//If the current MNBV context doesn't have any simulation at Awake time,
	//we set the simulation at index 0 by default.
	if (currentMNBVContext->simulation == nullptr)
	{
		currentMNBVContext->SetSimulation(Core::SimulationsManager::GetSingleton().GetSimulation(0));
	}
}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::Draw()
{	
	if (ImGui::Begin("Model Viewer"))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
		ImGui::Separator();

		ax::NodeEditor::SetCurrentEditor(editorCtxt);
		//Start interaction with editor.
		ax::NodeEditor::Begin("Model Exploration Space");
		currentMNBVContext = GetCurrentMNBVContext();


		//Checks for incomming Drag & Drop payloads
		HandleSimuDataRefDrop();

		//Draw the popup menu to choose which node to add (if it is open)
		DrawNodesPopupMenu();

		if (Util::IsFlagSet(utilityState, MNBVState_ImportAssetPopup))
		{
			DrawImportAssetPopup();
		}

		//Draw the nodes
		DrawNodes();

		ax::NodeEditor::NavigateToSelection();

		Utility::MNBV::NodeEditorDraw::NodeDestruction();

		Utility::MNBV::NodeEditorDraw::DynamicLinkDestruction(currentMNBVContext->dynamicLinks);

		Utility::MNBV::NodeEditorDraw::StaticLinkDestruction(currentMNBVContext->staticLinks);

		Utility::MNBV::NodeEditorDraw::LinkCreation(currentMNBVContext->dynamicLinks);

		if (Util::IsFlagSet(utilityState, MNBVState_FocusNode))
		{
			ax::NodeEditor::NavigateToSelection();
			Util::ClearFlag(utilityState, MNBVState_FocusNode);
		}

		// End of interaction with editor.
		ax::NodeEditor::End();
		ax::NodeEditor::SetCurrentEditor(nullptr);
	}
	ImGui::End();
}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::DrawImportAssetPopup()
{
	ax::NodeEditor::Suspend();
	ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(400, 120), ImGuiCond_Always);
	if (ImGui::Begin("Import Asset Node", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking))
	{
		ImGui::InputText("File Path", assetPathBuffer, 256);
		ImGui::InputText("Name to Display", assetNameBuffer, 64);

		if (ImGui::Button("Import"))
		{
			addModuleCommandArgs.simulationID = GetCurrentMNBVContext()->simulation->id;
			addModuleCommandArgs.modulePath = assetPathBuffer;
			addModuleCommandArgs.moduleName = assetNameBuffer;
			if (editor->engine.GetCommandsManager().ProcessCommand("addModule", addModuleCommandArgs))
			{
				//Spawn the corresponding asset node
				currentMNBVContext->assetNodes.emplace_back(Utility::MNBV::AssetNodeData(
					GetCurrentMNBVContext()->simulation->GetModules().back().get(),
					mousePos));
				currentMNBVContext->ConserveLinkDataIntegrity();

				Util::ClearFlag(utilityState, MNBVState_ImportAssetPopup); //Marks the Import Asset popup as closed
			}

		}

		ImGui::SameLine();

		if (ImGui::Button("Close"))
		{
			Util::ClearFlag(utilityState, MNBVState_ImportAssetPopup); //Marks the Import Asset popup as closed
		}

		ImGui::End();
	}
	ax::NodeEditor::Resume();
}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::DrawNodes()
{
	for (std::vector<Utility::MNBV::ArithmeticOperationNodeData>::iterator it = currentMNBVContext->arithmeticOperationNodes.begin(); it != currentMNBVContext->arithmeticOperationNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::ArithmeticOperationNode(*it);
	}

	for (std::vector<Utility::MNBV::AssetNodeData>::iterator it = currentMNBVContext->assetNodes.begin(); it != currentMNBVContext->assetNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::AssetNode(*it);

		//If double click on species selectable in the list box, spawn the corresponding species node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Species].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = currentMNBVContext->simulation->GetDataState().GetSpecies(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Species].GetDoubleClickedItem());
			currentMNBVContext->speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrSpecies], &currentMNBVContext->speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_Asset]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);
		}

		//If double click on simple parameter selectable in the list box, spawn the corresponding simple parameter node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Parameters].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Parameter> parameter = currentMNBVContext->simulation->GetDataState().GetParameter(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Parameters].GetDoubleClickedItem());
			currentMNBVContext->parameterNodes.emplace_back(Utility::MNBV::ParameterNodeData(parameter, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrParameters], &currentMNBVContext->parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::InputPin_Asset]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::CollapsingHeader_ModelLinks].id, 1);
		}

		//If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Equations].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Maths::Equation> equation = currentMNBVContext->simulation->GetDataState().GetEquation(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Equations].GetDoubleClickedItem());
			currentMNBVContext->equationNodes.emplace_back(Utility::MNBV::EquationNodeData(equation, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrEquations], &currentMNBVContext->equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::InputPin_Asset]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::InputPin_CollHdrModelLinks].id, 1);
		}

		//If double click on computed parameter selectable in the list box, spawn the corresponding computed parameter node
		if (it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Reactions].IsAnItemDoubleClicked())
		{
			currentMNBVContext->reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(
				currentMNBVContext->simulation->GetDataState().GetReaction(it->nlbsData[Utility::MNBV::AssetNodeData::NodeListBoxString_Reactions].GetDoubleClickedItem())));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::AssetNodeData::OutputPin_CollHdrReactions], &currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_Asset]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);
		}

		it->ResetNLBSDUtilityStates();
	}

	for (std::vector< Utility::MNBV::EquationNodeData>::iterator it = currentMNBVContext->equationNodes.begin(); it != currentMNBVContext->equationNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::EquationNode(*it);

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
			//set it to currentMNBVContext->equationNodes.end() (it is valid again).

			std::shared_ptr<ECellEngine::Maths::Equation> equation = it->nlbsDataEqDep.GetDoubleClickedItem().lock();
			Utility::MNBV::EquationNodeData equationNodeData = Utility::MNBV::EquationNodeData(equation, &currentMNBVContext->simulation->GetDependenciesDatabase());

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::EquationNodeData::OutputPin_CollHdrEquations], &equationNodeData.inputPins[Utility::MNBV::EquationNodeData::InputPin_NLBSEquations]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(equationNodeData.inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of the new node

			it->ResetNLBSDUtilityStates();
			currentMNBVContext->equationNodes.push_back(equationNodeData);
			it = currentMNBVContext->equationNodes.end() - 1;
		}

		//If double click on reaction kinetic law dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRKLDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRKLDep.GetDoubleClickedItem().lock();;
			currentMNBVContext->reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::EquationNodeData::OutputPin_CollHdrKineticLaws], &currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_NLBSEquations]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of the new node
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
			//set it to currentMNBVContext->equationNodes.end() (it is valid again).

			std::shared_ptr<ECellEngine::Maths::Equation> equation = currentMNBVContext->simulation->GetDataState().GetEquation(it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_EquationOperands].GetDoubleClickedItem());
			Utility::MNBV::EquationNodeData equationNodeData = Utility::MNBV::EquationNodeData(equation, &currentMNBVContext->simulation->GetDependenciesDatabase());

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::EquationNodeData::OutputPin_NLBSEquations], &equationNodeData.inputPins[Utility::MNBV::EquationNodeData::InputPin_CollHdrEquations]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(equationNodeData.inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node

			it->ResetNLBSDUtilityStates();
			currentMNBVContext->equationNodes.push_back(equationNodeData);
			it = currentMNBVContext->equationNodes.end() - 1;
		}

		//If double click on Parameter operand selectable in the list box, spawn the corresponding simple parameter node.
		if (it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_ParameterOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Parameter> parameter = currentMNBVContext->simulation->GetDataState().GetParameter(it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_ParameterOperands].GetDoubleClickedItem());
			currentMNBVContext->parameterNodes.emplace_back(Utility::MNBV::ParameterNodeData(parameter, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::EquationNodeData::OutputPin_NLBSParameters], &currentMNBVContext->parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::InputPin_CollHdrEquations]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on Species operand selectable in the list box, spawn the corresponding species node.
		if (it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_SpeciesOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = currentMNBVContext->simulation->GetDataState().GetSpecies(it->nlbsData[Utility::MNBV::EquationNodeData::NodeListBoxString_SpeciesOperands].GetDoubleClickedItem());
			currentMNBVContext->speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::EquationNodeData::OutputPin_NLBSSpecies], &currentMNBVContext->speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_CollHdrInEquation]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		it->ResetNLBSDUtilityStates();
	}

	for (std::vector<Utility::MNBV::LinePlotNodeData>::iterator it = currentMNBVContext->linePlotNodes.begin(); it != currentMNBVContext->linePlotNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::LinePlotNode(*it);
	}

	for (std::vector<Utility::MNBV::LogicOperationNodeData>::iterator it = currentMNBVContext->logicOperationNodes.begin(); it != currentMNBVContext->logicOperationNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::LogicOperationNode(*it);
	}

	for (std::vector<Utility::MNBV::ModifyDataStateValueEventNodeData>::iterator it = currentMNBVContext->modifyDataStateValueEventNodes.begin(); it != currentMNBVContext->modifyDataStateValueEventNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::ModifyDataStateValueEventNode(*it);
	}

	for (std::vector< Utility::MNBV::ReactionNodeData>::iterator it = currentMNBVContext->reactionNodes.begin(); it != currentMNBVContext->reactionNodes.end(); ++it)
	{
		Utility::MNBV::NodeEditorDraw::ReactionNode(*it);

		//------------------- Interaction with List Box for Contains Dependencies (Operands of the kinetic law)

		//If double click on Species operand selectable in the list box of products, spawn the corresponding species node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_Products].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = currentMNBVContext->simulation->GetDataState().GetSpecies(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_Products].GetDoubleClickedItem());
			currentMNBVContext->speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_CollHdrProducts], &currentMNBVContext->speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_CollHdrAsProduct]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on Species operand selectable in the list box of reactants, spawn the corresponding species node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_Reactants].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = currentMNBVContext->simulation->GetDataState().GetSpecies(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_Reactants].GetDoubleClickedItem());
			currentMNBVContext->speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_CollHdrReactants], &currentMNBVContext->speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_CollHdrAsReactant]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on Equation operand selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_EquationOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Maths::Equation> equation = currentMNBVContext->simulation->GetDataState().GetEquation(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_EquationOperands].GetDoubleClickedItem());
			currentMNBVContext->equationNodes.emplace_back(Utility::MNBV::EquationNodeData(equation, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_NLBSEquations], &currentMNBVContext->equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::InputPin_CollHdrKineticLaws]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on Parameter operand selectable in the list box, spawn the corresponding simple parameter node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_ParameterOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Parameter> parameter = currentMNBVContext->simulation->GetDataState().GetParameter(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_ParameterOperands].GetDoubleClickedItem());
			currentMNBVContext->parameterNodes.emplace_back(Utility::MNBV::ParameterNodeData(parameter, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_NLBSParameters], &currentMNBVContext->parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::InputPin_CollHdrKineticLaws]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->parameterNodes.back().inputPins[Utility::MNBV::ParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on Species operand selectable in the list box, spawn the corresponding species node.
		if (it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_SpeciesOperands].IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Species> species = currentMNBVContext->simulation->GetDataState().GetSpecies(it->nlbsData[Utility::MNBV::ReactionNodeData::NodeListBoxString_SpeciesOperands].GetDoubleClickedItem());
			currentMNBVContext->speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(species, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ReactionNodeData::OutputPin_NLBSSpecies], &currentMNBVContext->speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::InputPin_CollHdrInKineticLaw]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->speciesNodes.back().inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		it->ResetNLBSDUtilityStates();
	}

	for (std::vector< Utility::MNBV::ParameterNodeData>::iterator it = currentMNBVContext->parameterNodes.begin(); it != currentMNBVContext->parameterNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::ParameterNode(*it);

		//------------------- Interaction with List Box for Is-Part-Of Dependencies

		//If double click on Equation dependency selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsDataEqDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Maths::Equation> equation = it->nlbsDataEqDep.GetDoubleClickedItem().lock();
			currentMNBVContext->equationNodes.emplace_back(Utility::MNBV::EquationNodeData(equation, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ParameterNodeData::OutputPin_CollHdrEquations], &currentMNBVContext->equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::InputPin_NLBSParameters]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of the new node
		}

		//If double click on reaction kinetic law dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRKLDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRKLDep.GetDoubleClickedItem().lock();;
			currentMNBVContext->reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::ParameterNodeData::OutputPin_CollHdrKineticLaws], &currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_NLBSParameters]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::ParameterNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of the new node
		}

		it->ResetNLBSDUtilityStates();
	}

	for (std::vector< Utility::MNBV::SolverNodeData>::iterator it = currentMNBVContext->solverNodes.begin(); it != currentMNBVContext->solverNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::SolverNode(*it);
	}

	for (std::vector< Utility::MNBV::SpeciesNodeData>::iterator it = currentMNBVContext->speciesNodes.begin(); it != currentMNBVContext->speciesNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::SpeciesNode(*it);

		//If double click on Equation dependency selectable in the list box, spawn the corresponding computed parameter node.
		if (it->nlbsDataEqDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Maths::Equation> equation = it->nlbsDataEqDep.GetDoubleClickedItem().lock();
			currentMNBVContext->equationNodes.emplace_back(Utility::MNBV::EquationNodeData(equation, &currentMNBVContext->simulation->GetDependenciesDatabase()));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SpeciesNodeData::OutputPin_CollHdrInEquation], &currentMNBVContext->equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::InputPin_NLBSSpecies]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->equationNodes.back().inputPins[Utility::MNBV::EquationNodeData::CollapsingHeader_EquationOperands].id, 1);//fallback of the new node
		}

		//If double click on reaction products dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRPDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRPDep.GetDoubleClickedItem().lock();;
			currentMNBVContext->reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SpeciesNodeData::OutputPin_CollHdrAsProduct], &currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_CollHdrProducts]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on reaction reactants dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRRDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRRDep.GetDoubleClickedItem().lock();;
			currentMNBVContext->reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SpeciesNodeData::OutputPin_CollHdrAsReactant], &currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_CollHdrReactants]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of the new node
		}

		//If double click on reaction kinetic law dependency in the list box, spawn the corresponding reaction node.
		if (it->nlbsDataRKLDep.IsAnItemDoubleClicked())
		{
			std::shared_ptr<ECellEngine::Data::Reaction> reaction = it->nlbsDataRKLDep.GetDoubleClickedItem().lock();;
			currentMNBVContext->reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(reaction));

			currentMNBVContext->staticLinks.emplace_back(Utility::MNBV::LinkData(&it->outputPins[Utility::MNBV::SpeciesNodeData::OutputPin_CollHdrInKineticLaw], &currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::InputPin_NLBSSpecies]));
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(it->inputPins[Utility::MNBV::SpeciesNodeData::CollapsingHeader_ModelLinks].id, 1);//fallback of this node
			currentMNBVContext->staticLinks.back().OverrideEndFallbackPin(currentMNBVContext->reactionNodes.back().inputPins[Utility::MNBV::ReactionNodeData::CollapsingHeader_KineticLawOperands].id, 1);//fallback of the new node
		}

		it->ResetNLBSDUtilityStates();
	}

	for (std::vector< Utility::MNBV::TimeNodeData>::iterator it = currentMNBVContext->timeNodes.begin(); it != currentMNBVContext->timeNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::TimeNode(*it);
	}

	for (std::vector< Utility::MNBV::TriggerNodeData>::iterator it = currentMNBVContext->triggerNodes.begin(); it != currentMNBVContext->triggerNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::TriggerNode(*it);
	}

	for (std::vector< Utility::MNBV::ValueFloatNodeData>::iterator it = currentMNBVContext->valueFloatNodes.begin(); it != currentMNBVContext->valueFloatNodes.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::ValueFloatNode(*it);
	}

	for (std::vector< Utility::MNBV::LinkData>::iterator it = currentMNBVContext->staticLinks.begin(); it != currentMNBVContext->staticLinks.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::Link(*it);
	}

	for (std::vector< Utility::MNBV::LinkData>::iterator it = currentMNBVContext->dynamicLinks.begin(); it != currentMNBVContext->dynamicLinks.end(); it++)
	{
		Utility::MNBV::NodeEditorDraw::Link(*it);
	}
}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::DrawNodesPopupMenu()
{
	//Taking the mouse position before suspending the node editor
	//to spawn the nodes at the correct position.
	mousePos = ImGui::GetMousePos();

	ax::NodeEditor::Suspend();
	if (ax::NodeEditor::ShowBackgroundContextMenu())
	{
		ImGui::OpenPopup("Create New Node");
	}

	if (ImGui::BeginPopup("Create New Node"))
	{
		if (ImGui::BeginMenu("Data"))
		{
			if (ImGui::MenuItem("Import Asset From File"))
			{
				Util::SetFlag(utilityState, MNBVState_ImportAssetPopup); //Marks the Import Asset From File popup as opened
			}

			if (ImGui::MenuItem("Value Float Node"))
			{
				currentMNBVContext->valueFloatNodes.emplace_back(Utility::MNBV::ValueFloatNodeData(0.f, mousePos));
				currentMNBVContext->ConserveLinkDataIntegrity();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Events"))
		{
			if (ImGui::MenuItem("Modify Value in DataState Event Node"))
			{
				//TODO: Use a command to add th event
				currentMNBVContext->modifyDataStateValueEventNodes.emplace_back(Utility::MNBV::ModifyDataStateValueEventNodeData(currentMNBVContext->simulation->GetDataState().AddModifyDataStateValueEvent(), mousePos));
				currentMNBVContext->ConserveLinkDataIntegrity();
			}

			if (ImGui::MenuItem("Trigger Node"))
			{
				//TODO: Use a command to add a trigger
				currentMNBVContext->triggerNodes.emplace_back(Utility::MNBV::TriggerNodeData(currentMNBVContext->simulation->GetDataState().AddTrigger(), mousePos));
				currentMNBVContext->ConserveLinkDataIntegrity();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Maths"))
		{
			if (ImGui::MenuItem("Arithmetic Operation"))
			{
				//TODO: Use a command to add an Operation
				currentMNBVContext->arithmeticOperationNodes.emplace_back(Utility::MNBV::ArithmeticOperationNodeData(currentMNBVContext->simulation->GetDataState().AddOperation(), mousePos));
				currentMNBVContext->ConserveLinkDataIntegrity();
			}

			if (ImGui::MenuItem("Logic Operation"))
			{
				//TODO: Use a command to add a LogicOperation
				currentMNBVContext->logicOperationNodes.emplace_back(Utility::MNBV::LogicOperationNodeData(currentMNBVContext->simulation->GetDataState().AddLogicOperation(), mousePos));
				currentMNBVContext->ConserveLinkDataIntegrity();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Plots"))
		{
			if (ImGui::MenuItem("Line Plot Node"))
			{
				currentMNBVContext->linePlotNodes.emplace_back(Utility::MNBV::LinePlotNodeData(mousePos));
				currentMNBVContext->ConserveLinkDataIntegrity();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Solvers"))
		{
			bool addsolver = false;
			if (ImGui::MenuItem("Gillespie Next Reaction Method"))
			{
				addSolverCommandArgs.simulationID = GetCurrentMNBVContext()->simulation->id;
				addSolverCommandArgs.solverName = "GillespieNRMRSolver";
				addsolver = true;
			}

			if (ImGui::MenuItem("ODE Runge-Kutta 4"))
			{
				addSolverCommandArgs.simulationID = GetCurrentMNBVContext()->simulation->id;
				addSolverCommandArgs.solverName = "GeneralizedExplicitRK";
				addsolver = true;
			}
			
			if (addsolver)
			{
				if (editor->engine.GetCommandsManager().ProcessCommand("addSolver", addSolverCommandArgs))
				{
					//Spawn the corresponding asset node
					currentMNBVContext->solverNodes.emplace_back(Utility::MNBV::SolverNodeData(
						GetCurrentMNBVContext()->simulation->GetSolvers().back(),
						mousePos));
					currentMNBVContext->ConserveLinkDataIntegrity();
				}			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Time"))
		{
			if (ImGui::MenuItem("Simulation Time"))
			{
				currentMNBVContext->timeNodes.emplace_back(Utility::MNBV::TimeNodeData(&currentMNBVContext->simulation->GetTimer(), mousePos));
				currentMNBVContext->ConserveLinkDataIntegrity();
			}
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
	ax::NodeEditor::Resume();
}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::FocusNode(const std::size_t _nodeID)
{
	//Start interaction with editor.
	ax::NodeEditor::SetCurrentEditor(editorCtxt);
	ax::NodeEditor::Begin("Model Exploration Space");
	
	ax::NodeEditor::SelectNode(_nodeID);
	Util::SetFlag(utilityState, MNBVState_FocusNode);

	//// End of interaction with editor.
	ax::NodeEditor::End();
	ax::NodeEditor::SetCurrentEditor(nullptr);

}

void ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerWidget::HandleSimuDataRefDrop()
{
	if (ECellEngine::Editor::Utility::DragAndDrop::payloadCharged)
	{
		if (ECellEngine::Editor::Utility::DragAndDrop::AcceptPayload("Hierarchy_Equation_DND"))
		{
			currentMNBVContext->equationNodes.emplace_back(Utility::MNBV::EquationNodeData(*Utility::DragAndDrop::GetPayload<std::shared_ptr<ECellEngine::Maths::Equation>>(), &currentMNBVContext->simulation->GetDependenciesDatabase()));
		}

		if(ECellEngine::Editor::Utility::DragAndDrop::AcceptPayload("Hierarchy_Parameter_DND"))
		{
			currentMNBVContext->parameterNodes.emplace_back(Utility::MNBV::ParameterNodeData(*Utility::DragAndDrop::GetPayload<std::shared_ptr<ECellEngine::Data::Parameter>>(), &currentMNBVContext->simulation->GetDependenciesDatabase()));
		}
		
		if (ECellEngine::Editor::Utility::DragAndDrop::AcceptPayload("Hierarchy_Reaction_DND"))
		{
			currentMNBVContext->reactionNodes.emplace_back(Utility::MNBV::ReactionNodeData(*Utility::DragAndDrop::GetPayload<std::shared_ptr<ECellEngine::Data::Reaction>>()));
		}

		if (ECellEngine::Editor::Utility::DragAndDrop::AcceptPayload("Hierarchy_Species_DND"))
		{
			currentMNBVContext->speciesNodes.emplace_back(Utility::MNBV::SpeciesNodeData(*Utility::DragAndDrop::GetPayload<std::shared_ptr<ECellEngine::Data::Species>>(), &currentMNBVContext->simulation->GetDependenciesDatabase()));
		}
	}
}