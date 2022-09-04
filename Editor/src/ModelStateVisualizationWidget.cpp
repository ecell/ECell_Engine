#include "ModelStateVisualizationWidget.hpp"

void ECellEngine::Editor::ModelStateVisualizationWidget::draw()
{
    static int nbSpecies = 0;
    if (simulationLoop->getSimulationEnvironment()->isInitialized())
    {
        nbSpecies = simulationLoop->getSimulationEnvironment()->getQuantities()->size();
    }
    static ASTEvaluator* astEvaluator = simulationLoop->getSimulationEnvironment()->astEvaluator;

    ImGui::SetNextWindowSize(ImVec2(530, 300), ImGuiCond_FirstUseEver);

	ImGui::Begin("Model State Visualization");

	static ImGuiTableFlags flags =
          ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
        | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY;

    if (!simulationLoop->getSimulationEnvironment()->isInitialized())
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "No simulation environment has been set yet.");
    }
    else
    {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "A simulation environment has been set.");
    }

    ImGui::Text("The model state has %1d species", nbSpecies);
    if (ImGui::BeginTable("Loaded SBML Documents", 2, flags))
    {
        ImGui::TableSetupColumn("Species Id");
        ImGui::TableSetupColumn("Quantity");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        for (int i = 0; i < 2*nbSpecies; i+=2)
        {
            ImGui::TableNextColumn();
            ImGui::Text("%2d", i);
            ImGui::TableNextColumn();
            ImGui::Text("%2f", astEvaluator->getNamedNodeValue(i));
            ImGui::TableNextRow();
        }

        ImGui::EndTable();
    }

	ImGui::End();
}