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
		float stepTime = 0.016;

		const std::vector<std::string> pauseCommandArray = { "pause" };
		const std::vector<std::string> playCommandArray = { "play" };
		const std::vector<std::string> stopCommandArray = { "stop" };

		const std::vector<std::string> goBackwardCommandArray = { "goBackward" };
		const std::vector<std::string> goForwardCommandArray = { "goForward" };
		std::vector<std::string> stepBackwardCommandArray = { "stepBackward", "0.0000001"};
		std::vector<std::string> stepForwardCommandArray = { "stepForward", "0.0000001" };

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