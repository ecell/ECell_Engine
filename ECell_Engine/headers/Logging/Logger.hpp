#pragma once

#include <vector>

#include "Logging/LoggerSink.hpp"

namespace ECellEngine::Logging
{
	class Logger
	{
	private:
		std::vector<LoggerSink*> loggerSinks;

	public:
		static Logger& GetSingleton();

		inline std::size_t AddSink(LoggerSink* _loggerSink)
		{
			GetSingleton().loggerSinks.push_back(_loggerSink);
			LogDebug("Logger sink added");
			return loggerSinks.size() - 1;
		}

		inline void RemoveSink(std::size_t& _idx)
		{
			LogDebug("Removing Sink at index: " + std::to_string(_idx));
			GetSingleton().loggerSinks.erase(loggerSinks.begin()+_idx);
		}

		void LogTrace(const std::string& _msg) noexcept;
		
		void LogDebug(const std::string& _msg) noexcept;
		
		void LogError(const std::string& _msg) noexcept;
		
		void LogWarning(const std::string& _msg) noexcept;
	};
}
