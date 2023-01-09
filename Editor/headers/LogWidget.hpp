#pragma once

#include <iostream>//for debug
#include <string>

#include "Widget.hpp"
#include "WidgetsVisibility.hpp"

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

	class LogWidget : public Widget
	{
	
	private:
		std::vector<LogMessage> log;


	public:
		LogWidget(CommandsManager* _cmdManager) :
			Widget(_cmdManager)
		{
			log.push_back(LogMessage(LogLevel::debug, "This is a debug message"));
			log.push_back(LogMessage(LogLevel::error, "This is an error message"));
			log.push_back(LogMessage(LogLevel::trace, "This is a trace message"));
			log.push_back(LogMessage(LogLevel::warning, "This is a warning message"));
		}

		void Draw() override;

		void Log(const std::string& _msg)
		{
			log.push_back(LogMessage(LogLevel::trace, _msg));
		}
	};
}