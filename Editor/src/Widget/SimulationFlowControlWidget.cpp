#include "Editor.hpp"//forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::Widget::SimulationFlowControlWidget::DrawSimulationControls()
{
    //static char* simuState = "stopped";
    static bool isPlaying = false;
    static ImVec4 simuStateColor = ImVec4(0.191f, 0.845f, 0.249f, 1.000f);

    //ImGui::Text("This is the base of the Simulation Flow Controls.");

    ImGui::PushStyleColor(ImGuiCol_Button, simuStateColor);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
    //if (*simuState == SimulationState::isPlaying)
    //{
    //    ImGui::BeginDisabled();
    //    isPlaying = true;//We duplicate the information holded by simuState to be one frame late with isPlaying
    //                     //This is necessary to avoid asking to EndDisable() if we have yet to ask for BeginDisable
    //                     //because the value of *simuState changes inside the potentially disabled region.
    //}

    if (ImGui::Button("Play"))
    {
        if (editor.engine.GetCommandsManager()->interpretCommand(playCommandArray))
        {
            simuStateColor = ImVec4(0.902f, 0.272f, 0.070f, 1.000f);
            isPlaying = true;
        }
    }
    //ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
    //ImGui::InputFloat("Step time", &stepTime, 0.001f, 0.0f, "%e");
    //ImGui::PopStyleColor(1);
    ////ImGui::SameLine();
    //if (ImGui::Button("Step Backward"))
    //{
    //    stepBackwardCommandArray[1] = std::to_string(stepTime);
    //    engineCmdsManager->interpretCommand(stepBackwardCommandArray);
    //}
    //ImGui::SameLine();
    //if (ImGui::Button("Step Forward"))
    //{
    //    stepForwardCommandArray[1] = std::to_string(stepTime);
    //    engineCmdsManager->interpretCommand(stepForwardCommandArray);
    //}

    if (isPlaying)
    {
        //ImGui::EndDisabled();
        ImGui::SameLine();

        /*if (*simuDirection == 1)
        {
            if (ImGui::Button("Backward"))
            {
                editor.engine->GetCommandsManager()->interpretCommand(goBackwardCommandArray);
            }
        }
        else
        {
            if (ImGui::Button("Forward"))
            {
                editor.engine->GetCommandsManager()->interpretCommand(goForwardCommandArray);
            }
        }*/

        if (ImGui::Button("Pause"))
        {
            if (editor.engine.GetCommandsManager()->interpretCommand(pauseCommandArray))
            {
                isPlaying = false;
                simuStateColor = ImVec4(1.000f, 0.794f, 0.000f, 1.000f);
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Stop"))
        {
            if (editor.engine.GetCommandsManager()->interpretCommand(stopCommandArray))
            {
                isPlaying = false;
                simuStateColor = ImVec4(0.191f, 0.845f, 0.249f, 1.000f);
            }
        }
    }

    ImGui::PopStyleColor(2);

}

void ECellEngine::Editor::Widget::SimulationFlowControlWidget::Draw()
{
    ImGui::Begin("Simulation Flow Control", NULL, windowFlags);

    ImGui::GetWindowDockNode()->LocalFlags |= dockFlags;
    ImGui::DockBuilderSetNodeSize(ImGui::GetWindowDockID(), ImVec2(ImGui::GetWindowSize().x, 75.f));

    DrawSimulationControls();

    ImGui::End();
}

void ECellEngine::Editor::Widget::SimulationFlowControlWidget::SetSimulation(std::size_t _simuIdx)
{
    std::to_chars(simuIdxAsChar, simuIdxAsChar + 8, _simuIdx);
    simulation = ECellEngine::Core::SimulationsManager::GetSingleton().GetSimulation(_simuIdx);
}