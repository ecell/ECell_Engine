#include "EngineLog.hpp"

using namespace ECellEngine::Debug;

std::unique_ptr<EngineLog> EngineLog::singleton = nullptr;

std::unique_ptr<EngineLog>& EngineLog::GetSingleton() noexcept 
{
	if (singleton == nullptr)
	{
		singleton = std::make_unique<EngineLog>();
	}
	return singleton;
}