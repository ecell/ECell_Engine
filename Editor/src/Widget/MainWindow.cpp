#include "Widget/MainWindow.hpp"

void ECellEngine::Editor::Widget::MainWindow::Awake()
{
    ImGui::Begin("MainWindow", NULL, windowFlags);
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::DockBuilderRemoveNode(ImGui::GetID("MainWindow"));
    ImGuiID rootNode = ImGui::DockBuilderAddNode(ImGui::GetID("MainWindow"));
    ImGui::DockBuilderSetNodeSize(rootNode, viewport->Size);

    ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(rootNode, ImGuiDir_Left, 0.2f, nullptr, &rootNode);
    ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(rootNode, ImGuiDir_Right, 0.8f, nullptr, &rootNode);

    ImGuiID dock_id_LU = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.5f, nullptr, &dock_id_left);
    ImGuiID dock_id_LD = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.5f, nullptr, &dock_id_left);

    ImGui::DockBuilderDockWindow("Model Explorer", dock_id_right);
    ImGui::DockBuilderDockWindow("Model Hierarchy", dock_id_LU);
    ImGui::DockBuilderDockWindow("Console", dock_id_LD);

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