#pragma once 

//Forward declaration
namespace  ECellEngine::Editor::Widget
{
	class ConsoleWidget;
}

#include "Logging/LoggerSink.hpp"

namespace ECellEngine::Editor::Logging
{
	/*!
	@brief The logger sink used to collect messages and print them in the
			editor's console.
	*/
	class EditorConsoleLoggerSink : public ECellEngine::Logging::LoggerSink
	{
	private:
		/*!
		@brief Reference to the console where to print the messages.
		*/
		ECellEngine::Editor::Widget::ConsoleWidget& consoleWidget;

	public:
		/*!
		@brief The index of this sink in the ECellEngine::Logging::Logger.
		@details This is usefull when we need to remove and destroy this
				 sink from the list in case the console widget that was using
				 it is closed by the user.
		*/
		std::size_t sinkIdx = 0;

		EditorConsoleLoggerSink(ECellEngine::Editor::Widget::ConsoleWidget& _logWidget):
			consoleWidget{_logWidget}
		{

		}

		/*!
		@brief Forwards a message (@p _msg) to the ECellEngine::Editor::ConsoleWidget
				where it will be printed as #ECellEngine::Editor::LogLevel::trace.
		*/
		void LogTrace(const char* _msg) const noexcept override;
		
		/*!
		@brief Forwards a message (@p _msg) to the ECellEngine::Editor::ConsoleWidget
				where it will be printed as #ECellEngine::Editor::LogLevel::debug.
		*/
		void LogDebug(const char* _msg) const noexcept override;
		

		/*!
		@brief Forwards a message (@p _msg) to the ECellEngine::Editor::ConsoleWidget
				where it will be printed as #ECellEngine::Editor::LogLevel::error.
		*/
		void LogError(const char* _msg) const noexcept override;
		

		/*!
		@brief Forwards a message (@p _msg) to the ECellEngine::Editor::ConsoleWidget
				where it will be printed as #ECellEngine::Editor::LogLevel::warning.
		*/
		void LogWarning(const char* _msg) const noexcept override;
	};
}