#pragma once

#include <vector>
#include <string>

#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class SimulationFlowControlWidget : public Widget
	{
	private :
		/*SimulationState* simuState;
		short* simuDirection;
		float stepTime = 0.016;

		const std::vector<std::string> pauseCommandArray = { "pause" };
		const std::vector<std::string> playCommandArray = { "play" };
		const std::vector<std::string> stopCommandArray = { "stop" };

		const std::vector<std::string> goBackwardCommandArray = { "goBackward" };
		const std::vector<std::string> goForwardCommandArray = { "goForward" };
		std::vector<std::string> stepBackwardCommandArray = { "stepBackward", "0.0000001"};
		std::vector<std::string> stepForwardCommandArray = { "stepForward", "0.0000001" };*/

		void DrawSimulationControls();

	public:
		SimulationFlowControlWidget(Editor& _editor) :
			Widget(_editor)
		{
			/*simuState = _simuLoop->getSimulationState();
			simuDirection = _simuLoop->getSimulationDirection();*/
		}

		void Draw() override;
	};
}