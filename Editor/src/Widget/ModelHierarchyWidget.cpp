#include "IO/CommandArgs.hpp"
#include "Util/BitwiseOperationsUtility.hpp"
#include "Utility/DragAndDrop.hpp"
#include "Widget/ModelExplorerWidget.hpp" //includes MNBVContext as well
#include "Editor.hpp"//We use editor here so we need to finish the forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::Widget::ModelHierarchyWidget::Awake()
{
	
}

char* ECellEngine::Editor::Widget::ModelHierarchyWidget::GetNodeTypeName() noexcept
{
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_ArithmeticNodes))
	{
		return "Arithmetic";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_AssetNodes))
	{
		return "Asset";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Equations))
	{
		return "Equation";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_LinePlotNodes))
	{
		return "LinePlot";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_LogicNodes))
	{
		return "Logic";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_ModifyDataStateValueEventNodes))
	{
		return "ModifyDataStateValueEvent";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Reactions))
	{
		return "Reaction";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Parameters))
	{
		return "Parameter";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_SolverNodes))
	{
		return "Solver";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Species))
	{
		return "Species";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_TimeNodes))
	{
		return "Time";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_TriggerNodes))
	{
		return "Trigger";
	}
	if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_ValueFloatNodes))
	{
		return "ValueFloat";
	}
	return "Unknown";
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::Draw()
{
	if (ImGui::Begin("Model Hierarchy"))
	{
		Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Background);

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			if (ImGui::IsWindowHovered())
			{
				ImGui::OpenPopup("HierarchyContextMenu");
				hierarchyLevel = hierarchyLevelAccumulator;
			}
		}

		DrawHierarchy();

		DrawContextMenu();

		HandleDnD();

		Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Background);
	}
	ImGui::End();
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawContextMenu()
{
	if (ImGui::BeginPopup("HierarchyContextMenu"))
	{
		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Background))
		{
			if (ImGui::MenuItem("Add Simulation"))
			{
				editor->engine.GetCommandsManager().ProcessCommand("addSimulation", ECellEngine::IO::EmptyCommandArgs());

				hierarchyLevel = HierarchyLevel_None;
			}
		}

		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Simulation))
		{
			if (ImGui::MenuItem("Erase Simulation"))
			{
				ECellEngine::Core::Simulation* _simulation = (ECellEngine::Core::Simulation*)ctxtNodePayload;

				//First erase all MNBVContexts under this simulation
				unsigned short _mnbvCtxtCount = mnbvCtxtIdx;
				while (_mnbvCtxtCount < mnbvCtxts->size() && mnbvCtxts->at(_mnbvCtxtCount).simulation->id == _simulation->id)
				{					
					editor->GetCommandsManager().ProcessCommand("eraseMNBVCtxt",
						ECellEngine::Editor::IO::EraseMNBVContextCommandArgs({ _mnbvCtxtCount }));
					
					_mnbvCtxtCount++;
				}

				//Then erase the Simulation
				editor->engine.GetCommandsManager().ProcessCommand("eraseSimulation",
										ECellEngine::IO::SimulationCommandArgs({ _simulation->id }));

				hierarchyLevel = HierarchyLevel_None;
			}
		}

		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_DataState))
		{
			ImGui::Separator();

			if (ImGui::MenuItem("Clear Data State"))
			{
				//First delete all the nodes that use elements of the data state
				for (unsigned short i = 0; i < (unsigned short)mnbvCtxts->size(); ++i)
				{
					if(mnbvCtxts->at(i).simulation->id == simuManager.GetSimulation(simuIdx)->id)
					{
						//Then delete all the nodes that use elements of the data state
						editor->GetCommandsManager().ProcessCommand("eraseAllNodesOfType",
							ECellEngine::Editor::IO::EraseAllNodesOfTypeCommandArgs({ i, "Asset" }));
						editor->GetCommandsManager().ProcessCommand("eraseAllNodesOfType",
							ECellEngine::Editor::IO::EraseAllNodesOfTypeCommandArgs({ i, "Species" }));
						editor->GetCommandsManager().ProcessCommand("eraseAllNodesOfType",
							ECellEngine::Editor::IO::EraseAllNodesOfTypeCommandArgs({ i, "Parameter" }));
						editor->GetCommandsManager().ProcessCommand("eraseAllNodesOfType",
							ECellEngine::Editor::IO::EraseAllNodesOfTypeCommandArgs({ i, "Equation" }));
						editor->GetCommandsManager().ProcessCommand("eraseAllNodesOfType",
							ECellEngine::Editor::IO::EraseAllNodesOfTypeCommandArgs({ i, "Reaction" }));
						editor->GetCommandsManager().ProcessCommand("eraseAllNodesOfType",
							ECellEngine::Editor::IO::EraseAllNodesOfTypeCommandArgs({ i, "Logic" }));
						editor->GetCommandsManager().ProcessCommand("eraseAllNodesOfType",
							ECellEngine::Editor::IO::EraseAllNodesOfTypeCommandArgs({ i, "ModifyDataStateValueEvent" }));
						editor->GetCommandsManager().ProcessCommand("eraseAllNodesOfType",
							ECellEngine::Editor::IO::EraseAllNodesOfTypeCommandArgs({ i, "Trigger" }));
					}
				}

				//Then clear the data state
				editor->engine.GetCommandsManager().ProcessCommand("clearDataState",
					ECellEngine::IO::SimulationCommandArgs({ simuManager.GetSimulation(simuIdx)->id }));

				hierarchyLevel = HierarchyLevel_None;
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Destructive action. This will also delete all nodes in all the contexts that use elements of the data state.");
			}
		}

		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_MNBVCtxts))
		{
			ImGui::Separator();

			if (ImGui::MenuItem("Add Context"))
			{
				editor->GetCommandsManager().ProcessCommand("addMNBVCtxt",
					ECellEngine::Editor::IO::AddMNBVContextCommandArgs({ simuManager.GetSimulation(simuIdx)->id }));

				hierarchyLevel = HierarchyLevel_None;
			}
		}

		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_MNBVCtxt))
		{
			if (ImGui::MenuItem("Erase Context"))
			{
				editor->GetCommandsManager().ProcessCommand("eraseMNBVCtxt",
					ECellEngine::Editor::IO::EraseMNBVContextCommandArgs({ mnbvCtxtIdx }));

				hierarchyLevel = HierarchyLevel_None;
			}
		}
		
		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Leafs))
		{
			ImGui::Separator();
			if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_MNBVCtxt))
			{
				if (ImGui::MenuItem("Erase Nodes List"))
				{
					editor->GetCommandsManager().ProcessCommand("eraseAllNodesOfType",
						ECellEngine::Editor::IO::EraseAllNodesOfTypeCommandArgs({ mnbvCtxtIdx, GetNodeTypeName() }));

					hierarchyLevel = HierarchyLevel_None;
				}
			}
		}

		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Leaf))
		{
			ImGui::Separator();
			if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_MNBVCtxt))
			{
				if (ImGui::MenuItem("Erase Node"))
				{
					editor->GetCommandsManager().ProcessCommand("eraseNode",
						ECellEngine::Editor::IO::EraseNodeCommandArgs({ mnbvCtxtIdx, GetNodeTypeName(), (std::size_t)((Utility::MNBV::NodeData*)ctxtNodePayload)->id }));
				
					hierarchyLevel = HierarchyLevel_None;
				}
				
				if (ImGui::MenuItem("Focus Node"))
				{
					editor->GetCommandsManager().ProcessCommand("focusNode",
						ECellEngine::Editor::IO::FocusNodeCommandArgs(
							{ mnbvCtxtIdx, (std::size_t)((Utility::MNBV::NodeData*)ctxtNodePayload)->id}));

					hierarchyLevel = HierarchyLevel_None;
				}
			}
		}

		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_MNBVCtxt) ||
			Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Simulation))
		{
			ImGui::Separator();
			if (ImGui::MenuItem("Rename"))
			{
				Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);

				hierarchyLevel = HierarchyLevel_None;
			}
		}
		ImGui::EndPopup();		
	}
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawHierarchy()
{
	globalNodeCount = 0;
	mnbvCtxtCount = 0;
	simuCount = 0;
	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Simulation);
	for (std::vector<std::unique_ptr<Core::Simulation>>::iterator it = simuManager.GetSimulations().begin(); it != simuManager.GetSimulations().end(); it++)
	{
		globalNodeCount++;
		ImGui::PushID(it->get());
		//If users is trying to rename this asset.
		if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingInProgress) && globalNodeIdx == globalNodeCount)
		{
			std::strcpy(renamingBuffer, it->get()->GetName());
			if (TreeNodeRename(renamingBuffer))
			{
				it->get()->SetName(renamingBuffer);
			}

			ImGui::PushID(it->get()->GetName());
			ImGui::Indent(ImGui::GetStyle().IndentSpacing);
			DrawSimulationHierarchy(it->get());
			ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
			ImGui::PopID();

		}
		else
		{
			bool nodeOpen = ImGui::TreeNodeEx(it->get()->GetName());

			//If user performs the action to rename this node.
			if (ImGui::IsItemFocused() && ImGui::IsKeyDown(ImGuiKey_F2))
			{
				TrackContextItem();
				Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);
			}

			//If user performs the action to open the context menu of this node.
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				TrackContextItem();
				ctxtNodePayload = it->get();
				hierarchyLevel = hierarchyLevelAccumulator;
			}

			if (nodeOpen)
			{
				DrawSimulationHierarchy(it->get());
				ImGui::TreePop();
			}

		}
		simuCount++;
		ImGui::PopID();
	}
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Simulation);
}

template<typename LeafType, typename NameGetterType, typename NameSetterType>
void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawHierarchyLeafsList(const char* _leafsListName,
	std::vector<LeafType>& _leafs, NameGetterType& _nameGetter, NameSetterType& _nameSetter)
{
	globalNodeCount++;
	if (_leafs.size() > 0)
	{
		Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Leafs);
		
		bool leafsOpen = ImGui::TreeNodeEx(_leafsListName);
		//If user performs the action to open the context menu of this node.
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			TrackContextItem();
			hierarchyLevel = hierarchyLevelAccumulator;
		}

		if (leafsOpen)
		{
			Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Leaf);
			for (std::vector<LeafType>::iterator leafIt = _leafs.begin(); leafIt != _leafs.end(); leafIt++)
			{
				globalNodeCount++;
				//If users is trying to rename this asset.
				if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingInProgress) && globalNodeIdx == globalNodeCount)
				{
					std::strcpy(renamingBuffer, _nameGetter(leafIt));
					if (TreeNodeRename(renamingBuffer))
					{
						_nameSetter(leafIt, renamingBuffer);
					}
				}
				else
				{
					ImGui::TreeNodeEx(_nameGetter(leafIt), leafNodeFlags);

					//If user performs the action to rename this node.
					if (ImGui::IsItemFocused() && ImGui::IsKeyDown(ImGuiKey_F2))
					{
						TrackContextItem();
						Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);
					}

					//If user performs the action to open the context menu of this node.
					if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
					{
						TrackContextItem();
						ctxtNodePayload = &*leafIt;
						hierarchyLevel = hierarchyLevelAccumulator;
					}

					if (ImGui::BeginDragDropSource())
					{
						TrackContextItem();
						ctxtNodePayload = &*leafIt;
						hierarchyLevel = hierarchyLevelAccumulator;
						Util::SetFlag(hierarchyCtxt, HierarchyContext_DnD);
						ImGui::EndDragDropSource();
					}

					ImGui::TreePop();
				}
			}
			Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Leaf);
			ImGui::TreePop();
		}
		Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Leafs);
	}
}

template<typename LeafKeyType, typename LeafType, typename NameGetterType, typename NameSetterType>
void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawHierarchyLeafsUMap(const char* _leafsListName,
	const std::unordered_map<LeafKeyType, LeafType>& _leafs, NameGetterType& _nameGetter, NameSetterType& _nameSetter)
{
	globalNodeCount++;
	if (_leafs.size() > 0)
	{
		Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Leafs);
		bool leafsOpen = ImGui::TreeNodeEx(_leafsListName);
		//If user performs the action to open the context menu of this node.
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			TrackContextItem();
			hierarchyLevel = hierarchyLevelAccumulator;
		}

		if (leafsOpen)
		{
			Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Leaf);
			for (std::unordered_map<LeafKeyType, LeafType>::const_iterator leafIt = _leafs.begin(); leafIt != _leafs.end(); leafIt++)
			{
				globalNodeCount++;

				//If users is trying to rename this asset.
				if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingInProgress) && globalNodeIdx == globalNodeCount)
				{
					std::strcpy(renamingBuffer, _nameGetter(leafIt->second));
					if (TreeNodeRename(renamingBuffer))
					{
						_nameSetter(leafIt->second, renamingBuffer);
					}
				}
				else
				{
					ImGui::TreeNodeEx(_nameGetter(leafIt->second), leafNodeFlags);

					//If user performs the action to rename this node.
					if (ImGui::IsItemFocused() && ImGui::IsKeyDown(ImGuiKey_F2))
					{
						TrackContextItem();
						Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);
					}

					//If user performs the action to open the context menu of this node.
					if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
					{
						TrackContextItem();
						ctxtNodePayload = &(leafIt->second);
						hierarchyLevel = hierarchyLevelAccumulator;
					}

					if (ImGui::BeginDragDropSource())
					{
						TrackContextItem();
						ctxtNodePayload = &(leafIt->second);
						hierarchyLevel = hierarchyLevelAccumulator;
						Util::SetFlag(hierarchyCtxt, HierarchyContext_DnD);
						ImGui::EndDragDropSource();
					}

					ImGui::TreePop();
				}
			}
			Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Leaf);
			ImGui::TreePop();
		}
		Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Leafs);
	}
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawMNBVCtxtHierarchy(MNBV::ModelNodeBasedViewerContext& _mnbvCtxt)
{
	static NameGetterByIt nameGetterByIt;
	static NameSetterByIt nameSetterByIt;

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_ArithmeticNodes);
	DrawHierarchyLeafsList("Arithmetic Nodes", _mnbvCtxt.arithmeticOperationNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_ArithmeticNodes);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_AssetNodes);
	DrawHierarchyLeafsList("Asset Nodes", _mnbvCtxt.assetNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_AssetNodes);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Equations);
	DrawHierarchyLeafsList("Equation Nodes", _mnbvCtxt.equationNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Equations);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_LinePlotNodes);
	DrawHierarchyLeafsList("Line Plot Nodes", _mnbvCtxt.linePlotNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_LinePlotNodes);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_LogicNodes);
	DrawHierarchyLeafsList("Logic Nodes", _mnbvCtxt.logicOperationNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_LogicNodes);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_ModifyDataStateValueEventNodes);
	DrawHierarchyLeafsList("Modify Data State Event Nodes", _mnbvCtxt.modifyDataStateValueEventNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_ModifyDataStateValueEventNodes);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Reactions);
	DrawHierarchyLeafsList("Reaction Nodes", _mnbvCtxt.reactionNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Reactions);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Parameters);
	DrawHierarchyLeafsList("Parameter Nodes", _mnbvCtxt.parameterNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Parameters);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_SolverNodes);
	DrawHierarchyLeafsList("Solver Nodes", _mnbvCtxt.solverNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_SolverNodes);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Species);
	DrawHierarchyLeafsList("Species Nodes", _mnbvCtxt.speciesNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Species);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_TimeNodes);
	DrawHierarchyLeafsList("Time Nodes", _mnbvCtxt.timeNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_TimeNodes);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_TriggerNodes);
	DrawHierarchyLeafsList("Trigger Nodes", _mnbvCtxt.triggerNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_TriggerNodes);

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_ValueFloatNodes);
	DrawHierarchyLeafsList("Value Float Nodes", _mnbvCtxt.valueFloatNodes, nameGetterByIt, nameSetterByIt);
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_ValueFloatNodes);
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawSimulationHierarchy(ECellEngine::Core::Simulation* _simulation)
{
	static NameGetterBySPtr nameGetterBySPtr;
	static NameSetterBySPtr nameSetterBySPtr;
	
	globalNodeCount++;
	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_DataState);

	bool openDataState = ImGui::TreeNodeEx("Data State");
	//If user performs the action to open the context menu of this node.
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
	{
		TrackContextItem();
		hierarchyLevel = hierarchyLevelAccumulator;
	}

	if (openDataState)
	{
		Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Equations);
		DrawHierarchyLeafsUMap("Equations", _simulation->GetDataState().GetEquations(), nameGetterBySPtr, nameSetterBySPtr);
		Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Equations);

		Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Parameters);
		DrawHierarchyLeafsUMap("Parameters", _simulation->GetDataState().GetParameters(), nameGetterBySPtr, nameSetterBySPtr);
		Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Parameters);

		Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Reactions);
		DrawHierarchyLeafsUMap("Reactions", _simulation->GetDataState().GetReactions(), nameGetterBySPtr, nameSetterBySPtr);
		Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Reactions);

		Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Species);
		DrawHierarchyLeafsUMap("Species", _simulation->GetDataState().GetAllSpecies(), nameGetterBySPtr, nameSetterBySPtr);
		Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Species);

		ImGui::TreePop();
	}
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_DataState);

	globalNodeCount++;

	Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_MNBVCtxts);
	bool nodeOpen = ImGui::TreeNodeEx("Contexts");
	//If user performs the action to open the context menu of this node.
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
	{
		TrackContextItem();
		hierarchyLevel = hierarchyLevelAccumulator;
	}

	if (nodeOpen)
	{
		Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_MNBVCtxt);
		while (mnbvCtxtCount < mnbvCtxts->size() && mnbvCtxts->at(mnbvCtxtCount).simulation->id == _simulation->id)
		{
			globalNodeCount++;
			MNBV::ModelNodeBasedViewerContext& mnbvCtxt = mnbvCtxts->at(mnbvCtxtCount); //there is a reassignment of the same value here for the first iteration
			ImGui::PushID(mnbvCtxtCount);
			//If users is trying to rename this asset.
			if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingInProgress) && globalNodeIdx == globalNodeCount)
			{
				std::strcpy(renamingBuffer, mnbvCtxt.name);
				if (TreeNodeRename(renamingBuffer))
				{
					mnbvCtxt.SetName(renamingBuffer);
				}

				ImGui::PushID(mnbvCtxt.GetName());
				ImGui::Indent(ImGui::GetStyle().IndentSpacing);
				DrawMNBVCtxtHierarchy(mnbvCtxt);
				ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				ImGui::PopID();

			}
			else
			{
				bool nodeOpen = ImGui::TreeNodeEx(mnbvCtxt.GetName());

				//If user performs the action to rename this node.
				if (ImGui::IsItemFocused() && ImGui::IsKeyDown(ImGuiKey_F2))
				{
					TrackContextItem();
					Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);
				}

				//If user performs the action to open the context menu of this node.
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					TrackContextItem();
					hierarchyLevel = hierarchyLevelAccumulator;
				}

				if (nodeOpen)
				{
					DrawMNBVCtxtHierarchy(mnbvCtxt);
					ImGui::TreePop();
				}

			}
			mnbvCtxtCount++;
			ImGui::PopID();
		}
		Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_MNBVCtxt);
		ImGui::TreePop();
	}
	Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_MNBVCtxts);	
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::HandleDnD()
{
	if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_DnD))
	{
		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_DataState))
		{
			if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Equations))
			{
				std::shared_ptr<ECellEngine::Maths::Equation>* eq_sptr = (std::shared_ptr<ECellEngine::Maths::Equation>*)(ctxtNodePayload);
				ImGui::SetTooltip("Equation: %s", eq_sptr->get()->GetName());
				ECellEngine::Editor::Utility::DragAndDrop::SetPayload("Hierarchy_Equation_DND", eq_sptr);
			}

			if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Reactions))
			{
				std::shared_ptr<ECellEngine::Data::Reaction>* rxn_sptr = (std::shared_ptr<ECellEngine::Data::Reaction>*)(ctxtNodePayload);
				ImGui::SetTooltip("Reaction: %s", rxn_sptr->get()->GetName());
				ECellEngine::Editor::Utility::DragAndDrop::SetPayload("Hierarchy_Reaction_DND", rxn_sptr);
			}

			if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Parameters))
			{
				std::shared_ptr<ECellEngine::Data::Parameter>* param_sptr = (std::shared_ptr<ECellEngine::Data::Parameter>*)(ctxtNodePayload);
				ImGui::SetTooltip("Parameter: %s", param_sptr->get()->GetName());
				ECellEngine::Editor::Utility::DragAndDrop::SetPayload("Hierarchy_Parameter_DND", param_sptr);
			}

			if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Species))
			{
				std::shared_ptr<ECellEngine::Data::Species>* sp_sptr = (std::shared_ptr<ECellEngine::Data::Species>*)(ctxtNodePayload);
				ImGui::SetTooltip("Species: %s", sp_sptr->get()->GetName());
				ECellEngine::Editor::Utility::DragAndDrop::SetPayload("Hierarchy_Species_DND", sp_sptr);
			}

		}
		Util::ClearFlag(hierarchyCtxt, HierarchyContext_DnD);
	}
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::TrackContextItem() noexcept
{
	globalNodeIdx = globalNodeCount;
	simuIdx = simuCount;
	mnbvCtxtIdx = mnbvCtxtCount;
}

bool ECellEngine::Editor::Widget::ModelHierarchyWidget::TreeNodeRename(char* _nameBuffer)
{
	bool renamedConfirmed = false;
	if (ImGui::InputText("###name", _nameBuffer, 64, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		renamedConfirmed = true;
		Util::ClearFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);
	}

	if (ImGui::IsItemActive())
	{
		ImGui::SetTooltip("Press ENTER to confirm.");
	}

	//Stops renaming if user clicks outside of the input frame
	if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		Util::ClearFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);
	}

	return renamedConfirmed;
}