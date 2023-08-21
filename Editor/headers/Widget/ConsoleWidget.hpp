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

	public:
		ConsoleWidget(Editor& _editor) :
			Widget(_editor), ecLoggerSink(*this)
		{
			ecLoggerSink.sinkIdx = ECellEngine::Logging::Logger::AddSink(&ecLoggerSink);
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