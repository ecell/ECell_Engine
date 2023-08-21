#pragma once
#include <string>

namespace ECellEngine::Logging
{
	/*
	@brief LoggerSink is an interface for all sinks that can be added to the static logger.
	@details Derive from this class to create your new logging sink. Then, add the instance
			 of your new sink to the list of sinks in the Logger class. Finally, calling the
			 static methods of the same name (LogXXX) in the Logger class will defer loggin
			 to your new added sink.
	@see ECellEngine::Logging::Logger
	*/
	class LoggerSink
	{
	public:
		/*!
		@brief API to log a DEBUG message.
		*/
		virtual void LogDebug(const char* _msg) const noexcept = 0;

		/*!
		@brief API to log an ERROR message.
		*/
		virtual void LogError(const char* _msg) const noexcept = 0;

		/*!
		@brief API to log a TRACE message.
		*/
		virtual void LogTrace(const char* _msg) const noexcept = 0;

		/*!
		@brief API to log a WARNING message.
		*/
		virtual void LogWarning(const char* _msg) const noexcept = 0;
	};
}