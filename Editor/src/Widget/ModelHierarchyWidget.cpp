#include "IO/CommandArgs.hpp"
#include "Util/BitwiseOperationsUtility.hpp"
#include "Widget/ModelExplorerWidget.hpp"
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
		}

		if (ImGui::MenuItem("Delete Simulation"))
		{
			ECellEngine::Logging::Logger::LogDebug("Clicked on Delete Simulation");
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
			ECellEngine::Logging::Logger::LogDebug("Clicked on Add Context");
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
	for (std::vector<std::unique_ptr<Core::Simulation>>::iterator it = simuManager.GetSimulations().begin(); it != simuManager.GetSimulations().end(); it++)
	{
		ImGui::PushID(nodeID);
		//If users is trying to rename this asset.
		if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingInProgress) && contextNodeIdx == nodeID)
		{
			std::strcpy(renamingBuffer, it->get()->GetName());
			if (TreeNodeRename(renamingBuffer))
			{
				it->get()->SetName(renamingBuffer);
			}

			ImGui::Indent(ImGui::GetStyle().IndentSpacing);
			DrawSimulationHierarchy(it->get());
			ImGui::Unindent(ImGui::GetStyle().IndentSpacing);

		}
		else
		{
			bool nodeOpen = ImGui::TreeNodeEx(it->get()->GetName(), ImGuiTreeNodeFlags_OpenOnArrow);

			if (ImGui::IsItemHovered())
			{
				//If user performs the action to rename this node.
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					Util::SetFlag(hierarchyCtxt, HierarchyContext_RenamingInProgress);
					contextNodeIdx = nodeID;
				}

				//If user performs the action to open the context menu of this node.
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					Util::SetFlag(hierarchyCtxt, HierarchyContext_RightClickOnSimulationNode);
					Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnBackground);
				}
			}

			if (nodeOpen)
			{
				DrawSimulationHierarchy(it->get());
				ImGui::TreePop();
			}

		}

		ImGui::PopID();
		nodeID++;
	}
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawMNBVCtxtHierarchy()
{
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
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawSimulationHierarchy(ECellEngine::Core::Simulation* _simulation)
{
	//TODO: draw all the contexts associated to this simulation

	//TODO: the contexts must be retrieved from the Model Explorer Widget

	//The API in the Model Explorer Widget needs rework. I suggest to order the
	//contexts by simulation in the MEW. This way we can easily retrieve the contexts
	//associated to a simulation.
	//When inserting/ermoving a context, make sure to update the MEW's ctxtsPerViewer accordingly:
	// - When inserting a context, any value in the ::ctxtsPerViewer map with a key greater than the inserted context's index must be incremented by 1.
	// - When removing a context, any value in the ::ctxtsPerViewer map with a key greater than the removed context's index must be decremented by 1.

	//There is no guarentee the above instructions are exhaustive.

	//temporary
	DrawMNBVCtxtHierarchy();
	
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