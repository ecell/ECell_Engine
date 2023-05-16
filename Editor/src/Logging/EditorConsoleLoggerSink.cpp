#include "Widget/ConsoleWidget.hpp"

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogTrace(const std::string& _msg) const noexcept
{
	consoleWidget.LogTrace(_msg);
}

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogDebug(const std::string& _msg) const noexcept
{
	consoleWidget.LogDebug(_msg);
}

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogError(const std::string& _msg) const noexcept
{
	consoleWidget.LogError(_msg);
}

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogWarning(const std::string& _msg) const noexcept
{
	consoleWidget.LogWarning(_msg);
}