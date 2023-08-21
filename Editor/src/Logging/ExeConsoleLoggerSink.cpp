#include <iostream>
#include "Logging/ExeConsoleLoggerSink.hpp"

void ECellEngine::Editor::Logging::ExeConsoleLoggerSink::LogTrace(const char* _msg) const noexcept
{
	std::cout << "[Trace] " << _msg << std::endl;
}


void ECellEngine::Editor::Logging::ExeConsoleLoggerSink::LogDebug(const char* _msg) const noexcept
{
	std::cout << "[Debug] " << _msg << std::endl;
}


void ECellEngine::Editor::Logging::ExeConsoleLoggerSink::LogError(const char* _msg) const noexcept
{
	std::cout << "[Error] " << _msg << std::endl;
}


void ECellEngine::Editor::Logging::ExeConsoleLoggerSink::LogWarning(const char* _msg) const noexcept
{
	std::cout << "[Warning] " << _msg << std::endl;
}