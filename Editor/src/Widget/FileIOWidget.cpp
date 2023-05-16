#include "Widget/FileIOWidget.hpp"

void ECellEngine::Editor::FileIOWidget::draw()
{
    ImGui::SetNextWindowSize(ImVec2(530, 60), ImGuiCond_FirstUseEver);
    ImGui::Begin("File Input");

    drawSearchTargetFile();
    
    ImGui::Separator();

    drawLoadedSBMLDocuments();

    ImGui::End();
}

void ECellEngine::Editor::FileIOWidget::drawLoadedSBMLDocuments()
{
    static ImGuiTableFlags flags =
          ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
        | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ScrollY;

    ImGui::Text("Currently %1d models were loaded", nbLoadedSBMLDocuments);
    if (ImGui::BeginTable("Loaded SBML Documents", 2, flags))
    {
        ImGui::TableSetupColumn("Model Id");
        ImGui::TableSetupColumn("Simulate");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        for (int i = 0; i < nbLoadedSBMLDocuments; i++)
        {
            
            ImGui::TableNextColumn();
            ImGui::Text(loadedSBMLDocumentNames[i].c_str());
            ImGui::TableNextColumn();
            if (ImGui::Button("Set As Simulation Target"))
            {
                addSimulationTargetCommandArray[1] = std::to_string(i);
                engineCmdsManager->interpretCommand(addSimulationTargetCommandArray);
            }
            ImGui::TableNextRow();
        }

        ImGui::EndTable();
    }
}

void ECellEngine::Editor::FileIOWidget::drawSearchTargetFile()
{
    static char path[256] = "";
    ImGui::Text("Model file path");
    ImGui::SameLine();
    ImGui::InputText(" ", path, 256);
    ImGui::SameLine();
    if (ImGui::Button("Open"))
    {
        setFilePathCommandArray[1] = path;
        engineCmdsManager->interpretCommand(setFilePathCommandArray);
        engineCmdsManager->interpretCommand(addFileAsSBMLCommandArray);

        if (nbLoadedSBMLDocuments != loadedSBMLDocuments->size())
        {
            nbLoadedSBMLDocuments++;
            loadedSBMLDocumentNames.push_back(loadedSBMLDocuments->back()->getModel()->getId());
        }
    }
}