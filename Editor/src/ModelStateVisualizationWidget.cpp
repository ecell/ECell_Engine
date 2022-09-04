#include "ModelStateVisualizationWidget.hpp"

void ECellEngine::Editor::ModelStateVisualizationWidget::draw()
{
    static std::vector<int>* quantities = gillespieSimulationEnv->isInitialized() ? gillespieSimulationEnv->getQuantities() : nullptr;

    static int nbSpecies = quantities == nullptr ? 0 : quantities->size();

    ImGui::SetNextWindowSize(ImVec2(530, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Model State Visualization");

	static ImGuiTableFlags flags =
          ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
        | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY;

    ImGui::Text("The model state has %1d species", nbSpecies);
    if (ImGui::BeginTable("Loaded SBML Documents", 2, flags))
    {
        ImGui::TableSetupColumn("Species Id");
        ImGui::TableSetupColumn("Quantity");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        for (int i = 0; i < nbSpecies; i++)
        {
            
            ImGui::TableNextColumn();
            ImGui::Text("%2d", i);
            ImGui::TableNextColumn();
            ImGui::Text("%2d", quantities[i]);
            ImGui::TableNextRow();
        }

        ImGui::EndTable();
    }

	ImGui::End();
}