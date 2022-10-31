#pragma once

#include <vector>

#include "imgui.h"
#include "implot.h"

#include "Gillespie_NRM_R.hpp"
#include "SimulationLoop.hpp"
#include "CommandsManager.hpp"
#include "Widget.hpp"
#include "ModelStateVisualizationWidget.hpp"


namespace ECellEngine::Editor
{
	/*
	@brief The widget from which every other widget to visualize
			data about the engine will be instanced and stored.
	*/
	class EngineDataVisualizationWidget : public Widget
	{
	private:
		SimulationLoop* simuLoop;

		short nbMsvWidgets;//stored to pass on the newly instanciated msvWidget as their ID.
		std::vector<std::unique_ptr<ModelStateVisualizationWidget>> msvWidgets;//the list of instantiated msvWidgets

		/*
		@brief Draw all the instanciated widgets stored in the
			   different relevant vectors.
		*/
		void drawInstancedWidgets();

	public:
		EngineDataVisualizationWidget(CommandsManager* _cmdsManager, SimulationLoop* _simuLoop) :
			Widget(_cmdsManager), simuLoop(_simuLoop)
		{
			nbMsvWidgets = 0;
		}

		void draw() override;
	};
}