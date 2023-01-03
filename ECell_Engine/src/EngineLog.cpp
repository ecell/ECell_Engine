#include "EngineLog.hpp"

std::unique_ptr<ECellEngine::Debug::EngineLog> ECellEngine::Debug::EngineLog::singleton = nullptr;

ECellEngine::Debug::EngineLog& ECellEngine::Debug::EngineLog::GetSingleton() noexcept
{
	if (singleton == nullptr)
	{
		singleton = std::make_unique<ECellEngine::Debug::EngineLog>();
	}
	return *singleton.get();
}