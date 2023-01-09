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
            addModuleCommandArray[1] = "C:/Users/eliot/Documents/Source/External/Papers/Hat-et-al_p53_BNG/S2_Code_BioNetGen/export/p53_L3V2_mod.xml";
            engineCmdsManager->interpretCommand(addModuleCommandArray);
        }

        ImGui::End();
    }
}