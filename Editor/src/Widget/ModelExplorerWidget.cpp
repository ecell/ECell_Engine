#include "Widget/ModelExplorerWidget.hpp"
#include "Editor.hpp"//We use editor here so we need to finish the forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::Widget::ModelExplorerWidget::Awake()
{
    ImGui::Begin("Model Explorer");

    ImGuiID rootNode = ImGui::DockBuilderAddNode(ImGui::GetID("Model Explorer"));
    ImGui::DockBuilderSetNodeSize(rootNode, ImGui::GetCurrentWindow()->Size);

    ImGuiID dock_id_DRL = ImGui::DockBuilderSplitNode(rootNode, ImGuiDir_Left, 0.2f, nullptr, &rootNode);
    ImGuiID dock_id_DRR = ImGui::DockBuilderSplitNode(rootNode, ImGuiDir_Right, 0.8f, nullptr, &rootNode);


    ImGui::DockBuilderDockWindow("Model Hierarchy", dock_id_DRL);
    ImGui::DockBuilderDockWindow("Model Viewer", dock_id_DRR);

    ImGui::DockBuilderFinish(ImGui::GetID("MainWindow"));


    ImGui::End();

    modelHierarchy.Awake();
    for (std::vector<MNBV::ModelNodeBasedViewerWidget>::iterator it = mnbViewers.begin(); it != mnbViewers.end(); it++)
    {
        it->Awake();
    }
}

void ECellEngine::Editor::Widget::ModelExplorerWidget::Draw()
{
    static bool wasdocked = false;
    ImGui::Begin("Model Explorer", NULL, windowFlags);
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
    ImGui::DockSpace(dockspaceID);

    DrawMenuBar();

    if ((utilityState >> 0) & 1)
    {
        DrawImportAssetPopup();
    }

    if ((utilityState >> 2) & 1)
    {
        DrawPreferencesPopup();
    }

    modelHierarchy.Draw();

    for (std::vector<MNBV::ModelNodeBasedViewerWidget>::iterator it = mnbViewers.begin(); it != mnbViewers.end(); it++)
    {
        it->Draw();
    }

    ImGui::End();
}

void ECellEngine::Editor::Widget::ModelExplorerWidget::DrawImportAssetPopup()
{
    if (ImGui::Begin("Import Asset From File", NULL, popupWindowFlags))
    {
        ImGui::InputText("File Path", assetPathBuffer, 256);
        ImGui::InputText("Name to Display", assetNameBuffer, 64);

        if (ImGui::Button("Import"))
        {
            addModuleCommandArray[2] = assetPathBuffer;
            if (editor.engine.GetCommandsManager()->interpretCommand(addModuleCommandArray))
            {
                ECellEngine::Core::SimulationsManager::GetSingleton().GetSimulation(0)->GetModules().back().get()->SetName(assetNameBuffer);
            }

            SwitchState(0); //Marks the Import Asset popup as closed
        }

        ImGui::SameLine();

        if (ImGui::Button("Close"))
        {
            SwitchState(0); //Marks the Import Asset popup as closed
        }

        ImGui::End();
    }
}

void ECellEngine::Editor::Widget::ModelExplorerWidget::DrawMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Add"))
        {
            if (ImGui::MenuItem("Import Asset From File"))
            {
                SwitchState(0); //Marks the Import Asset From File popup as opened
                ImGui::SetNextWindowPos(ImGui::GetIO().MousePos, ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(400, 120), ImGuiCond_Always);
                DrawImportAssetPopup();//to draw immediately during this frame
            }

            ImGui::EndMenu();
        }

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
                if (CountEditorContexts() > 0)
                {
                    static int currentEditorCtxtIdx = 0; // Here we store our selection data as an index.
                    const std::string combo_preview_value = std::to_string(currentEditorCtxtIdx + 1);  // Pass in the preview value visible before opening the combo (it could be anything)
                    std::string combo_visualization_value = std::to_string(CountEditorContexts() + 1);
                    if (ImGui::BeginCombo("Editor Context", combo_preview_value.c_str()))
                    {
                        for (int n = 0; n < CountEditorContexts(); n++)
                        {
                            combo_visualization_value = std::to_string(n + 1);
                            if (ImGui::Selectable(combo_visualization_value.c_str()))
                            {
                                currentEditorCtxtIdx = n;
                            }
                        }
                        ImGui::EndCombo();
                    }

                    if (ImGui::CollapsingHeader("Node Style"))
                    {
                        ax::NodeEditor::SetCurrentEditor(nodeEditorCtxts[currentEditorCtxtIdx]);
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

                        ax::NodeEditor::SetCurrentEditor(nodeEditorCtxts[currentEditorCtxtIdx]);
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
                        for (int i = 0; i < ECellEngine::Editor::Utility::MNBV::NodeType_Count; i++)
                        {
                            ECellEngine::Editor::Utility::MNBV::NodeType nodeType = (ECellEngine::Editor::Utility::MNBV::NodeType)i;
                            if (ImGui::TreeNode(MNBV::GetNodeTypeName(nodeType)))
                            {
                                ImVec4* colors = MNBV::GetNodeColors(nodeType);
                                for (int j = 0; j < ECellEngine::Editor::Utility::MNBV::NodeColorType_Count; j++)
                                {
                                    const char* name = MNBV::GetNodeColorTypeName((ECellEngine::Editor::Utility::MNBV::NodeColorType)j);
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
                        for (int i = 0; i < ECellEngine::Editor::Utility::MNBV::PinType_Count; i++)
                        {
                            ECellEngine::Editor::Utility::MNBV::PinType pinType = (ECellEngine::Editor::Utility::MNBV::PinType)i;
                            if (ImGui::TreeNode(MNBV::GetPinTypeName(pinType)))
                            {
                                ImVec4* colors = MNBV::GetPinColors(pinType);
                                for (int j = 0; j < ECellEngine::Editor::Utility::MNBV::PinColorType_Count; j++)
                                {
                                    const char* name = MNBV::GetPinColorTypeName((ECellEngine::Editor::Utility::MNBV::PinColorType)j);
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