#include "Widget/OptionsWidget.hpp"
#include "Editor.hpp"//We use editor here so we need to finish the forward declaration initiated in the  base class "Widget"

void ECellEngine::Editor::Widget::OptionsWidget::Draw()
{
    static float f = 0.0f;

    ImGui::Begin("Options");// Create a window called "Hello, world!" and append into it.

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Show Debug Window", &editor.showDemoWindow);
    ImGui::End();
}