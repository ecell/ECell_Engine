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

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawMNBVCtxtHierarchy()
{
	nodeID++;
	ImGui::PushID(nodeID);
	if (ImGui::TreeNode("Assets"))
	{
		//Display the list of loaded assets
		//for (std::size_t idx = 0; idx < _simulation->GetModules().size(); idx++)
		//{
			//Id from Drag & Drop
			//ImGui::PushID(nodeID++);
			////If users is trying to rename this asset.
			//if (renamingInProgress && contextNodeIdx == nodeID)
			//{
			//	std::strcpy(renamingBuffer, _simulation->GetModule(idx)->GetName());
			//	TreeNodeRenaming(renamingBuffer);
			//}

			//else//no renaming in progress for this item.
			//{
			//	//Leaf Node to display the name of the asset.
			//	ImGui::TreeNodeEx(_simulation->GetName(), leafNodeFlags);
			//	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			//	{
			//		renamingInProgress = true;
			//		contextNodeIdx = nodeID;
			//	}
			//}

			// The Asset name can be draged.
			//if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			//{
			//	// Set payload to carry the index of our item (could be anything)
			//	ImGui::SetDragDropPayload("DND_ASSET", &idx, sizeof(std::size_t));

			//	// Display preview (could be anything, e.g. when dragging an image we could decide to display
			//	// the filename and a small preview of the image, etc.)
			//	ImGui::Text(_simulation->GetModule(idx)->GetName());
			//	ImGui::EndDragDropSource();
			//}

			//Pop Id of the Drag & Drop Item
			//ImGui::PopID();
		//}

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
			DrawMNBVCtxtHierarchy();
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
			bool nodeOpen = ImGui::TreeNodeEx(mnbvCtxts->at(_out_mnbvCtxtStartIdx).name, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_NoTreePushOnOpen);

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
				DrawMNBVCtxtHierarchy();
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