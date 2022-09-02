#include "FileIOWidget.hpp"

void ECellEngine::Editor::FileIOWidget::draw()
{
    ImGui::SetNextWindowSize(ImVec2(530, 60), ImGuiCond_FirstUseEver);
    ImGui::Begin("File Input");

    static char path[256] = "";
    ImGui::Text("Model file path");
    ImGui::SameLine();
    ImGui::InputText(" ", path, 256);
    ImGui::SameLine();
    if (ImGui::Button("Open"))
    {
        engineFileIOManager->setTargetFilePath(path);
        engineCmdsManager->interpretCommand(addFileAsSBMLCommandArray);
    }

    ImGui::End();
}