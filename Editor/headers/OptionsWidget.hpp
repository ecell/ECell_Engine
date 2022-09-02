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
		OptionsWidget(bool* _showDemoWindow, CommandsManager* _cmdManager) :
			Widget(_cmdManager), showDemoWindow(_showDemoWindow)
		{

		}

		void draw() override;
	};
}
