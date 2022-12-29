#pragma once

#include <iostream>//for debug
#include <string>

#include "Widget.hpp"

//using namespace ECellEngine::IO;

namespace ECellEngine::Editor
{
	class LogWidget : public Widget
	{
	public:
		LogWidget(CommandsManager* _cmdManager) :
			Widget(_cmdManager)
		{

		}

		void Draw() override;

		void Log(const std::string& _msg);
	};
}