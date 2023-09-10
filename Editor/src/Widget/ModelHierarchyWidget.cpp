#include "Widget/ModelExplorerWidget.hpp"
#include "Editor.hpp"//We use editor here so we need to finish the forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::Widget::ModelHierarchyWidget::Awake()
{
	
}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::Draw()
{
	if (ImGui::Begin("Model Hierarchy"))
	{
		nodeID = 0;
		for (std::vector<std::unique_ptr<Core::Simulation>>::iterator it = simuManager.GetSimulations().begin(); it != simuManager.GetSimulations().end(); it++)
		{
			ImGui::PushID(nodeID);
			//If users is trying to rename this asset.
			if (renamingInProgress && renamingIdx == nodeID)
			{
				std::strcpy(renamingBuffer, it->get()->GetName());
				if (TreeNodeRename(renamingBuffer))
				{
					it->get()->SetName(renamingBuffer);
				}

				ImGui::Indent(ImGui::GetStyle().IndentSpacing);
				DrawSimulationHierarchy(it->get());

			}
			else
			{
				bool nodeOpen = ImGui::TreeNodeEx(it->get()->GetName(), ImGuiTreeNodeFlags_OpenOnArrow);

				//If user performs the action to rename this asset.
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					renamingInProgress = true;
					renamingIdx = nodeID;
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
	ImGui::End();
}

//void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawMNBVCtxtHierarchy(ECellEngine::Editor::Widget::MNBV::ModelNodeBasedViewerContext* _ctxt)
//{
//
//}

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawSimulationHierarchy(ECellEngine::Core::Simulation* _simulation)
{
	
	if (ImGui::TreeNode("Assets"))
	{
		//Display the list of loaded assets
		for (std::size_t idx = 0; idx < _simulation->GetModules().size(); idx++)
		{
			//Id from Drag & Drop
			//ImGui::PushID(nodeID++);
			////If users is trying to rename this asset.
			//if (renamingInProgress && renamingIdx == nodeID)
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
			//		renamingIdx = nodeID;
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
		}

		ImGui::TreePop();
	}	
}

//void ECellEngine::Editor::Widget::ModelHierarchyWidget::SetSimulation(std::size_t _simulationIndex)
//{
//	simulation = ECellEngine::Core::SimulationsManager::GetSingleton().GetSimulation(_simulationIndex);
//}

bool ECellEngine::Editor::Widget::ModelHierarchyWidget::TreeNodeRename(char* _nameBuffer)
{
	bool renamedConfirmed = false;
	if (ImGui::InputText("##name", _nameBuffer, 64, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		renamedConfirmed = true;
		renamingInProgress = false;
	}

	if (ImGui::IsItemActive())
	{
		ImGui::SetTooltip("Press ENTER to confirm.");
	}

	//Stops renaming if user clicks outside of the input frame
	if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		renamingInProgress = false;
	}

	return renamedConfirmed;
}