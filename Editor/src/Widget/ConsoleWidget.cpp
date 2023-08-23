#include "Style/EditorStyle.hpp"
#include "Widget/ConsoleWidget.hpp"

void ECellEngine::Editor::Widget::ConsoleWidget::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Console", NULL, ImGuiWindowFlags_MenuBar))
	{
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::SmallButton("Clear"))
		{
			log.clear();
		}

		if (ImGui::BeginMenu("Display"))
		{
			if (ImGui::MenuItem("Everything"))
			{
				utilityState |= ConsoleState_ShowDebugMessages;
				utilityState |= ConsoleState_ShowErrorMessages;
				utilityState |= ConsoleState_ShowTraceMessages;
				utilityState |= ConsoleState_ShowWarningMessages;
			}

			if (ImGui::MenuItem("Nothing"))
			{
				utilityState &= ~ConsoleState_ShowDebugMessages;
				utilityState &= ~ConsoleState_ShowErrorMessages;
				utilityState &= ~ConsoleState_ShowTraceMessages;
				utilityState &= ~ConsoleState_ShowWarningMessages;
			}

			ImGui::Separator();

			ImGui::CheckboxFlags("Debug", (unsigned int*)&utilityState, ConsoleState_ShowDebugMessages);
			ImGui::CheckboxFlags("Error", (unsigned int*)&utilityState, ConsoleState_ShowErrorMessages);
			ImGui::CheckboxFlags("Trace", (unsigned int*)&utilityState, ConsoleState_ShowTraceMessages);
			ImGui::CheckboxFlags("Warning", (unsigned int*)&utilityState, ConsoleState_ShowWarningMessages);

			ImGui::EndMenu();
		}

		ImGui::CheckboxFlags("Auto scroll down", (unsigned int*)&utilityState, ConsoleState_ScrollDownOnMessageReceive);

		ImGui::EndMenuBar();
	}

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -50), false, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	for (int i = 0; i < log.size(); i++)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, Style::EditorStyle::GetLogStyle().loggingColors[log[i].lvl]);

		switch ((ECellEngine::Logging::LogLevel)log[i].lvl)
		{
		case ECellEngine::Logging::LogLevel_Debug:
			if (utilityState & ConsoleState_ShowDebugMessages)
			{
				ImGui::TextUnformatted(log[i].msg.c_str());
			}
			break;

		case ECellEngine::Logging::LogLevel_Error:
			if (utilityState & ConsoleState_ShowErrorMessages)
			{
				ImGui::TextUnformatted(log[i].msg.c_str());
			}
			break;

		case ECellEngine::Logging::LogLevel_Trace:
			if (utilityState & ConsoleState_ShowTraceMessages)
			{
				ImGui::TextUnformatted(log[i].msg.c_str());
			}
			break;

		case ECellEngine::Logging::LogLevel_Warning:
			if (utilityState & ConsoleState_ShowWarningMessages)
			{
				ImGui::TextUnformatted(log[i].msg.c_str());
			}
			break;
		}

		ImGui::PopStyleColor();
	}

	if (utilityState & ConsoleState_ScrollDownOnMessageReceive)
	{
		if (utilityState & ConsoleState_DebugMessageReceived)
		{
			if (utilityState & ConsoleState_ShowDebugMessages)
			{
				ImGui::SetScrollHereY();
				utilityState &= ~ConsoleState_DebugMessageReceived;
			}
		}

		if (utilityState & ConsoleState_ErrorMessageReceived)
		{
			if (utilityState & ConsoleState_ShowErrorMessages)
			{
				ImGui::SetScrollHereY();
				utilityState &= ~ConsoleState_ErrorMessageReceived;
			}
		}

		if (utilityState & ConsoleState_TraceMessageReceived)
		{
			if (utilityState & ConsoleState_ShowTraceMessages)
			{
				ImGui::SetScrollHereY();
				utilityState &= ~ConsoleState_TraceMessageReceived;
			}
		}

		if (utilityState & ConsoleState_WarningMessageReceived)
		{
			if (utilityState & ConsoleState_ShowWarningMessages)
			{
				ImGui::SetScrollHereY();
				utilityState &= ~ConsoleState_WarningMessageReceived;
			}
		}
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::End();
}

void ECellEngine::Editor::Widget::ConsoleWidget::LogDebug(const char* _msg)
{
	log.push_back(Logging::LogMessage(ECellEngine::Logging::LogLevel_Debug, _msg));
	utilityState |= ConsoleState_DebugMessageReceived;
}

void ECellEngine::Editor::Widget::ConsoleWidget::LogError(const char* _msg)
{
	log.push_back(Logging::LogMessage(ECellEngine::Logging::LogLevel_Error, _msg));
	utilityState |= ConsoleState_ErrorMessageReceived;
}

void ECellEngine::Editor::Widget::ConsoleWidget::LogTrace(const char* _msg)
{
	log.push_back(Logging::LogMessage(ECellEngine::Logging::LogLevel_Trace, _msg));
	utilityState |= ConsoleState_TraceMessageReceived;
}

void ECellEngine::Editor::Widget::ConsoleWidget::LogWarning(const char* _msg)
{
	log.push_back(Logging::LogMessage(ECellEngine::Logging::LogLevel_Warning, _msg));
	utilityState |= ConsoleState_WarningMessageReceived;
}