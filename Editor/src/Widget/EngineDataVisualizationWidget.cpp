#include "Widget/EngineDataVisualizationWidget.hpp"

void ECellEngine::Editor::EngineDataVisualizationWidget::drawInstancedWidgets()
{
	for(std::vector<std::unique_ptr<ModelStateVisualizationWidget>>::iterator it = msvWidgets.begin(); it != msvWidgets.end(); it++)
	{
		(*it).get()->draw();
	}
}

void ECellEngine::Editor::EngineDataVisualizationWidget::draw()
{
	ImGui::Begin("Engine Data View Manager");

	if (ImGui::Button("Add Model State Visulizer"))
	{
		msvWidgets.push_back(
			std::unique_ptr<ModelStateVisualizationWidget>(new ModelStateVisualizationWidget(engineCmdsManager, simuLoop, nbMsvWidgets))
		);
		nbMsvWidgets++;
	}

	ImGui::End();

	drawInstancedWidgets();
	
}