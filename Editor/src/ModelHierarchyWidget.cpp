#include "ModelExplorerWidget.hpp"//forward declaration
#include "Editor.hpp"

void ECellEngine::Editor::ModelHierarchyWidget::Awake()
{
	SetSimulation(0);
}

void ECellEngine::Editor::ModelHierarchyWidget::Draw()
{
	if (ImGui::Begin("Model Hierarchy"))
	{
		static char valueBuffer[64] = "";
		short dragableID = 0;
		if (ImGui::TreeNode("Assets"))
		{
			//Display the list of loaded assets
			for (std::size_t idx = 0; idx < simulation->GetModules().size(); idx++)
			{
				//Id from Drag & Drop
				ImGui::PushID(dragableID);
				
				//If users is trying to rename this asset.
				if (renamingInProgress && renamingIdx == dragableID)
				{
					std::strcpy(valueBuffer, simulation->GetModule(idx)->GetName());
					if (ImGui::InputText("##name", valueBuffer, 64, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						simulation->GetModule(idx)->SetName(valueBuffer);
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
				}
				else//no renaming in progress for this item.
				{
					//Leaf Node to display the name of the asset.
					ImGui::TreeNodeEx(simulation->GetModule(idx)->GetName(), leafNodeFlags);
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						renamingInProgress = true;
						renamingIdx = dragableID;
					}
				}

				// The Asset name can be draged.
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					// Set payload to carry the index of our item (could be anything)
					ImGui::SetDragDropPayload("DND_ASSET", &idx, sizeof(std::size_t));

					// Display preview (could be anything, e.g. when dragging an image we could decide to display
					// the filename and a small preview of the image, etc.)
					ImGui::Text(simulation->GetModule(idx)->GetName());
					ImGui::EndDragDropSource();
				}

				//Pop Id of the Drag & Drop Item
				ImGui::PopID();

				//Increment ID for next Drag & Drop Item
				dragableID++;
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Solvers"))
		{
			//Display the list of loaded solvers
			for (std::size_t idx = 0; idx < simulation->GetSolvers().size(); idx++)
			{
				//Id from Drag & Drop
				ImGui::PushID(dragableID);

				//If users is trying to rename this solver.
				if (renamingInProgress && renamingIdx == dragableID)
				{
					std::strcpy(valueBuffer, simulation->GetSolver(idx)->GetName());
					if (ImGui::InputText("##name", valueBuffer, 64, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						simulation->GetSolver(idx)->SetName(valueBuffer);
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
				}
				else//no renaming in progress for this item.
				{
					//Leaf Node to display the name of the solver.
					ImGui::TreeNodeEx(simulation->GetSolver(idx)->GetName(), leafNodeFlags);
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						renamingInProgress = true;
						renamingIdx = dragableID;
					}
				}

				// The Asset name can be draged.
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					// Set payload to carry the index of our item (could be anything)
					ImGui::SetDragDropPayload("DND_SOLVER", &idx, sizeof(std::size_t));

					// Display preview (could be anything, e.g. when dragging an image we could decide to display
					// the filename and a small preview of the image, etc.)
					ImGui::Text(simulation->GetSolver(idx)->GetName());
					ImGui::EndDragDropSource();
				}

				//Pop Id of the Drag & Drop Item
				ImGui::PopID();

				//Increment ID for next Drag & Drop Item
				dragableID++;
			}

			ImGui::TreePop();
		}

		ImGui::End();
	}
}

void ECellEngine::Editor::ModelHierarchyWidget::SetSimulation(std::size_t _simulationIndex)
{
	simulation = editor.engine.GetSimulationsManager()->GetSimulation(_simulationIndex);
}