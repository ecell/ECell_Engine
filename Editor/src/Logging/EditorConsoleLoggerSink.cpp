#include "Widget/ConsoleWidget.hpp"

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogTrace(const char* _msg) const noexcept
{
	consoleWidget.LogTrace(_msg);
}

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogDebug(const char* _msg) const noexcept
{
	consoleWidget.LogDebug(_msg);
}

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogError(const char* _msg) const noexcept
{
	consoleWidget.LogError(_msg);
}

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogWarning(const char* _msg) const noexcept
{
	consoleWidget.LogWarning(_msg);
}