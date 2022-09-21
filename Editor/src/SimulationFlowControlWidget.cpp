#include "SimulationFlowControlWidget.hpp"

void ECellEngine::Editor::SimulationFlowControlWidget::drawSimulationControls()
{
    //static char* simuState = "stopped";
    static bool isPlaying = false;
    static ImVec4 simuStateColor = ImVec4(0.191f, 0.845f, 0.249f, 1.000f);

    
    ImGui::PushStyleColor(ImGuiCol_Button, simuStateColor);
    if (*simuState == SimulationState::isPlaying)
    {
        ImGui::BeginDisabled();
        isPlaying = true;//We duplicate the information holded by simuState to be one frame late with isPlaying
                         //This is necessary to avoid asking to EndDisable() if we have yet to ask for BeginDisable
                         //because the value of *simuState changes inside the potentially disabled region.
    }

        if (ImGui::Button("Play"))
        {
            engineCmdsManager->interpretCommand(playCommandArray);
            simuStateColor = ImVec4(0.902f, 0.272f, 0.070f, 1.000f);
        }
        ImGui::SameLine();
        if (ImGui::Button("Step Backward"))
        {
            engineCmdsManager->interpretCommand(stepBackwardCommandArray);
        }
        ImGui::SameLine();
        if (ImGui::Button("Step Forward"))
        {
            engineCmdsManager->interpretCommand(stepForwardCommandArray);
        }

    if (isPlaying)
    {
        ImGui::EndDisabled();
        ImGui::SameLine();

        if (*simuDirection == 1)
        {
            if (ImGui::Button("Backward"))
            {
                engineCmdsManager->interpretCommand(goBackwardCommandArray);
            }
        }
        else
        {
            if (ImGui::Button("Forward"))
            {
                engineCmdsManager->interpretCommand(goForwardCommandArray);
            }
        }

        if (ImGui::Button("Pause"))
        {
            engineCmdsManager->interpretCommand(pauseCommandArray);
            isPlaying = false;
            simuStateColor = ImVec4(0.976f, 0.937f, 0.148f, 1.000f);
        }

        ImGui::SameLine();

        if (ImGui::Button("Stop"))
        {
            engineCmdsManager->interpretCommand(stopCommandArray);
            isPlaying = false;
            simuStateColor = ImVec4(0.191f, 0.845f, 0.249f, 1.000f);
        }
    }

    ImGui::PopStyleColor();

}

void ECellEngine::Editor::SimulationFlowControlWidget::draw()
{
    ImGui::Begin("Simulation Flow Control");
    
    drawSimulationControls();

    ImGui::End();
}