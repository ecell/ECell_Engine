#include "Editor.hpp"//forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::ModelExplorerWidget::Awake()
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
    for (std::vector<ModelNodeBasedViewerWidget>::iterator it = mnbViewers.begin(); it != mnbViewers.end(); it++)
    {
        it->Awake();
    }
}

void ECellEngine::Editor::ModelExplorerWidget::Draw()
{
    static bool wasdocked = false;
    ImGui::Begin("Model Explorer", NULL, windowFlags);
    if (ImGui::IsWindowDocked())
    {
        if (!wasdocked)
        {
            ImGui::DockBuilderSetNodeSize(ImGui::GetWindowDockID(), ImVec2(ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y * 0.95));
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

    if ((utilityState >> 1) & 1)
    {
        DrawAddSolverPopup();
    }

    modelHierarchy.Draw();

    for (std::vector<ModelNodeBasedViewerWidget>::iterator it = mnbViewers.begin(); it != mnbViewers.end(); it++)
    {
        it->Draw();
    }

    ImGui::End();
}

void ECellEngine::Editor::ModelExplorerWidget::DrawAddSolverPopup()
{
    if (ImGui::Begin("Add Solver", NULL, popupWindowFlags))
    {
        if (ImGui::BeginMenu("Biochemical"))
        {
            if (ImGui::MenuItem("Gillespie Next Reaction Method"))
            {
                addSolverCommandArray[2] = "GillespieNRMRSolver";
                editor.engine.GetCommandsManager()->interpretCommand(addSolverCommandArray);

                modelHierarchy.AddSolverName("Gillespie Next Reaction Method");

                utilityState &= 0 << 1;
            }
                
            if (ImGui::MenuItem("Ordinary Differential Equations (TODO)"))
            {
                ECellEngine::Logging::Logger::GetSingleton().LogWarning("ODE Solver is not yet available");
                utilityState &= 0 << 1;
            }

            ImGui::EndMenu();// Biochemical
        }

        if (ImGui::Button("Close"))
        {
            utilityState &= 0 << 1;
        }

        ImGui::End();
    }
}

void ECellEngine::Editor::ModelExplorerWidget::DrawImportAssetPopup()
{
    if (ImGui::Begin("Import Asset From File", NULL, popupWindowFlags))
    {
        ImGui::InputText("File Path", assetPathBuffer, 256);
        ImGui::InputText("Name to Display", assetNameBuffer, 256);

        if (ImGui::Button("Import"))
        {
            addModuleCommandArray[2] = assetPathBuffer;
            editor.engine.GetCommandsManager()->interpretCommand(addModuleCommandArray);

            /*TODO: Check that the module was correctly imported before drawing.*/
            modelHierarchy.AddAssetName(assetNameBuffer);

            utilityState &= 0 << 0;
        }

        ImGui::SameLine();

        if (ImGui::Button("Close"))
        {
            utilityState &= 0 << 0;
        }

        ImGui::End();
    }
}

void ECellEngine::Editor::ModelExplorerWidget::DrawMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Add"))
        {
            if (ImGui::MenuItem("Import Asset From File"))
            {
                utilityState |= 1 << 0;
                ImGui::SetNextWindowPos(ImGui::GetIO().MousePos);
                ImGui::SetNextWindowSize(ImVec2(400, 120));
            }

            if (ImGui::MenuItem("Asset Solver"))
            {
                utilityState |= 1 << 1;
                ImGui::SetNextWindowPos(ImGui::GetIO().MousePos);
                ImGui::SetNextWindowSize(ImVec2(150, 90));
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}