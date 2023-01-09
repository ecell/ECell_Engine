#include "LogWidget.hpp"

void ECellEngine::Editor::LogWidget::Draw()
{
    static bool* visibility = WidgetsVisibility::GetSingleton().GetLogWidgetVisibility();
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (*visibility)
    {
        if (!ImGui::Begin("Console", visibility))
        {
            ImGui::End();
            return;
        }

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -50), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

        for (int i = 0; i < LogWidget::log.size(); i++)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, LogWidget::log[i].color);
            ImGui::TextUnformatted(LogWidget::log[i].msg.c_str());
            ImGui::PopStyleColor();
        }

        ImGui::PopStyleVar();
        ImGui::EndChild();

        ImGui::End();
    }
}