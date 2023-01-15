#pragma once 

//Forward declaration
namespace  ECellEngine::Editor
{
	class ConsoleWidget;
}

#include "LoggerSink.hpp"

namespace ECellEngine::Editor::Logging
{
	class EditorConsoleLoggerSink : public ECellEngine::Logging::LoggerSink
	{
	private:
		ECellEngine::Editor::ConsoleWidget& consoleWidget;

	public:
		std::size_t sinkIdx;

		EditorConsoleLoggerSink(ECellEngine::Editor::ConsoleWidget& _logWidget):
			consoleWidget{_logWidget}
		{

		}

		virtual void LogTrace(const std::string& _msg) const noexcept override;
		
		virtual void LogDebug(const std::string& _msg) const noexcept override;
		
		virtual void LogError(const std::string& _msg) const noexcept override;
		
		virtual void LogWarning(const std::string& _msg) const noexcept override;
	};
}