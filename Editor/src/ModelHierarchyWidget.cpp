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
		short dragableID = 0;
		if (ImGui::TreeNode("Assets"))
		{
			//Display the list of loaded assets
			for (std::size_t idx = 0; idx < simulation->GetModules().size(); idx++)
			{
				//Id fro Drag & Drop
				ImGui::PushID(dragableID);
				
				//Leaf Node to display the name of the asset.
				ImGui::TreeNodeEx(simulation->GetModule(idx)->GetName(), leafNodeFlags);

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
				//Id fro Drag & Drop
				ImGui::PushID(dragableID);

				//Leaf Node to display the name of the asset.
				ImGui::TreeNodeEx(simulation->GetSolver(idx)->GetName(), leafNodeFlags);

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