#pragma once

#include <charconv>
#include <vector>
#include <string>

#include "imgui_internal.h"

#include "Widget.hpp"

namespace ECellEngine::Core
{
	class Simulation;
}

namespace ECellEngine::Editor::Widget
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

		char simuIdxAsChar[8] = "0";
		ECellEngine::Core::Simulation* simulation = nullptr;
		//short* simuDirection;
		float stepTime = 0.01;

		const std::vector<std::string> pauseCommandArray = { "pauseSimulation" , simuIdxAsChar };
		const std::vector<std::string> playCommandArray = { "playSimulation", simuIdxAsChar };
		const std::vector<std::string> stopCommandArray = { "stopSimulation", simuIdxAsChar };

		//const std::vector<std::string> goBackwardCommandArray = { "goBackward" };
		//const std::vector<std::string> goForwardCommandArray = { "goForward" };
		//std::vector<std::string> stepBackwardCommandArray = { "stepSimulationBackward", "0.0000001"};
		std::vector<std::string> stepForwardCommandArray = { "stepSimulationForward", simuIdxAsChar, std::to_string(stepTime)};

		void DrawSimulationControls();

	public:
		SimulationFlowControlWidget(Editor& _editor) :
			Widget(_editor)
		{
			
		}

		void SetSimulation(std::size_t _simuIdx);

		inline void Awake() override {};

		void Draw() override;
	};
}