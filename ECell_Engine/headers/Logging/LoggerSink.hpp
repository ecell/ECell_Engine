#pragma once
#include <string>

namespace ECellEngine::Logging
{
	class LoggerSink
	{
	public:
		virtual void LogDebug(const std::string& _msg) const noexcept = 0;
		virtual void LogError(const std::string& _msg) const noexcept = 0;
		virtual void LogTrace(const std::string& _msg) const noexcept = 0;
		virtual void LogWarning(const std::string& _msg) const noexcept = 0;
	};
}