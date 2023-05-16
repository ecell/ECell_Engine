#pragma once

#include <vector>

#include "imgui.h"
#include "implot.h"

#include "ASTNode_parser.hpp"
#include "Gillespie_NRM_R.hpp"
#include "SimulationLoop.hpp"
#include "CommandsManager.hpp"

#include "Widget.hpp"

#include "ModelNodeBasedViewerGlobal.hpp"

using namespace ECellEngine::Core;
using namespace ECellEngine::IO;

namespace ECellEngine::Editor::Widget
{
    /*!
    @brief The class for the widget to visuals the data stored in the model state.
    @remark There can be several instance of this widget. The instances are managed
            by the ::EngineDataVisualizationWidget.
    */
	class ModelStateVisualizationWidget : public Widget
	{
	private:
        char widgetNameID[32];//The unique name of the instance of the widget.
		SimulationLoop* simulationLoop;//a pointer to the member of the same name in the Engine class.

        const char* visualizationOptions[2] = { "Line Plot", "Quantity Table" };//The names of the tabs that can be instantiated within the widget.
        std::vector<LinePlotTabData> lpTabsData;//The vector of tabs displaying a line plot.
        std::vector<TabData> qtTabsData;//The vector of tabs displaying the table of quantity of species in the model state.

		int nbSpecies;//A buffer integer to know the number of species in the model state without querying every time.
		ASTEvaluator* astEvaluator;//A pointer to the model state
        std::vector<std::string> speciesNamesBuffer;//A buffer vector to know the names of the species in the model state without querying every time.

        /*!
        @brief The function used to draw all the widgets of a line plot tab.
        */
		void drawLinePlotTab(LinePlotTabData* _lpTabData);

        /*!
        @brief The function used to draw all the widgets of a quantity table.
        */
		void drawQuantityTableTab(TabData* _tabData);

	public:
		ModelStateVisualizationWidget(CommandsManager* _cmdsManager, SimulationLoop* _simulationLoop, short _id) :
			Widget(_cmdsManager), simulationLoop(_simulationLoop)
		{
			astEvaluator = nullptr;
			nbSpecies = 0;
            sprintf(widgetNameID, "Model State Visualization##%d", _id);
		}

		void draw() override;
	};
}