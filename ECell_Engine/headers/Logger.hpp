#pragma once
#include <string>

namespace ECellEngine::Debug
{
	class Logger
	{
	public:
		virtual void Log(const char* _msg) const noexcept = 0;
	};
}