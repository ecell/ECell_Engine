#include "Logging/Logger.hpp"

char ECellEngine::Logging::Logger::msgBuffer[ECellEngine::Logging::Logger::maxMsgBufferSize] = { 0 };

std::vector<ECellEngine::Logging::LoggerSink*> ECellEngine::Logging::Logger::loggerSinks = std::vector<LoggerSink*>();

std::size_t ECellEngine::Logging::Logger::AddSink(LoggerSink* _loggerSink)
{
	loggerSinks.push_back(_loggerSink);
	LogDebug("Logger sink added");
	return loggerSinks.size() - 1;
}

void ECellEngine::Logging::Logger::LogTrace(const char* _fmt, ...) noexcept
{
	va_list args;
	va_start(args, _fmt);

	vsnprintf(msgBuffer, maxMsgBufferSize, _fmt, args);

	for (std::vector<LoggerSink*>::iterator it = loggerSinks.begin(); it != loggerSinks.end(); it++)
	{
		(*it)->LogTrace(msgBuffer);
	}

	va_end(args);
}

void ECellEngine::Logging::Logger::LogDebug(const char* _fmt, ...) noexcept
{
	va_list args;
	va_start(args, _fmt);

	vsnprintf(msgBuffer, maxMsgBufferSize, _fmt, args);
	for (std::vector<LoggerSink*>::iterator it = loggerSinks.begin(); it != loggerSinks.end(); it++)
	{
		(*it)->LogDebug(msgBuffer);
	}

	va_end(args);
}

void ECellEngine::Logging::Logger::LogError(const char* _fmt, ...) noexcept
{
	va_list args;
	va_start(args, _fmt);

	vsnprintf(msgBuffer, maxMsgBufferSize, _fmt, args);
	for (std::vector<LoggerSink*>::iterator it = loggerSinks.begin(); it != loggerSinks.end(); it++)
	{
		(*it)->LogError(msgBuffer);
	}

	va_end(args);
}

void ECellEngine::Logging::Logger::LogWarning(const char* _fmt, ...) noexcept
{
	va_list args;
	va_start(args, _fmt);

	vsnprintf(msgBuffer, maxMsgBufferSize, _fmt, args);
	for (std::vector<LoggerSink*>::iterator it = loggerSinks.begin(); it != loggerSinks.end(); it++)
	{
		(*it)->LogWarning(msgBuffer);
	}

	va_end(args);
}
void ECellEngine::Logging::Logger::RemoveSink(std::size_t& _idx)
{
	LogDebug("Removing Sink at index: %llu", _idx);
	loggerSinks.erase(loggerSinks.begin() + _idx);
}
