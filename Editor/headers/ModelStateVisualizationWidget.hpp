#pragma once
#include <vector>
#include "imgui.h"
#include "implot.h"
#include "Gillespie_NRM_R.hpp"
#include "CommandsManager.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class ModelStateVisualizationWidget : public Widget
	{
	private:
		Gillespie_NRM_R* gillespieSimulationEnv;//a pointer to the member of the same name in the SimulationLoop class.

	public:
		ModelStateVisualizationWidget(CommandsManager* _cmdsManager, Gillespie_NRM_R* _gillespieSimulationEnv) :
			Widget(_cmdsManager), gillespieSimulationEnv(_gillespieSimulationEnv)
		{

		}

		void draw() override;
	};
}