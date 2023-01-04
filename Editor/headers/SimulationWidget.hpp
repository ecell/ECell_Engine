#pragma once

#include <string>
#include <vector>

#include "EditorLog.hpp"
#include "Widget.hpp"
#include "WidgetsVisibility.hpp"

namespace ECellEngine::Editor
{
	class SimulationWidget : public Widget
	{
	private:
		std::vector<std::string> addModuleCommandArray = {"addModule", "path"};
	public:
		SimulationWidget(CommandsManager* _cmdsManager) :
			Widget(_cmdsManager)
		{

		}

		void Draw() override;
	};
}