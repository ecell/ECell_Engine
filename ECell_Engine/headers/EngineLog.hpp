#pragma once

#include <memory>
#include <string>
#include "Logger.hpp"

namespace ECellEngine::Debug
{
	class EngineLog : public Logger
	{
	private:
		const Logger* listener;
		static std::unique_ptr<EngineLog> singleton;

	public:
		static std::unique_ptr<EngineLog>& GetSingleton() noexcept;

		inline void SetListener(const Logger* _listener)
		{
			listener = _listener;
		}

		inline void Log(const char* _msg) const noexcept override
		{
			listener->Log(_msg);
		}
	};
}
