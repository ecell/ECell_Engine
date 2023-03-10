#pragma once
#include <iostream>
#include "LoggerSink.hpp"

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
		virtual void LogTrace(const std::string& _msg) const noexcept override
		{
			std::cout << "[Trace] " << _msg << std::endl;
		}

		/*!
		@brief Forwards a message (@p _msg) to indicate it is of type @a Debug
				and prints it in the console.
		*/
		virtual void LogDebug(const std::string& _msg) const noexcept override
		{
			std::cout << "[Debug] " << _msg << std::endl;
		}

		/*!
		@brief Forwards a message (@p _msg) to indicate it is of type @a Error
				and prints it in the console.
		*/
		virtual void LogError(const std::string& _msg) const noexcept override
		{
			std::cout << "[Error] " << _msg << std::endl;
		}

		/*!
		@brief Forwards a message (@p _msg) to indicate it is of type @a Warning
				and prints it in the console.
		*/
		virtual void LogWarning(const std::string& _msg) const noexcept override
		{
			std::cout << "[Warning] " << _msg << std::endl;
		}
	};
}