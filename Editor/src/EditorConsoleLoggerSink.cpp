//#include "EditorConsoleLoggerSink.hpp"
#include "LogWidget.hpp"

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogTrace(const std::string& _msg) const noexcept
{
	logWidget.LogTrace(_msg);
}

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogDebug(const std::string& _msg) const noexcept
{
	logWidget.LogDebug(_msg);
}

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogError(const std::string& _msg) const noexcept
{
	logWidget.LogError(_msg);
}

void ECellEngine::Editor::Logging::EditorConsoleLoggerSink::LogWarning(const std::string& _msg) const noexcept
{
	logWidget.LogWarning(_msg);
}