#pragma once

#define CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdarg>
#include <vector>

#include "Logging/LoggerSink.hpp"

namespace ECellEngine::Logging
{
	/*!
	@brief Static struct to handle logging to different sinks.
	*/
	struct Logger
	{
		/*!
		@brief Maximum size of a message that can be logged at a time.
		*/
		static constexpr unsigned short maxMsgBufferSize = 1024;

		/*!
		@brief Buffer to store the message to be logged.
		@details This is sent to sinks.
		*/
		static char msgBuffer[maxMsgBufferSize];

		/*!
		@brief The sinks to log to.
		*/
		static std::vector<LoggerSink*> loggerSinks;

		/*!
		@brief Adds a sink to ::loggerSinks.
		@param _loggerSink The sink to add.
		*/
		static std::size_t AddSink(LoggerSink* _loggerSink);

		/*!
		@brief Assembles and sends a message to all sinks with the TRACE level.
		@param _fmt The message to log containing formating anchors.
		@param ... The arguments to replace the formating anchors with.
		*/
		static void LogTrace(const char* _fmt, ...) noexcept;
		
		/*!
		@brief Assembles and sends a message to all sinks with the DEBUG level.
		@param _fmt The message to log containing formating anchors.
		@param ... The arguments to replace the formating anchors with.
		*/
		static void LogDebug(const char* _fmt, ...) noexcept;
		
		/*!
		@brief Assembles and sends a message to all sinks with the ERROR level.
		@param _fmt The message to log containing formating anchors.
		@param ... The arguments to replace the formating anchors with.
		*/
		static void LogError(const char* _fmt, ...) noexcept;
		
		/*!
		@brief Assembles and sends a message to all sinks with the WARNING level.
		@param _fmt The message to log containing formating anchors.
		@param ... The arguments to replace the formating anchors with.
		*/
		static void LogWarning(const char* _fmt, ...) noexcept;

		/*!
		@brief Removes the sink at index @p _idx from ::loggerSinks.
		@param _idx The index of the sink to remove.
		*/
		static void RemoveSink(std::size_t& _idx);
	};
}
