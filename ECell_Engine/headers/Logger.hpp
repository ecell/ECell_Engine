#pragma once

#include <memory>
#include <vector>

#include "LoggerSink.hpp"

namespace ECellEngine::Logging
{
	class Logger
	{
	private:
		std::vector<LoggerSink*> loggerSinks;

	public:
		static Logger& GetSingleton();
		/*{
			static Logger singleton;
			return singleton;
		}*/

		inline std::size_t AddSink(LoggerSink* _loggerSink)
		{
			GetSingleton().loggerSinks.push_back(_loggerSink);
			LogTrace("Logger sink added");
			return loggerSinks.size() - 1;
		}

		inline void RemoveSink(std::size_t& _idx)
		{
			LogTrace("Removing Sink");
			GetSingleton().loggerSinks.erase(loggerSinks.begin()+_idx);
		}

		void LogTrace(const std::string& _msg) noexcept;
		
		void LogDebug(const std::string& _msg) noexcept;
		
		void LogError(const std::string& _msg) noexcept;
		
		void LogWarning(const std::string& _msg) noexcept;
	};
}
