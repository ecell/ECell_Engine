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
	class EngineDataVisualizationWidget : public Widget
	{
	private:
		CommandsManager* cmdsManager;
		SimulationLoop* simuLoop;

		short nbMsvWidgets;
		std::vector<std::unique_ptr<ModelStateVisualizationWidget>> msvWidgets;

		void drawInstancedWidgets();

	public:
		EngineDataVisualizationWidget(CommandsManager* _cmdsManager, SimulationLoop* _simuLoop) :
			Widget(_cmdsManager), cmdsManager(_cmdsManager), simuLoop(_simuLoop)
		{
			nbMsvWidgets = 0;
		}

		void draw() override;
	};
}