#pragma once

#include <string>
#include <memory>

#include "EditorConsoleLoggerSink.hpp"
#include "Logger.hpp"
#include "Widget.hpp"
#include "WidgetsVisibility.hpp"

using namespace ECellEngine::Editor::Logging;

namespace ECellEngine::Editor
{
	enum LogLevel {debug, error, trace, warning};

	struct LogMessage
	{
		std::string msg;
		ImVec4 color;

		LogMessage(const LogLevel _lvl, const std::string& _msg) :
			msg{_msg}
		{
			ProcessLevel(_lvl);
		}

		void ProcessLevel(const LogLevel _lvl)
		{
			switch (_lvl)
			{
			case(LogLevel::debug):
				color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);//white
				break;
			case(LogLevel::error):
				color = ImVec4(0.90234375f, 0.296875f, 0.234375f, 1.0f);//dark (-ish) red HEX #e74c3c RGB (231, 76, 60) HSL (6 , 78 %, 57 %)
				break;
			case(LogLevel::trace):
				color = ImVec4(0.3632825f, 0.42578125f, 0.4921825f, 1.0f);//Dark duck blue HEX #5d6d7e RGB (93, 109, 126) HSL (210, 15 %, 43 %)
				break;
			case(LogLevel::warning):
				color = ImVec4(0.94140625f, 0.765625f, 0.05859375f, 1.0f);//dark (-ish) yellow HEX #f1c40f RGB (241, 196, 15) HSL (48, 89 %, 50 %)
				break;
			}
		}
	};

	class ConsoleWidget : public Widget
	{
	
	private:
		EditorConsoleLoggerSink ecLoggerSink;
		std::vector<LogMessage> log;

	public:
		ConsoleWidget(CommandsManager* _cmdManager) :
			Widget(_cmdManager), ecLoggerSink(*this)
		{
			ecLoggerSink.sinkIdx = ECellEngine::Logging::Logger::GetSingleton().AddSink(&ecLoggerSink);
		}

		~ConsoleWidget()
		{
			ECellEngine::Logging::Logger::GetSingleton().RemoveSink(ecLoggerSink.sinkIdx);
		}

		void Draw() override;

		void LogTrace(const std::string& _msg)
		{
			log.push_back(LogMessage(LogLevel::trace, _msg));
		}
		
		void LogDebug(const std::string& _msg)
		{
			log.push_back(LogMessage(LogLevel::debug, _msg));
		}
		
		void LogError(const std::string& _msg)
		{
			log.push_back(LogMessage(LogLevel::error, _msg));
		}
		
		void LogWarning(const std::string& _msg)
		{
			log.push_back(LogMessage(LogLevel::warning, _msg));
		}
	};
}