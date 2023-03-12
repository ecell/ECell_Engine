#pragma once

#include <vector>
#include <string>

#include "Widget.hpp"

namespace ECellEngine::Editor
{
	/*!
	@brief The window containing the buttons to control the flow of time of
			a simulation.
	@details This window cannot be undocked nor destroyed from the editor. If
			 every other window have been closed, this one will always be here.
			 We do this because these controls are the core of the engine.
	*/
	class SimulationFlowControlWidget : public Widget
	{
	private :

		/*!
		  @brief Flags for the window containing the buttons controlling simulations.
		  @details This window cannot be resized nor collapsed. It also cannot be moved
					nor focused.
		*/
		const ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		/*!
		  @brief Flags for the docking conditions to this window.
		  @details Nothing can be docked on this window. We do this to preserve
					theintegratity and visibility of these important controls.
		*/
		const ImGuiDockNodeFlags dockFlags =
			ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingSplitMe;


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

		inline void Awake() override {};

		void Draw() override;
	};
}