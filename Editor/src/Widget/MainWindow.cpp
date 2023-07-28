#include "Widget/MainWindow.hpp"

void ECellEngine::Editor::Widget::MainWindow::Awake()
{
    ImGui::Begin("MainWindow", NULL, windowFlags);
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::DockBuilderRemoveNode(ImGui::GetID("MainWindow"));
    ImGuiID rootNode = ImGui::DockBuilderAddNode(ImGui::GetID("MainWindow"));
    ImGui::DockBuilderSetNodeSize(rootNode, viewport->Size);

    ImGuiID dock_id_top = ImGui::DockBuilderSplitNode(rootNode, ImGuiDir_Up, 0.05f, nullptr, &rootNode);
    ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(rootNode, ImGuiDir_Down, 0.95f, nullptr, &rootNode);
    ImGuiDockNode* topNode = ImGui::DockBuilderGetNode(dock_id_top);
    topNode->LocalFlags |= ImGuiDockNodeFlags_NoResize;

    ImGuiID dock_id_DR = ImGui::DockBuilderSplitNode(dock_id_down, ImGuiDir_Right, 0.8f, nullptr, &dock_id_down);
    ImGuiID dock_id_DL = ImGui::DockBuilderSplitNode(dock_id_down, ImGuiDir_Left, 0.2f, nullptr, &dock_id_down);
    ImGuiID dock_id_DLU = ImGui::DockBuilderSplitNode(dock_id_DL, ImGuiDir_Up, 0.5f, nullptr, &dock_id_DL);
    ImGuiID dock_id_DLD = ImGui::DockBuilderSplitNode(dock_id_DL, ImGuiDir_Down, 0.5f, nullptr, &dock_id_DL);

    ImGui::DockBuilderDockWindow("Simulation Flow Control", dock_id_top);
    ImGui::DockBuilderDockWindow("Model Explorer", dock_id_DR);
    ImGui::DockBuilderDockWindow("Options", dock_id_DLU);
    ImGui::DockBuilderDockWindow("Console", dock_id_DLD);

    ImGui::DockBuilderFinish(ImGui::GetID("MainWindow"));

    ImGui::End();
}

void ECellEngine::Editor::Widget::MainWindow::Draw()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::Begin("MainWindow", NULL, windowFlags);
    ImGui::PopStyleVar(2); //poping no rounding and no borders
    
    // Submit the DockSpace
    ImGuiID dockspaceID = ImGui::GetID("MainWindow");
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);

    //if (ImGui::BeginMenuBar())
    //{
    //    if (ImGui::BeginMenu("Windows"))
    //    {
    //        // Disabling fullscreen would allow the window to be moved to the front of other windows,
    //        // which we can't undo at the moment without finer window depth/z control.
    //        ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
    //        ImGui::MenuItem("Padding", NULL, &opt_padding);
    //        ImGui::Separator();

    //        if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
    //        if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
    //        if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
    //        if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
    //        if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
    //        ImGui::Separator();

    //        if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
    //            *p_open = false;
    //        ImGui::EndMenu();
    //    }

    //    ImGui::EndMenuBar();
    //}
    
    
	ImGui::End();

}