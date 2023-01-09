#pragma once
#include <string>

namespace ECellEngine::Debug
{
	class Logger
	{
	public:
		virtual void Log(const std::string& _msg) const noexcept = 0;
	};
}