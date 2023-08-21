#pragma once

#include <string>
#include <memory>

#include "Logging/EditorConsoleLoggerSink.hpp"
#include "Logging/Logger.hpp"
#include "Widget.hpp"

using namespace ECellEngine::Editor::Logging;

namespace ECellEngine::Editor::Logging
{
	/*! \file ConsoleWidget.hpp */

	/*!
	@brief The struct containing the data used to correctly
		display a message in the console of the editor.
	*/
	struct LogMessage
	{
		/*!
		@brief The message to display in the console.
		*/
		const std::string msg;

		/*!
		@brief The 8-bit unsigned integer encoding the enum value
				ECellEngine::Logging::Logger::LogLevel of the message.
		*/
		const unsigned char lvl;

		LogMessage(const unsigned char _lvl, const char* _msg) :
			msg{ _msg }, lvl{ _lvl }
		{

		}
	};
}

namespace ECellEngine::Editor::Widget
{
	/*!
	@brief The window displaying a console where messages about the state of
			the simulation are printed.
	*/
	class ConsoleWidget : public Widget
	{
	
	private:
		EditorConsoleLoggerSink ecLoggerSink; /*!< The logger sink used to collect the messages.*/
		std::vector<LogMessage> log; /*< The collection of log messages received.*/

		/*!
		@brief The array to store all colors for the logging system.
		*/
		ImVec4 loggingColors[ECellEngine::Logging::LogLevel_Count];

	public:
		ConsoleWidget(Editor& _editor) :
			Widget(_editor), ecLoggerSink(*this)
		{
			ecLoggerSink.sinkIdx = ECellEngine::Logging::Logger::AddSink(&ecLoggerSink);

			loggingColors[ECellEngine::Logging::LogLevel_Debug] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			loggingColors[ECellEngine::Logging::LogLevel_Error] = ImVec4(0.90234375f, 0.296875f, 0.234375f, 1.0f);//dark (-ish) red HEX #e74c3c RGB (231, 76, 60) HSL (6 , 78 %, 57 %)
			loggingColors[ECellEngine::Logging::LogLevel_Trace] = ImVec4(0.3632825f, 0.42578125f, 0.4921825f, 1.0f);//Dark duck blue HEX #5d6d7e RGB (93, 109, 126) HSL (210, 15 %, 43 %)
			loggingColors[ECellEngine::Logging::LogLevel_Warning] = ImVec4(0.94140625f, 0.765625f, 0.05859375f, 1.0f);//dark (-ish) yellow HEX #f1c40f RGB (241, 196, 15) HSL (48, 89 %, 50 %)

		}

		~ConsoleWidget()
		{
			ECellEngine::Logging::Logger::RemoveSink(ecLoggerSink.sinkIdx);
		}

		inline void Awake() override {};

		void Draw() override;

		/*!
		@brief Display a message of type #ECellEngine::Editor::LogLevel::Trace
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		inline void LogTrace(const char* _msg)
		{
			log.push_back(LogMessage(ECellEngine::Logging::LogLevel_Trace, _msg));
		}
		
		/*!
		@brief Display a message of type #ECellEngine::Editor::LogLevel::Debug
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		inline void LogDebug(const char* _msg)
		{
			log.push_back(LogMessage(ECellEngine::Logging::LogLevel_Debug, _msg));
		}
		
		/*!
		@brief Display a message of type #ECellEngine::Logging::LogLevel::Error
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		inline void LogError(const char* _msg)
		{
			log.push_back(LogMessage(ECellEngine::Logging::LogLevel_Error, _msg));
		}
		
		/*!
		@brief Display a message of type ECellEngine::Logging::LogLevel_Warning
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		inline void LogWarning(const char* _msg)
		{
			log.push_back(LogMessage(ECellEngine::Logging::LogLevel_Warning, _msg));
		}
	};
}