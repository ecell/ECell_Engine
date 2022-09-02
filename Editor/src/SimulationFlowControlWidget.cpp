#include "SimulationFlowControlWidget.hpp"

void ECellEngine::Editor::SimulationFlowControlWidget::draw()
{
    static bool isPlaying = false;
    static char* simuState = "stopped";
    static ImVec4 simuStateColor = ImVec4(0.191f, 0.845f, 0.249f, 1.000f);

    ImGui::Begin("Simulation Flow Control");

    ImGui::Text("Simulation is");
    ImGui::SameLine();
    ImGui::TextColored(simuStateColor, simuState);
    if (!isPlaying)
    {
        if (ImGui::Button("Play"))
        {
            //engineCmdsManager->interpretCommand(playCommandArray);
            isPlaying = true;
            simuState = "playing";
            simuStateColor = ImVec4(0.902f, 0.272f, 0.070f, 1.000f);
        }
    }
    else
    {
        if (ImGui::Button("Pause"))
        {
            //engineCmdsManager->interpretCommand(pauseCommandArray);
            isPlaying = false;
            simuState = "paused";
            simuStateColor = ImVec4(0.976f, 0.937f, 0.148f, 1.000f);
        }

        ImGui::SameLine();

        if (ImGui::Button("Stop"))
        {
            //engineCmdsManager->interpretCommand(stopCommandArray);
            isPlaying = false;
            simuState = "stopped";
            simuStateColor = ImVec4(0.191f, 0.845f, 0.249f, 1.000f);
        }
    }

    ImGui::End();
}