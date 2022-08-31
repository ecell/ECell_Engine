#include "Windows.hpp"

void ECellEngine::Editor::Windows::OptionsWindow(bool* _showDemoWindow)
{
    static float f = 0.0f;

    ImGui::Begin("Options");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Checkbox("Demo Window", _showDemoWindow);  // Edit bool storing our window open/close state

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}