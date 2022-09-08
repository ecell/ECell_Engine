#pragma once

#include <vector>

#include "imgui.h"
#include "implot.h"

#include "ASTNode_parser.hpp"
#include "Gillespie_NRM_R.hpp"
#include "SimulationLoop.hpp"
#include "CommandsManager.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class ModelStateVisualizationWidget : public Widget
	{
	private:
		SimulationLoop* simulationLoop;//a pointer to the member of the same name in the Engine class.
		const std::vector<std::string> displayCommandArray = { "display" };

		int nbSpecies;
		ASTEvaluator* astEvaluator;
		int dataIdxToPlotOnX = -1;
		int dataIdxToPlotOnY = -1;

		void drawLinePlot();

		void drawRawString();

		void drawTable();

	public:
		ModelStateVisualizationWidget(CommandsManager* _cmdsManager, SimulationLoop* _simulationLoop) :
			Widget(_cmdsManager), simulationLoop(_simulationLoop)
		{

		}

		void draw() override;
	};
}