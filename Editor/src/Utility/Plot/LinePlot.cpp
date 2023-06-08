#include "Data/BinaryOperatedVector.hpp"
#include "Utility/Plot/LinePlot.hpp"

ECellEngine::Editor::Utility::Plot::Line& ECellEngine::Editor::Utility::Plot::LinePlot::AddLine(std::size_t _matchedDataID) noexcept
{
	std::vector<Line>::iterator it = ECellEngine::Data::BinaryOperation::LowerBound(lines.begin(), lines.end(), _matchedDataID);
	it = lines.insert(it, Plot::Line(_matchedDataID, linesMaxNbDataPoints));

	return *it;
}

void ECellEngine::Editor::Utility::Plot::LinePlot::Draw() noexcept
{
	if (ImPlot::BeginPlot(plotTitle, ImVec2(plotSize[0], plotSize[1]), plotFlags))
	{
		ImPlot::SetupAxes(xAxisLabel, yAxisLabel, xAxisFlags, yAxisFlags);
		ImPlot::SetupAxisScale(ImAxis_X1, xAxisScale);
		ImPlot::SetupAxisScale(ImAxis_Y1, yAxisScale);

		for (std::vector<Line>::iterator it = lines.begin(); it!=lines.end(); it++)
		{
			it->Draw();
		}
		ImPlot::EndPlot();
	}
}

void ECellEngine::Editor::Utility::Plot::LinePlot::Update() noexcept
{
	if (ptrX)
	{
		for (std::vector<Line>::iterator it = lines.begin(); it != lines.end(); it++)
		{
			it->Update(*ptrX);
		}
	}
}