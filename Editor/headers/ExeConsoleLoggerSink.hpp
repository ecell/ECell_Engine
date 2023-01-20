#pragma once
#include <iostream>
#include "LoggerSink.hpp"

namespace ECellEngine::Editor::Logging
{
	class ExeConsoleLoggerSink : public ECellEngine::Logging::LoggerSink
	{
	public:

		ExeConsoleLoggerSink() = default;

		virtual void LogTrace(const std::string& _msg) const noexcept override
		{
			std::cout << "[Trace] " << _msg << std::endl;
		}

		virtual void LogDebug(const std::string& _msg) const noexcept override
		{
			std::cout << "[Debug] " << _msg << std::endl;
		}

		virtual void LogError(const std::string& _msg) const noexcept override
		{
			std::cout << "[Error] " << _msg << std::endl;
		}

		virtual void LogWarning(const std::string& _msg) const noexcept override
		{
			std::cout << "[Warning] " << _msg << std::endl;
		}
	};
}