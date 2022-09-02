#pragma once

#include <vector>
#include <string>

#include "imgui.h"

#include "CommandsManager.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class SimulationFlowControlWidget : public Widget
	{
	private :
		const std::vector<std::string> pauseCommandArray = { "pause" };
		const std::vector<std::string> playCommandArray = { "play" };
		const std::vector<std::string> stopCommandArray = { "stop" };

	public:
		SimulationFlowControlWidget(CommandsManager* _cmdsManager) :
			Widget(_cmdsManager)
		{
		}

		void draw() override;
	};
}