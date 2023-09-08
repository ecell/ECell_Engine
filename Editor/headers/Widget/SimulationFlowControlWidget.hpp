#pragma once

#include <charconv>
#include <vector>

#include "imgui_internal.h"

#include "IO/CommandArgs.hpp"
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
			ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingSplitMe | ImGuiDockNodeFlags_NoTabBar;

		ECellEngine::Core::Simulation* simulation = nullptr;
		//short* simuDirection;
		float stepTime = 0.01;

		IO::SimulationCommandArgs pauseCommandArgs;
		IO::SimulationCommandArgs playCommandArgs;
		IO::SimulationCommandArgs stopCommandArgs;

		//std::vector<std::string> goBackwardCommandArgs = { "goBackward" };
		//std::vector<std::string> goForwardCommandArgs = { "goForward" };
		//std::vector<std::string> stepBackwardCommandArgs = { "stepSimulationBackward", "0.0000001"};
		IO::StepSimulationCommandArgs stepForwardCommandArgs;

		void DrawSimulationControls();

	public:
		SimulationFlowControlWidget(Editor* _editor) :
			Widget(_editor)
		{
			pauseCommandArgs.simulationID = 0;
			playCommandArgs.simulationID = 0;
			stopCommandArgs.simulationID = 0;

			stepForwardCommandArgs.simulationID = 0;
			stepForwardCommandArgs.deltaTime = 0.01;
		}

		void SetSimulation(Core::Simulation* _simulation);

		inline void Awake() override {};

		void Draw() override;
	};
}