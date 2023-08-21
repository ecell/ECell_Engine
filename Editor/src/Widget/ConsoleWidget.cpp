#include "Style/EditorStyle.hpp"
#include "Widget/ConsoleWidget.hpp"

void ECellEngine::Editor::Widget::ConsoleWidget::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Console"))
	{
		ImGui::End();
		return;
	}

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -50), false, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	for (int i = 0; i < ConsoleWidget::log.size(); i++)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, Style::EditorStyle::GetLogStyle().loggingColors[ConsoleWidget::log[i].lvl]);
		ImGui::TextUnformatted(ConsoleWidget::log[i].msg.c_str());
		ImGui::PopStyleColor();
	}

	if (utilityState & ConsoleState_ScrollDownOnMessageReceive)
	{
		if (utilityState & ConsoleState_MessageReceived)
		{
			ImGui::SetScrollHereY();
			utilityState &= ~ConsoleState_MessageReceived;
		}
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::End();
}