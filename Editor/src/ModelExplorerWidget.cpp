#include "Editor.hpp"//forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::ModelExplorerWidget::AddAssetNode(const char* _name)
{
    assetNodes.push_back(ECellEngine::Editor::Utility::AssetNodeInfo(uniqueIdx, _name));

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

    
    DrawMenuBar();

    if ((utilityState >> 0) & 1)
    {
        DrawImportAssetPopup();
    }

    if ((utilityState >> 1) & 1)
    {
        DrawAddSolverPopup();
    }

    DrawModelExplorer();

    ImGui::End();
}

void ECellEngine::Editor::ModelExplorerWidget::DrawAddSolverPopup()
{
    if (ImGui::Begin("Add Solver", NULL, popupWindowFlags))
    {
        if (ImGui::BeginMenu("Solvers"))
        {
            if (ImGui::BeginMenu("Biochemical"))
            {
                if (ImGui::MenuItem("Gillespie Next Reaction Method"))
                {
                    addSolverCommandArray[2] = "GillespieNRMRSolver";
                    editor.engine.GetCommandsManager()->interpretCommand(addSolverCommandArray);
                    utilityState &= 0 << 1;
                }
                
                if (ImGui::MenuItem("Ordinary Differential Equations (TODO)"))
                {
                    ECellEngine::Logging::Logger::GetSingleton().LogWarning("ODE Solver is not yet available");
                    utilityState &= 0 << 1;
                }
                ImGui::EndMenu();// Solvers/Biochemical
            }

            ImGui::EndMenu();// Solvers
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
    ImGui::SetNextWindowSize(ImVec2(400, 120), ImGuiCond_Once);
    if (ImGui::Begin("Import Asset From File", NULL, popupWindowFlags))
    {
        ImGui::InputText("File Path", assetPathBuffer, 256);
        ImGui::InputText("Name to Display", assetNameBuffer, 256);

        if (ImGui::Button("Import"))
        {
            addModuleCommandArray[2] = assetPathBuffer;
            editor.engine.GetCommandsManager()->interpretCommand(addModuleCommandArray);

            /*TODO: Check that the module was correctly imported before drawing.*/
            AddAssetNode(assetNameBuffer);

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
            }

            if (ImGui::MenuItem("Asset Solver"))
            {
                utilityState |= 1 << 1;
                ImGui::SetNextWindowPos(ImGui::GetIO().MousePos);
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void ECellEngine::Editor::ModelExplorerWidget::DrawModelExplorer()
{    
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImGui::Separator();

    ax::NodeEditor::SetCurrentEditor(nodeEditorCtxt);

    // Start interaction with editor.
    ax::NodeEditor::Begin("Model Exploration Space", ImVec2(0.0, 0.0f));

    // Submit Links
    for (ECellEngine::Editor::Utility::LinkInfo& linkInfo : links)
    {
        ax::NodeEditor::Link(linkInfo.id, linkInfo.inputId, linkInfo.outputId);
    }

    // End of interaction with editor.
    ax::NodeEditor::End();
    ax::NodeEditor::SetCurrentEditor(nullptr);
}