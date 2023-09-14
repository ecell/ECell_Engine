#include "imgui_internal.h"
#include "Style/EditorStyle.hpp"
#include "Util/BinarySearch.hpp"
#include "Widget/ModelExplorerWidget.hpp"
#include "Editor.hpp"//We use editor here so we need to finish the forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::Widget::ModelExplorerWidget::AddModelNodeBasedViewerWidget()
{
	mnbViewers.emplace_back(MNBV::ModelNodeBasedViewerWidget(editor));
	ctxtsPerViewer.emplace_back(0);//By default we display the context at index 0.
}

void ECellEngine::Editor::Widget::ModelExplorerWidget::AddModelNodeBasedViewerContext(Core::Simulation* _simulation)
{
	CompareMNBVCtxtBySimulation cmp;
	std::vector<MNBV::ModelNodeBasedViewerContext>::iterator it = Util::BinarySearch::LowerBound(mnbvCtxts.begin(), mnbvCtxts.end(), _simulation->id, cmp);
	
	it = mnbvCtxts.insert(it, MNBV::ModelNodeBasedViewerContext());

	it->SetSimulation(_simulation);
	it->SetEngineCommandsManager(&editor->engine.GetCommandsManager());

	for (std::size_t i = 0; i < mnbViewers.size(); i++)
	{
		if (ctxtsPerViewer[i] >= it - mnbvCtxts.begin())
		{
			ctxtsPerViewer[i]++;
		}
	}

	//It's low cost to set the pointer every time we add a context so we do it
	// by default even if there hasn't been any resize of the vector during the insert.
	modelHierarchy.SetMNBVCtxts(&mnbvCtxts);
}

void ECellEngine::Editor::Widget::ModelExplorerWidget::Awake()
{
	ImGui::Begin("Model Explorer");

	ImGuiID rootNode = ImGui::DockBuilderAddNode(ImGui::GetID("Model Explorer"));
	ImGui::DockBuilderSetNodeSize(rootNode, ImGui::GetCurrentWindow()->Size);

	ImGuiID dock_id_up = ImGui::DockBuilderSplitNode(rootNode, ImGuiDir_Up, 0.05f, nullptr, &rootNode);
	ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(rootNode, ImGuiDir_Down, 0.95f, nullptr, &rootNode);

	ImGui::DockBuilderDockWindow("Simulation Flow Control", dock_id_up);
	ImGui::DockBuilderDockWindow("Model Viewer", dock_id_down);

	ImGui::DockBuilderFinish(ImGui::GetID("MainWindow"));

	modelHierarchy.Awake();
	simuFlowControl.Awake();

	for (unsigned char i = 0; i < mnbViewers.size(); i++)
	{
		MNBV::SetCurrentMNBVContext(&mnbvCtxts[ctxtsPerViewer[i]]);
		mnbViewers[i].Awake();
		MNBV::SetCurrentMNBVContext(nullptr);
	}
	ImGui::End();
}

void ECellEngine::Editor::Widget::ModelExplorerWidget::Draw()
{
	static bool wasdocked = false;
	if(ImGui::Begin("Model Explorer", NULL, windowFlags))
	{
		if (ImGui::IsWindowDocked())
		{
			if (!wasdocked)
			{
				ImGui::DockBuilderSetNodeSize(ImGui::GetWindowDockID(), ImVec2(ImGui::GetMainViewport()->Size.x, (float)ImGui::GetMainViewport()->Size.y * 0.95f));
				wasdocked = true;
			}
		}
		else
		{
			wasdocked = false;
		}

		ImGuiID dockspaceID = ImGui::GetID("Model Explorer");
		ImGui::DockSpace(dockspaceID, ImVec2(0.f,0.f), ImGuiDockNodeFlags_NoResizeY);//blocking resize to always have the SimuFlowControlWidget Visible at the top

		DrawMenuBar();

		if ((utilityState >> 2) & 1)
		{
			DrawPreferencesPopup();
		}

		modelHierarchy.Draw();
		simuFlowControl.Draw();

		for (unsigned char i = 0; i < mnbViewers.size(); i++)
		{
			MNBV::SetCurrentMNBVContext(&mnbvCtxts[ctxtsPerViewer[i]]);
			mnbViewers[i].Draw();
			MNBV::SetCurrentMNBVContext(nullptr);
		}
	}

	ImGui::End();
}

void ECellEngine::Editor::Widget::ModelExplorerWidget::DrawMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Preferences"))
			{
				SwitchState(2); //Marks the Preferences popup as opened
				ImGui::SetNextWindowPos(ImGui::GetIO().MousePos, ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Always);
				DrawPreferencesPopup();//to draw immediately during this frame
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void ECellEngine::Editor::Widget::ModelExplorerWidget::DrawPreferencesPopup()
{
	if (ImGui::Begin("Preferences", NULL, popupWindowFlags | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar))
	{
		if (ImGui::BeginTabBar("##ModelExplorerPreferencesTabs"))
		{
			if (ImGui::BeginTabItem("Node Editor Original"))
			{
				if (mnbViewers.size() > 0)
				{
					static int currentMNBVIdx = 0; // Here we store our selection data as an index.
					const std::string combo_preview_value = std::to_string(currentMNBVIdx + 1);  // Pass in the preview value visible before opening the combo (it could be anything)
					std::string combo_visualization_value = std::to_string(mnbViewers.size() + 1);
					if (ImGui::BeginCombo("Editor Context", combo_preview_value.c_str()))
					{
						for (std::size_t n = 0; n < mnbViewers.size(); n++)
						{
							combo_visualization_value = std::to_string(n + 1);
							if (ImGui::Selectable(combo_visualization_value.c_str()))
							{
								currentMNBVIdx = n;
							}
						}
						ImGui::EndCombo();
					}

					if (ImGui::CollapsingHeader("Node Style"))
					{
						ax::NodeEditor::SetCurrentEditor(mnbViewers[currentMNBVIdx].editorCtxt);
						ax::NodeEditor::Style& nodeStyle = ax::NodeEditor::GetStyle();

						if (ImGui::TreeNode("Flow"))
						{
							ImGui::SliderFloat("Flow Duration", (float*)&nodeStyle.FlowDuration, 0.f, 20.f, "%.0f");
							ImGui::SliderFloat("Flow Marker Distance", (float*)&nodeStyle.FlowMarkerDistance, 0.f, 100.f, "%.0f");
							ImGui::SliderFloat("Flow Speed", (float*)&nodeStyle.FlowSpeed, 0.f, 300.f, "%.0f");
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Group"))
						{
							ImGui::SliderFloat("Group Rounding", (float*)&nodeStyle.GroupRounding, 0.f, 20.f, "%.0f");
							ImGui::SliderFloat("Group Border Width", (float*)&nodeStyle.GroupBorderWidth, 0.f, 20.f, "%.0f");
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Links"))
						{
							ImGui::SliderFloat("Link Strength", (float*)&nodeStyle.LinkStrength, 0.f, 200.f, "%.0f");
							ImGui::Checkbox("Highlight Connected Links", (bool*)&nodeStyle.HighlightConnectedLinks);
							ImGui::Checkbox("Snap Link To PinDir", (bool*)&nodeStyle.SnapLinkToPinDir);
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Node"))
						{
							ImGui::SliderFloat("Node Rounding", (float*)&nodeStyle.NodeRounding, 0.f, 20.f, "%.0f");
							ImGui::SliderFloat4("Node Padding", (float*)&nodeStyle.NodePadding, 0.f, 20.f, "%.0f");
							ImGui::SliderFloat("Node Border Width", (float*)&nodeStyle.NodeBorderWidth, 0.f, 20.f, "%.0f");
							ImGui::SliderFloat("Node Hovered Border Width", (float*)&nodeStyle.HoveredNodeBorderWidth, 0.f, 20.f, "%.0f");
							ImGui::SliderFloat("Node Selected Border Width", (float*)&nodeStyle.SelectedNodeBorderWidth, 0.f, 20.f, "%.0f");
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Pin"))
						{
							ImGui::SliderFloat2("Pin Source Direction", (float*)&nodeStyle.SourceDirection, -1.f, 1.f, "%.0f");
							ImGui::SliderFloat2("Pin Target Direction", (float*)&nodeStyle.TargetDirection, -1.f, 1.f, "%.0f");
							ImGui::SliderFloat("Pin Arrow Size", (float*)&nodeStyle.PinArrowSize, 0.f, 20.f, "%.0f");
							ImGui::SliderFloat("Pin Rounding", (float*)&nodeStyle.PinRounding, 0.f, 20.f, "%.0f");
							ImGui::SliderFloat("Pin Arrow Width", (float*)&nodeStyle.PinArrowWidth, 0.f, 20.f, "%.0f");
							ImGui::SliderFloat("Pin Border Width", (float*)&nodeStyle.PinBorderWidth, 0.f, 20.f, "%.0f");
							ImGui::SliderFloat("Pin Corners", (float*)&nodeStyle.PinCorners, 0.f, 500.f, "%.0f");
							ImGui::SliderFloat("Pin Radius", (float*)&nodeStyle.PinRadius, 0.f, 20.f, "%.0f");
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Pivot"))
						{
							ImGui::SliderFloat2("Pivot Alignement", (float*)&nodeStyle.PivotAlignment, 0.f, 100.f, "%.0f");
							ImGui::SliderFloat2("Pivot Scale", (float*)&nodeStyle.PivotScale, 0.f, 100.f, "%.0f");
							ImGui::SliderFloat2("Pivot Size", (float*)&nodeStyle.PivotSize, 0.f, 100.f, "%.0f");
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Zoom"))
						{
							ImGui::SliderFloat("Scroll Duration", (float*)&nodeStyle.ScrollDuration, 0.f, 20.f, "%.0f");
							ImGui::TreePop();
						}

					}

					if (ImGui::CollapsingHeader("Node Colors"))
					{
						static ImGuiColorEditFlags alpha_flags = 0;
						if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None))
						{
							alpha_flags = ImGuiColorEditFlags_None;
						}
						ImGui::SameLine();
						if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview))
						{
							alpha_flags = ImGuiColorEditFlags_AlphaPreview;
						}
						ImGui::SameLine();
						if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf))
						{
							alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf;
						}

						ax::NodeEditor::SetCurrentEditor(mnbViewers[currentMNBVIdx].editorCtxt);
						ax::NodeEditor::Style& nodeStyle = ax::NodeEditor::GetStyle();
						for (int i = 0; i < ax::NodeEditor::StyleColor_Count; i++)
						{
							const char* name = ax::NodeEditor::GetStyleColorName((ax::NodeEditor::StyleColor)i);
							ImGui::PushID(i);
							ImGui::ColorEdit4("##color", (float*)&nodeStyle.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::TextUnformatted(name);
							ImGui::PopID();
						}
					}
				}
				else
				{
					ImGui::TextColored(ImVec4(255, 0, 0, 255), "No model viewer currently opened.");
				}
				
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Node Editor Custom"))
			{
				static int currentMNBVCtxtIdx = 0; // Here we store our selection data as an index.
				const std::string combo_preview_value = std::to_string(currentMNBVCtxtIdx + 1);  // Pass in the preview value visible before opening the combo (it could be anything)
				std::string combo_visualization_value = std::to_string(CountModelNodeBasedViewerContext() + 1);
				if (ImGui::BeginCombo("Editor Style", combo_preview_value.c_str()))
				{
					for (int n = 0; n < CountModelNodeBasedViewerContext(); n++)
					{
						combo_visualization_value = std::to_string(n + 1);
						if (ImGui::Selectable(combo_visualization_value.c_str()))
						{
							currentMNBVCtxtIdx = n;
						}
					}
					ImGui::EndCombo();
				}

				MNBV::SetCurrentMNBVContext(&mnbvCtxts[currentMNBVCtxtIdx]);

				if (ImGui::CollapsingHeader("Colors"))
				{
					static ImGuiColorEditFlags alpha_flags = 0;
					if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None))
					{
						alpha_flags = ImGuiColorEditFlags_None;
					}
					ImGui::SameLine();
					if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview))
					{
						alpha_flags = ImGuiColorEditFlags_AlphaPreview;
					}
					ImGui::SameLine();
					if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf))
					{
						alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf;
					}

					if (ImGui::TreeNode("Nodes"))
					{
						for (int i = 0; i < Utility::MNBV::NodeType_Count; i++)
						{
							Utility::MNBV::NodeType nodeType = (Utility::MNBV::NodeType)i;
							if (ImGui::TreeNode(Style::EditorStyle::GetNodeTypeName(nodeType)))
							{
								ImVec4* colors = Style::EditorStyle::GetNodeColors(nodeType);
								for (int j = 0; j < Style::NodeColorType_Count; j++)
								{
									const char* name = Style::EditorStyle::GetNodeColorTypeName((Style::NodeColorType)j);
									ImGui::PushID(j);
									ImGui::ColorEdit4("##color", &(colors+j)->x, ImGuiColorEditFlags_AlphaBar | alpha_flags);
									ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
									ImGui::TextUnformatted(name);
									ImGui::PopID();
								}

								ImGui::TreePop();
							}
						}

						ImGui::TreePop(); //Nodes
					}

					if (ImGui::TreeNode("Pins"))
					{
						for (int i = 0; i < Utility::MNBV::PinType_Count; i++)
						{
							Utility::MNBV::PinType pinType = (Utility::MNBV::PinType)i;
							if (ImGui::TreeNode(Style::EditorStyle::GetPinTypeName(pinType)))
							{
								ImVec4* colors = Style::EditorStyle::GetPinColors(pinType);
								for (int j = 0; j < Style::PinColorType_Count; j++)
								{
									const char* name = Style::EditorStyle::GetPinColorTypeName((Style::PinColorType)j);
									ImGui::PushID(j);
									ImGui::ColorEdit4("##color", &(colors+j)->x, ImGuiColorEditFlags_AlphaBar | alpha_flags);
									ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
									ImGui::TextUnformatted(name);
									ImGui::PopID();
								}
								ImGui::TreePop();
							}
						}
						ImGui::TreePop(); //Pins
					}
				}
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		if (ImGui::Button("Close"))
		{
			SwitchState(2); //Marks the preferences popup as closed
		}

		ImGui::End();
	}
}

bool ECellEngine::Editor::Widget::ModelExplorerWidget::FocusNode(unsigned short _ctxtIdx, unsigned long long _nodeID)
{
	MNBV::SetCurrentMNBVContext(&mnbvCtxts[_ctxtIdx]);
	Utility::MNBV::NodeData* _node = MNBV::FindNodeInAll(_nodeID);
	if (_node == nullptr)
	{
		ECellEngine::Logging::Logger::LogError("Could not find node with ID \"%llu\" in context \"%u\"", _nodeID, _ctxtIdx);
		return false;
	}

	for (unsigned short i = 0; i < (unsigned short)ctxtsPerViewer.size(); i++)
	{
		if (ctxtsPerViewer[i] == _ctxtIdx)
		{
			mnbViewers[i].FocusNode(_nodeID);
		}
	}
	MNBV::SetCurrentMNBVContext(nullptr);

	return true;
}

std::pair<bool, std::vector<unsigned short>::iterator> ECellEngine::Editor::Widget::ModelExplorerWidget::IsMNBVContextInUse(std::size_t _idx)
{
	for (std::vector<unsigned short>::iterator it = ctxtsPerViewer.begin(); it != ctxtsPerViewer.end(); it++)
	{
		if (*it == _idx)
		{
			return std::pair(true, it);
		}
	}
	return std::pair(false, ctxtsPerViewer.end());
}

bool ECellEngine::Editor::Widget::ModelExplorerWidget::RemoveModelNodeBasedViewerWidget(std::size_t _idx)
{
	if (_idx >= mnbViewers.size())
	{
		ECellEngine::Logging::Logger::LogError("Tried to destroy a ModelNodeBasedViewerWidget at an index beyond the size of current list.");
		return false;
	}
	ax::NodeEditor::DestroyEditor(mnbViewers[_idx].editorCtxt);
	mnbViewers.erase(mnbViewers.begin() + _idx);

	return true;
}

bool ECellEngine::Editor::Widget::ModelExplorerWidget::RemoveModelNodeBasedViewerContext(std::size_t _idx)
{
	if (_idx >= mnbvCtxts.size())
	{
		ECellEngine::Logging::Logger::LogError("Tried to destroy a ModelNodeBasedViewerContext at an index beyond the size of current list.");
		return false;
	}

	if (mnbvCtxts.size() == 1)
	{
		ECellEngine::Logging::Logger::LogError("Tried to destroy the last ModelNodeBasedViewerContext; this is not allowed, there must be at least one MNBV context available in the model explorer.");
		return false;
	}

	mnbvCtxts.erase(mnbvCtxts.begin() + _idx);

	//compensate the indeces in ctxtsPerViewer for the removed context
	for (std::vector<unsigned short>::iterator it = ctxtsPerViewer.begin(); it != ctxtsPerViewer.end(); it++)
	{
		if (*it > _idx)
		{
			(*it)--;
		}
	}

	return true;
}