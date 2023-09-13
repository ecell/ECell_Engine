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
		ImGui::PushID(it->get());
		//If users is trying to rename this asset.
		if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingInProgress) && contextNodeIdx == nodeID)
		{
			std::strcpy(renamingBuffer, it->get()->GetName());
			if (TreeNodeRename(renamingBuffer))
			{
				it->get()->SetName(renamingBuffer);
			}

			ImGui::PushID(it->get()->GetName());
			ImGui::Indent(ImGui::GetStyle().IndentSpacing);
			DrawSimulationHierarchy(mnbvCtxtCount, it->get());
			ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
			ImGui::PopID();

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
					contextNodeIdx = simuCount;
					Util::SetFlag(hierarchyCtxt, HierarchyContext_RightClickOnSimulationNode);
					Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnMNBVCtxtNode);
					Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnBackground);
				}
			}

			if (nodeOpen)
			{
				DrawSimulationHierarchy(mnbvCtxtCount, it->get());
				ImGui::TreePop();
			}

		}
		simuCount++;
		ImGui::PopID();
	}
}

template<typename LeafType, typename NameAccessorType>
void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawHierarchyLeafsList(const char* _leafsListName, MNBV::ModelNodeBasedViewerContext& _mnbvCtxt, const std::vector<LeafType>& _leafs, NameAccessorType& _nameAccessor)
{
	nodeID++;
	if (_leafs.size() > 0)
	{
		if (ImGui::TreeNodeEx(_leafsListName, ImGuiTreeNodeFlags_OpenOnArrow))
		{
			for (auto _leaf : _leafs)
			{
				nodeID++;
				ImGui::TreeNodeEx(_nameAccessor(_leaf), leafNodeFlags);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}

template<typename LeafKeyType, typename LeafType, typename NameAccessorType>
void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawHierarchyLeafsUMap(const char* _leafsListName, MNBV::ModelNodeBasedViewerContext& _mnbvCtxt, const std::unordered_map<LeafKeyType, LeafType>& _leafs, NameAccessorType& _nameAccessor)
{
	nodeID++;
	if (_leafs.size() > 0)
	{
		if (ImGui::TreeNodeEx(_leafsListName, ImGuiTreeNodeFlags_OpenOnArrow))
		{
			for (auto [_leafKey, _leaf] : _leafs)
			{
				nodeID++;
				ImGui::TreeNodeEx(_nameAccessor(_leaf), leafNodeFlags);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
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

void ECellEngine::Editor::Widget::ModelHierarchyWidget::DrawSimulationHierarchy(int& _out_mnbvCtxtStartIdx, ECellEngine::Core::Simulation* _simulation)
{
	static NameAccessorBySPtr nameAccessorBySPtr;
	MNBV::ModelNodeBasedViewerContext& mnbvCtxt = mnbvCtxts->at(_out_mnbvCtxtStartIdx);
	nodeID++;
	if (ImGui::TreeNodeEx("Data State", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		DrawHierarchyLeafsUMap("Equations", mnbvCtxt, mnbvCtxt.simulation->GetDataState().GetEquations(), nameAccessorBySPtr);

		DrawHierarchyLeafsUMap("Parameters", mnbvCtxt, mnbvCtxt.simulation->GetDataState().GetParameters(), nameAccessorBySPtr);

		DrawHierarchyLeafsUMap("Reactions", mnbvCtxt, mnbvCtxt.simulation->GetDataState().GetReactions(), nameAccessorBySPtr);

		DrawHierarchyLeafsUMap("Species", mnbvCtxt, mnbvCtxt.simulation->GetDataState().GetAllSpecies(), nameAccessorBySPtr);

		ImGui::TreePop();
	}

	nodeID++;
	if (ImGui::TreeNodeEx("Contexts", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		while (_out_mnbvCtxtStartIdx < mnbvCtxts->size() && mnbvCtxt.simulation->id == _simulation->id)
		{
			nodeID++;
			ImGui::PushID(_out_mnbvCtxtStartIdx);
			//If users is trying to rename this asset.
			if (Util::IsFlagSet(hierarchyCtxt, HierarchyContext_RenamingInProgress) && contextNodeIdx == nodeID)
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
				bool nodeOpen = ImGui::TreeNodeEx(mnbvCtxt.GetName(), ImGuiTreeNodeFlags_OpenOnArrow);

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
						contextNodeIdx = _out_mnbvCtxtStartIdx;
						Util::SetFlag(hierarchyCtxt, HierarchyContext_RightClickOnMNBVCtxtNode);
						Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnSimulationNode);
						Util::ClearFlag(hierarchyCtxt, HierarchyContext_RightClickOnBackground);
					}
				}

				if (nodeOpen)
				{
					DrawMNBVCtxtHierarchy(mnbvCtxt);
					ImGui::TreePop();
				}

			}
			_out_mnbvCtxtStartIdx++;
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
	
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