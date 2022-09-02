#include "SimulationFlowControlWidget.hpp"

void ECellEngine::Editor::SimulationFlowControlWidget::draw()
{
    static bool isPlaying = false;

    ImGui::Begin("Simulation Flow Control");

    if (!isPlaying)
    {
        if (ImGui::Button("Play"))
        {
            //engineCmdsManager->interpretCommand(playCommandArray);
            isPlaying = true;
        }
    }
    else
    {
        if (ImGui::Button("Pause"))
        {
            //engineCmdsManager->interpretCommand(pauseCommandArray);
            isPlaying = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Stop"))
        {
            //engineCmdsManager->interpretCommand(stopCommandArray);
            isPlaying = false;
        }
    }

    ImGui::End();
}