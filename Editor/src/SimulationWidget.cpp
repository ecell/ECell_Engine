#include "SimulationWidget.hpp"

#include "Editor.hpp"//forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::SimulationWidget::Draw()
{
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Simulation"))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Add Module"))
    {
        addModuleCommandArray[1] = "C:/Users/eliot/Documents/Source/External/Papers/Hat-et-al_p53_BNG/S2_Code_BioNetGen/export/p53_L3V2_mod2.xml";
        editor.engine.GetCommandsManager()->interpretCommand(addModuleCommandArray);
    }

    ImGui::End();
}