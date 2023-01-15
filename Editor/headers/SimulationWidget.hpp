#pragma once

#include <string>
#include <vector>

#include "Logger.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
	class SimulationWidget : public Widget
	{
	private:
		std::vector<std::string> addModuleCommandArray = {"addModule", "path"};
	public:
		SimulationWidget(Editor& _editor) :
			Widget(_editor)
		{

		}

		void Draw() override;
	};
}