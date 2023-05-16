#pragma once 

//Forward declaration
namespace  ECellEngine::Editor
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
		ECellEngine::Editor::ConsoleWidget& consoleWidget; 

	public:
		/*!
		@brief The index of this sink in the ECellEngine::Logging::Logger.
		@details This is usefull when we need to remove and destroy this
				 sink from the list in case the console widget that was using
				 it is closed by the user.
		*/
		std::size_t sinkIdx;

		EditorConsoleLoggerSink(ECellEngine::Editor::ConsoleWidget& _logWidget):
			consoleWidget{_logWidget}
		{

		}

		/*!
		@brief Forwards a message (@p _msg) to the ECellEngine::Editor::ConsoleWidget
				where it will be printed as #ECellEngine::Editor::LogLevel::trace.
		*/
		virtual void LogTrace(const std::string& _msg) const noexcept override;
		
		/*!
		@brief Forwards a message (@p _msg) to the ECellEngine::Editor::ConsoleWidget
				where it will be printed as #ECellEngine::Editor::LogLevel::debug.
		*/
		virtual void LogDebug(const std::string& _msg) const noexcept override;
		

		/*!
		@brief Forwards a message (@p _msg) to the ECellEngine::Editor::ConsoleWidget
				where it will be printed as #ECellEngine::Editor::LogLevel::error.
		*/
		virtual void LogError(const std::string& _msg) const noexcept override;
		

		/*!
		@brief Forwards a message (@p _msg) to the ECellEngine::Editor::ConsoleWidget
				where it will be printed as #ECellEngine::Editor::LogLevel::warning.
		*/
		virtual void LogWarning(const std::string& _msg) const noexcept override;
	};
}