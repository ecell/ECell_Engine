#include "Logging/Logger.hpp"
#include <iostream>

ECellEngine::Logging::Logger& ECellEngine::Logging::Logger::GetSingleton()
{
	static ECellEngine::Logging::Logger singleton;
	return singleton;
}

void ECellEngine::Logging::Logger::LogTrace(const std::string& _msg) noexcept
{
	for (std::vector<LoggerSink*>::iterator it = loggerSinks.begin(); it != loggerSinks.end(); it++)
	{
		(*it)->LogTrace(_msg);
	}
}

void ECellEngine::Logging::Logger::LogDebug(const std::string& _msg) noexcept
{
	for (std::vector<LoggerSink*>::iterator it = loggerSinks.begin(); it != loggerSinks.end(); it++)
	{
		(*it)->LogDebug(_msg);
	}
}

void ECellEngine::Logging::Logger::LogError(const std::string& _msg) noexcept
{
	for (std::vector<LoggerSink*>::iterator it = loggerSinks.begin(); it != loggerSinks.end(); it++)
	{
		(*it)->LogError(_msg);
	}
}

void ECellEngine::Logging::Logger::LogWarning(const std::string& _msg) noexcept
{
	for (std::vector<LoggerSink*>::iterator it = loggerSinks.begin(); it != loggerSinks.end(); it++)
	{
		(*it)->LogWarning(_msg);
	}
}