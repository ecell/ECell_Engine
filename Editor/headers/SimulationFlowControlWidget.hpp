#pragma once

#include <vector>
#include <string>

#include "imgui.h"
#include "SimulationLoop.hpp"
#include "CommandsManager.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class SimulationFlowControlWidget : public Widget
	{
	private :
		SimulationState* simuState;
		short* simuDirection;

		const std::vector<std::string> pauseCommandArray = { "pause" };
		const std::vector<std::string> playCommandArray = { "play" };
		const std::vector<std::string> stopCommandArray = { "stop" };

		const std::vector<std::string> gobackwardCommandArray = { "gobackward" };
		const std::vector<std::string> goforwardCommandArray = { "goforward" };

		void drawSimulationControls();

	public:
		SimulationFlowControlWidget(CommandsManager* _cmdsManager, SimulationLoop* _simuLoop) :
			Widget(_cmdsManager)
		{
			simuState = _simuLoop->getSimulationState();
			simuDirection = _simuLoop->getSimulationDirection();

		}

		void draw() override;
	};
}