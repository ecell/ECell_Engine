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

        for (int i = 0; i < LogWidget::log.Size; i++)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, LogWidget::log[i].color);
            ImGui::TextUnformatted(LogWidget::log[i].msg);
            ImGui::PopStyleColor();
        }

        ImGui::PopStyleVar();
        ImGui::EndChild();

        ImGui::End();
    }
}

void ECellEngine::Editor::LogWidget::Log(const char* _msg)
{
	std::cout << "Inside the Log method of LogWidget" << std::endl;
	log.push_back(LogMessage(LogLevel::trace, _msg));
}