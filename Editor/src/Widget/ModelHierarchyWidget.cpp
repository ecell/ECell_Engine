#include "IO/CommandArgs.hpp"
#include "Util/BitwiseOperationsUtility.hpp"
#include "Widget/ModelExplorerWidget.hpp" //includes MNBVContext as well
#include "Editor.hpp"//We use editor here so we need to finish the forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::Widget::ModelHierarchyWidget::Awake()
{
	
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
				openedNodes.push_back(0);

				hierarchyLevel = HierarchyLevel_None;
			}
		}

		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Simulation))
		{
			ImGui::Separator();

			if (ImGui::MenuItem("Delete Simulation"))
			{
				ECellEngine::Logging::Logger::LogDebug("Clicked on Delete Simulation");

				//TODO: Delete Simulation
				//TODO: Delete MNBVContexts

				hierarchyLevel = HierarchyLevel_None;
			}
		}

		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_MNBVCtxts))
		{
			ImGui::Separator();

			if (ImGui::MenuItem("Add Context"))
			{
				editor->GetCommandsManager().ProcessCommand("addMNBVCtxt", ECellEngine::Editor::IO::AddMNBVContextCommandArgs({ simuManager.GetSimulation(simuIdx)->id }));

				hierarchyLevel = HierarchyLevel_None;
			}
		}

		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_MNBVCtxt))
		{
			ImGui::Separator();

			if (ImGui::MenuItem("Delete Context"))
			{
				ECellEngine::Logging::Logger::LogDebug("Clicked on Delete Context");

				hierarchyLevel = HierarchyLevel_None;
			}

			/*if (ImGui::MenuItem("Rename Context"))
			{
				Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);
				globalNodeIdx = simuIdx;

				hierarchyLevel = HierarchyLevel_None;
			}*/
		}
		
		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Leafs))
		{
			ImGui::Separator();
			ImGui::Text("This is the context menu of the leafs");

			hierarchyLevel = HierarchyLevel_None;
		}

		if (Util::IsFlagSet(hierarchyLevel, HierarchyLevel_Leaf))
		{
			ImGui::Separator();
			ImGui::Text("This is the context menu of the leaf");

			hierarchyLevel = HierarchyLevel_None;
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

template<typename LeafType, typename NameAccessorType>
void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawHierarchyLeafsList(const char* _leafsListName, MNBV::ModelNodeBasedViewerContext& _mnbvCtxt, const std::vector<LeafType>& _leafs, NameAccessorType& _nameAccessor)
{
	globalNodeCount++;
	if (_leafs.size() > 0)
	{
		Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Leafs);
		if (ImGui::TreeNodeEx(_leafsListName))
		{
			Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Leaf);
			for (auto _leaf : _leafs)
			{
				globalNodeCount++;
				ImGui::TreeNodeEx(_nameAccessor(_leaf), leafNodeFlags);
				ImGui::TreePop();
			}
			Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Leaf);
			ImGui::TreePop();
		}
		Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Leafs);
	}
}

template<typename LeafKeyType, typename LeafType, typename NameAccessorType>
void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawHierarchyLeafsUMap(const char* _leafsListName, MNBV::ModelNodeBasedViewerContext& _mnbvCtxt, const std::unordered_map<LeafKeyType, LeafType>& _leafs, NameAccessorType& _nameAccessor)
{
	globalNodeCount++;
	if (_leafs.size() > 0)
	{
		Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Leafs);
		if (ImGui::TreeNodeEx(_leafsListName))
		{
			Util::SetFlag(hierarchyLevelAccumulator, HierarchyLevel_Leaf);
			for (auto [_leafKey, _leaf] : _leafs)
			{
				globalNodeCount++;
				ImGui::TreeNodeEx(_nameAccessor(_leaf), leafNodeFlags);
				ImGui::TreePop();
			}
			Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Leaf);
			ImGui::TreePop();
		}
		Util::ClearFlag(hierarchyLevelAccumulator, HierarchyLevel_Leafs);
	}
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawMNBVCtxtHierarchy(MNBV::ModelNodeBasedViewerContext& _mnbvCtxt)
{
	static NameAccessorByRef nameAccessorByRef;

	DrawHierarchyLeafsList("Arithmetic Nodes", _mnbvCtxt, _mnbvCtxt.arithmeticOperationNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Asset Nodes", _mnbvCtxt, _mnbvCtxt.assetNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Equation Nodes", _mnbvCtxt, _mnbvCtxt.equationNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Line Plot Nodes", _mnbvCtxt, _mnbvCtxt.linePlotNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Logic Nodes", _mnbvCtxt, _mnbvCtxt.logicOperationNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Modify Data State Event Nodes", _mnbvCtxt, _mnbvCtxt.modifyDataStateValueEventNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Reaction Nodes", _mnbvCtxt, _mnbvCtxt.reactionNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Parameter Nodes", _mnbvCtxt, _mnbvCtxt.parameterNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Solver Nodes", _mnbvCtxt, _mnbvCtxt.solverNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Species Nodes", _mnbvCtxt, _mnbvCtxt.speciesNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Time Nodes", _mnbvCtxt, _mnbvCtxt.timeNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Trigger Nodes", _mnbvCtxt, _mnbvCtxt.triggerNodes, nameAccessorByRef);

	DrawHierarchyLeafsList("Value Nodes", _mnbvCtxt, _mnbvCtxt.valueFloatNodes, nameAccessorByRef);
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawSimulationHierarchy(ECellEngine::Core::Simulation* _simulation)
{
	static NameAccessorBySPtr nameAccessorBySPtr;
	MNBV::ModelNodeBasedViewerContext& mnbvCtxt = mnbvCtxts->at(mnbvCtxtCount);
	globalNodeCount++;
	if (ImGui::TreeNodeEx("Data State"))
	{
		DrawHierarchyLeafsUMap("Equations", mnbvCtxt, mnbvCtxt.simulation->GetDataState().GetEquations(), nameAccessorBySPtr);

		DrawHierarchyLeafsUMap("Parameters", mnbvCtxt, mnbvCtxt.simulation->GetDataState().GetParameters(), nameAccessorBySPtr);

		DrawHierarchyLeafsUMap("Reactions", mnbvCtxt, mnbvCtxt.simulation->GetDataState().GetReactions(), nameAccessorBySPtr);

		DrawHierarchyLeafsUMap("Species", mnbvCtxt, mnbvCtxt.simulation->GetDataState().GetAllSpecies(), nameAccessorBySPtr);

		ImGui::TreePop();
	}

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
		while (mnbvCtxtCount < mnbvCtxts->size() && mnbvCtxt.simulation->id == _simulation->id)
		{
			globalNodeCount++;
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