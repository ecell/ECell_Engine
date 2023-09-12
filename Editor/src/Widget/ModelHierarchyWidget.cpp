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
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			if (ImGui::IsWindowHovered())
			{
				Util::SetFlag(hierarchyCtxt, HierarchyContext_RightClick);
				Util::SetFlag(hierarchyCtxt, HierarchyContext_RightClickOnBackground);
			}
		}

		DrawHierarchy();

		if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RightClickOnBackground))
		{
			ImGui::OpenPopup("HierarchyContext");
		}

		else if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RightClickOnSimulationNode))
		{
			ImGui::OpenPopup("HierarchySimuContext");
		}

		DrawContextMenu();


	}
	ImGui::End();
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawContextMenu()
{
	Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClick);
	
	if (ImGui::BeginPopup("HierarchyContext"))
	{
		ImGui::Text("Hierarchy Context Menu");
		ImGui::Separator();

		if (ImGui::MenuItem("Add Simulation"))
		{
			editor->engine.GetCommandsManager().ProcessCommand("addSimulation", ECellEngine::IO::EmptyCommandArgs());
			openedNodes.push_back(0);
		}

		if (ImGui::MenuItem("Delete Simulation"))
		{
			ECellEngine::Logging::Logger::LogDebug("Clicked on Delete Simulation");

			//TODO: Delete Simulation
			//TODO: Delete MNBVContexts
		}
		Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnBackground);
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("HierarchySimuContext"))
	{
		ImGui::Text("Simulation Context Menu");
		ImGui::Separator();

		if (ImGui::MenuItem("Add Context"))
		{
			editor->GetCommandsManager().ProcessCommand("addMNBVCtxt", ECellEngine::Editor::IO::AddMNBVContextCommandArgs({ simuManager.GetSimulation(contextNodeIdx)->id }));
		}

		if (ImGui::MenuItem("Delete Context"))
		{
			ECellEngine::Logging::Logger::LogDebug("Clicked on Delete Context");
		}
		Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnSimulationNode);
		ImGui::EndPopup();
	}
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawHierarchy()
{
	nodeID = 0;
	int mnbvCtxtCount = 0;
	int simuCount = 0;
	for (std::vector<std::unique_ptr<Core::Simulation>>::iterator it = simuManager.GetSimulations().begin(); it != simuManager.GetSimulations().end(); it++)
	{
		nodeID++;
		ImGui::PushID(nodeID);
		//If users is trying to rename this asset.
		if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingInProgress) && contextNodeIdx == nodeID)
		{
			std::strcpy(renamingBuffer, it->get()->GetName());
			if (TreeNodeRename(renamingBuffer))
			{
				it->get()->SetName(renamingBuffer);
				Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingDone);
			}

			ImGui::Indent(ImGui::GetStyle().IndentSpacing);
			DrawSimulationHierarchy(mnbvCtxtCount, it->get());
			ImGui::Unindent(ImGui::GetStyle().IndentSpacing);

		}
		else
		{
			//Retrieves the correct open state of the node that might have been just renamed
			//By default, a renamed node will be closed because chaging the name changes it's ID
			//in ImGui and breaks the internal tracking of it's open state 
			if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingDone) && contextNodeIdx == nodeID)
			{
				Util::ClearFlag(hierarchyCtxt, HierarchyContext_RenamingDone);
				ImGui::SetNextItemOpen(Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamedNodeWasOpen));
				Util::ClearFlag(hierarchyCtxt, HierarchyContext_RenamedNodeWasOpen);
			}
			bool nodeOpen = ImGui::TreeNodeEx(it->get()->GetName(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_NoTreePushOnOpen);

			if (ImGui::IsItemHovered())
			{
				//If user performs the action to rename this node.
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);
					contextNodeIdx = nodeID;
					if (nodeOpen)
					{
						//Save the open state of the node before renaming it
						Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamedNodeWasOpen);
					}
				}

				//If user performs the action to open the context menu of this node.
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					contextNodeIdx = simuCount;
					Util::SetFlag(hierarchyCtxt, HierarchyContext_RightClickOnSimulationNode);
					Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnMNBVCtxtNode);
					Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnBackground);
				}
			}

			if (nodeOpen)
			{
				//We manage indentation ourselves because we use ImGuiTreeNodeFlags_NoTreePushOnOpen
				//The reason we use this flag is to avoid disrupting the internal tracking of the open state of the node
				//The open state can only be tracked if the same tree structure stays between frames which
				//is not the case when we rename the tree node (replaced by an input field temporarily)
				ImGui::Indent(ImGui::GetStyle().IndentSpacing);
				DrawSimulationHierarchy(mnbvCtxtCount, it->get());
				ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
			}

		}
		simuCount++;
		ImGui::PopID();
	}
}

template<typename LeafType>
void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawHierarchyLeafsList(const char* _leafsListName, MNBV::ModelNodeBasedViewerContext& _mnbvCtxt, const std::vector<LeafType>& _leafs)
{
	if (_mnbvCtxt.simulation->GetModules().size() > 0)
	{
		nodeID++;
		ImGui::PushID(nodeID);
		if (ImGui::TreeNodeEx(_leafsListName, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_NoTreePushOnOpen))
		{
			for (auto _leaf : _leafs)
			{
				nodeID++;
				ImGui::PushID(nodeID);
				ImGui::Indent(ImGui::GetStyle().IndentSpacing);
				ImGui::TreeNodeEx(_leaf->GetName(), leafNodeFlags);
				ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				ImGui::PopID();
			}
		}
		ImGui::PopID();
	}
}

template<typename LeafKeyType, typename LeafType>
void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawHierarchyLeafsUMap(const char* _leafsListName, MNBV::ModelNodeBasedViewerContext& _mnbvCtxt, const std::unordered_map<LeafKeyType, LeafType>& _leafs)
{
	if (_mnbvCtxt.simulation->GetModules().size() > 0)
	{
		nodeID++;
		ImGui::PushID(nodeID);
		if (ImGui::TreeNodeEx(_leafsListName, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_NoTreePushOnOpen))
		{
			for (auto [_leafKey, _leaf] : _leafs)
			{
				nodeID++;
				ImGui::PushID(nodeID);
				ImGui::Indent(ImGui::GetStyle().IndentSpacing);
				ImGui::TreeNodeEx(_leaf->GetName(), leafNodeFlags);
				ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				ImGui::PopID();
			}
		}
		ImGui::PopID();
	}
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawMNBVCtxtHierarchy(MNBV::ModelNodeBasedViewerContext& _mnbvCtxt)
{
	//nodeID++;
	ImGui::PushID(nodeID);
	if (ImGui::TreeNodeEx("Data Nodes", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		DrawHierarchyLeafsList("Module Nodes", _mnbvCtxt, _mnbvCtxt.simulation->GetModules());

		DrawHierarchyLeafsUMap("Equation Nodes", _mnbvCtxt, _mnbvCtxt.simulation->GetDataState().GetEquations());

		DrawHierarchyLeafsUMap("Parameter Nodes", _mnbvCtxt, _mnbvCtxt.simulation->GetDataState().GetParameters());

		DrawHierarchyLeafsUMap("Reaction Nodes", _mnbvCtxt, _mnbvCtxt.simulation->GetDataState().GetReactions());

		DrawHierarchyLeafsUMap("Species Nodes", _mnbvCtxt, _mnbvCtxt.simulation->GetDataState().GetAllSpecies());

		ImGui::TreePop();
	}
	ImGui::PopID();
	
	ImGui::PushID(nodeID);
	if (ImGui::TreeNodeEx("Transform Nodes", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		//if (_mnbvCtxt.simulation->GetModules().size() > 0)
		//{
		//	nodeID++;
		//	ImGui::PushID(nodeID);
		//	if (ImGui::TreeNodeEx("Module Nodes", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_NoTreePushOnOpen))
		//	{
		//		for (auto _module : _mnbvCtxt.simulation->GetModules())
		//		{
		//			//nodeID++;
		//			ImGui::PushID(nodeID);
		//			ImGui::TreeNodeEx(_module->GetName(), leafNodeFlags);
		//			//ImGui::TreePop();
		//			ImGui::PopID();
		//		}

		//		//ImGui::TreePop();
		//	}
		//	ImGui::PopID();
		//}

		//DrawHierarchyLeafsList("Module Nodes", _mnbvCtxt, _mnbvCtxt.simulation->GetModules());

		ImGui::TreePop();
	}
	ImGui::PopID();
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawSimulationHierarchy(int& _out_mnbvCtxtStartIdx, ECellEngine::Core::Simulation* _simulation)
{
	while (_out_mnbvCtxtStartIdx < mnbvCtxts->size() && mnbvCtxts->at(_out_mnbvCtxtStartIdx).simulation->id == _simulation->id)
	{
		nodeID++;
		ImGui::PushID(nodeID);
		//If users is trying to rename this asset.
		if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingInProgress) && contextNodeIdx == nodeID)
		{
			std::strcpy(renamingBuffer, mnbvCtxts->at(_out_mnbvCtxtStartIdx).name);
			if (TreeNodeRename(renamingBuffer))
			{
				mnbvCtxts->at(_out_mnbvCtxtStartIdx).SetName(renamingBuffer);
				Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingDone);
			}

			ImGui::Indent(ImGui::GetStyle().IndentSpacing);
			DrawMNBVCtxtHierarchy(mnbvCtxts->at(_out_mnbvCtxtStartIdx));
			ImGui::Unindent(ImGui::GetStyle().IndentSpacing);

		}
		else
		{
			//Retrieves the correct open state of the node that might have been just renamed
			//By default, a renamed node will be closed because chaging the name changes it's ID
			//in ImGui and breaks the internal tracking of it's open state 
			if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingDone) && contextNodeIdx == nodeID)
			{
				Util::ClearFlag(hierarchyCtxt, HierarchyContext_RenamingDone);
				ImGui::SetNextItemOpen(Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamedNodeWasOpen));
				Util::ClearFlag(hierarchyCtxt, HierarchyContext_RenamedNodeWasOpen);
			}
			bool nodeOpen = ImGui::TreeNodeEx(mnbvCtxts->at(_out_mnbvCtxtStartIdx).GetName(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_NoTreePushOnOpen);

			if (ImGui::IsItemHovered())
			{
				//If user performs the action to rename this node.
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);
					contextNodeIdx = nodeID;
					if (nodeOpen)
					{
						//Save the open state of the node before renaming it
						Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamedNodeWasOpen);
					}
				}

				//If user performs the action to open the context menu of this node.
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					contextNodeIdx = _out_mnbvCtxtStartIdx;
					Util::SetFlag(hierarchyCtxt, HierarchyContext_RightClickOnMNBVCtxtNode);
					Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnSimulationNode);
					Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnBackground);
				}
			}

			if (nodeOpen)
			{
				ImGui::Indent(ImGui::GetStyle().IndentSpacing);
				DrawMNBVCtxtHierarchy(mnbvCtxts->at(_out_mnbvCtxtStartIdx));
				ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				//ImGui::TreePop();
			}

		}		
		_out_mnbvCtxtStartIdx++;
		ImGui::PopID();
	}
	
}

bool ECellEngine::Editor::Widget::ModelHierarchyWidget::TreeNodeRename(char* _nameBuffer)
{
	bool renamedConfirmed = false;
	if (ImGui::InputText("##name", _nameBuffer, 64, ImGuiInputTextFlags_EnterReturnsTrue))
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