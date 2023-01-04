#include "SimulationWidget.hpp"

void ECellEngine::Editor::SimulationWidget::Draw()
{
    static bool* visibility = WidgetsVisibility::GetSingleton().GetSimulationWidgetVisibility();
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (*visibility)
    {
        if (!ImGui::Begin("Simulation", visibility))
        {
            ImGui::End();
            return;
        }

        if (ImGui::Button("Add Module"))
        {
            ECellEngine::Editor::Debug::EditorLog::GetSingleton().Log("Button Add Module Clicked!");
        }

        ImGui::End();
    }
}