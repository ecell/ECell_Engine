#include "OptionsWidget.hpp"

void ECellEngine::Editor::OptionsWidget::Draw()
{
    static float f = 0.0f;
    static bool* logWidgetVisibility = WidgetsVisibility::GetSingleton().GetLogWidgetVisibility();
    static bool* simulationWidgetVisibility = WidgetsVisibility::GetSingleton().GetSimulationWidgetVisibility();

    ImGui::Begin("Options");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Checkbox("Demo Window", &showDemoWindow);  // Edit bool storing the ImGui Demo window open/close state
    ImGui::Checkbox("Log Window", logWidgetVisibility);  // Edit bool storing ECellEngine editor's log window open/close state
    ImGui::Checkbox("Simulation Window", simulationWidgetVisibility);  // Edit bool storing ECellEngine editor's simulation our window open/close state

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}