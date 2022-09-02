#pragma once

#include "imgui.h"
#include "CommandsManager.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class OptionsWidget : public Widget
	{
	private:
		bool* showDemoWindow;

	public:
		OptionsWidget(CommandsManager* _cmdManager, bool* _showDemoWindow) :
			Widget(_cmdManager), showDemoWindow(_showDemoWindow)
		{

		}

		void draw() override;
	};
}
