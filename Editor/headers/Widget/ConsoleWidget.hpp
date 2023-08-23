#pragma once

#include <string>
#include <memory>

#include "Logging/EditorConsoleLoggerSink.hpp"
#include "Logging/Logger.hpp"
#include "Widget.hpp"

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
		
		/*!
		@brief The enum to describe the state of the console logger.
		*/
		enum ConsoleState : unsigned char
		{
			ConsoleState_None = 0,
			ConsoleState_ScrollDownOnMessageReceive = 1 << 0, //Automatically scroll down when a message is received,
			ConsoleState_ShowDebugMessages = 1 << 1, //Show debug messages
			ConsoleState_ShowErrorMessages = 1 << 2, //Show error messages
			ConsoleState_ShowTraceMessages = 1 << 3, //Show trace messages
			ConsoleState_ShowWarningMessages = 1 << 4, //Show warning messages
			ConsoleState_DebugMessageReceived = 1 << 5, //Debug message received in the last frame
			ConsoleState_ErrorMessageReceived = 1 << 6, //Error message received in the last frame
			ConsoleState_TraceMessageReceived = 1 << 7, //Trace message received in the last frame
			ConsoleState_WarningMessageReceived = 1 << 8, //Warning message received in the last frame
		};

		/*!
		@brief The logger sink used to collect the messages.
		*/
		ECellEngine::Editor::Logging::EditorConsoleLoggerSink ecLoggerSink;
		
		/*
		@brief The collection of log messages received.
		*/
		std::vector<ECellEngine::Editor::Logging::LogMessage> log;

		/*!
		@brief The 8-bit unsigned integer encoding state information about the 
				console logger.
		@see ECellEngine::Editor::Widget::ConsoleWidget::ConsoleState
		*/
		unsigned char utilityState = ConsoleState_ScrollDownOnMessageReceive |
									 ConsoleState_ShowDebugMessages |
									 ConsoleState_ShowErrorMessages |
									 ConsoleState_ShowTraceMessages |
									 ConsoleState_ShowWarningMessages;

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
		@brief Display a message of type #ECellEngine::Editor::LogLevel::Debug
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		void LogDebug(const char* _msg);

		/*!
		@brief Display a message of type #ECellEngine::Logging::LogLevel::Error
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		void LogError(const char* _msg);

		/*!
		@brief Display a message of type #ECellEngine::Editor::LogLevel::Trace
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		void LogTrace(const char* _msg);

		/*!
		@brief Display a message of type ECellEngine::Logging::LogLevel_Warning
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		void LogWarning(const char* _msg);
	};
}