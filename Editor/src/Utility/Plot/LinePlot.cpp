#include "Util/BinarySearch.hpp"
#include "Utility/Plot/LinePlot.hpp"

std::shared_ptr<ECellEngine::Editor::Utility::Plot::Line> ECellEngine::Editor::Utility::Plot::LinePlot::AddLine(std::size_t _matchedDataID) noexcept
{
	std::vector<std::shared_ptr<Line>>::iterator it = ECellEngine::Util::BinarySearch::LowerBound(lines.begin(), lines.end(), _matchedDataID);
	it = lines.insert(it, std::make_shared<Plot::Line>(_matchedDataID, linesMaxNbDataPoints, &X));

	return *it;
}

void ECellEngine::Editor::Utility::Plot::LinePlot::Draw() noexcept
{
	if (ImPlot::BeginPlot(plotTitle, ImVec2(plotSize[0], plotSize[1]), plotFlags))
	{
		ImPlot::SetupAxes(xAxisLabel, yAxisLabel, xAxisFlags, yAxisFlags);
		ImPlot::SetupAxisScale(ImAxis_X1, xAxisScale);
		ImPlot::SetupAxisScale(ImAxis_Y1, yAxisScale);

		for (std::vector<std::shared_ptr<Line>>::iterator it = lines.begin(); it!=lines.end(); it++)
		{
			(*it)->Draw();
		}
		ImPlot::EndPlot();
	}
}

void ECellEngine::Editor::Utility::Plot::LinePlot::UpdateX(const float _previousValue, const float _newValue)
{
	if (_previousValue != _newValue)
	{
		X = _newValue;
	}
}