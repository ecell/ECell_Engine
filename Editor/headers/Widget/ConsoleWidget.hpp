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
			ConsoleState_MessageReceived = 1 << 0, //Message received in the last frame
			ConsoleState_ScrollDownOnMessageReceive = 1 << 1 //Automatically scroll down when a message is received
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
		unsigned char utilityState = 0;

	public:
		ConsoleWidget(Editor& _editor) :
			Widget(_editor), ecLoggerSink(*this)
		{
			ecLoggerSink.sinkIdx = ECellEngine::Logging::Logger::AddSink(&ecLoggerSink);

			utilityState |= ConsoleState_ScrollDownOnMessageReceive;
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
			log.push_back(Logging::LogMessage(ECellEngine::Logging::LogLevel_Trace, _msg));
			utilityState |= ConsoleState_MessageReceived;
		}

		/*!
		@brief Display a message of type #ECellEngine::Editor::LogLevel::Debug
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		inline void LogDebug(const char* _msg)
		{
			log.push_back(Logging::LogMessage(ECellEngine::Logging::LogLevel_Debug, _msg));
			utilityState |= ConsoleState_MessageReceived;
		}

		/*!
		@brief Display a message of type #ECellEngine::Logging::LogLevel::Error
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		inline void LogError(const char* _msg)
		{
			log.push_back(Logging::LogMessage(ECellEngine::Logging::LogLevel_Error, _msg));
			utilityState |= ConsoleState_MessageReceived;
		}

		/*!
		@brief Display a message of type ECellEngine::Logging::LogLevel_Warning
			 in this ConsoleWidget of the Editor.
		@param _msg The message to display in the console of the editor.
		*/
		inline void LogWarning(const char* _msg)
		{
			log.push_back(Logging::LogMessage(ECellEngine::Logging::LogLevel_Warning, _msg));
			utilityState |= ConsoleState_MessageReceived;
		}
	};
}