#pragma once
#include "Logging/LoggerSink.hpp"

namespace ECellEngine::Editor::Logging
{
	/*!
	@brief The Logger sink to receive log messages and print them in the
			executables console.
	*/
	class ExeConsoleLoggerSink : public ECellEngine::Logging::LoggerSink
	{
	public:

		ExeConsoleLoggerSink() = default;

		/*!
		@brief Forwards a message (@p _msg) to indicate it is of type @a Trace
				and prints it in the console.
		*/
		void LogTrace(const char* _msg) const noexcept override;

		/*!
		@brief Forwards a message (@p _msg) to indicate it is of type @a Debug
				and prints it in the console.
		*/
		void LogDebug(const char* _msg) const noexcept override;

		/*!
		@brief Forwards a message (@p _msg) to indicate it is of type @a Error
				and prints it in the console.
		*/
		void LogError(const char* _msg) const noexcept override;

		/*!
		@brief Forwards a message (@p _msg) to indicate it is of type @a Warning
				and prints it in the console.
		*/
		void LogWarning(const char* _msg) const noexcept override;
	};
}