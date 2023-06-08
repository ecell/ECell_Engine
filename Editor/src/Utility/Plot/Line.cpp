#include "Utility/Plot/Line.hpp"

ECellEngine::Editor::Utility::Plot::Line::operator std::size_t()
{
	return id;
}


void ECellEngine::Editor::Utility::Plot::Line::Draw() noexcept
{
	ImPlot::SetNextLineStyle(color);
	ImPlot::PlotLine(
		lineLegend,
		&dataPoints.Data[0].x, &dataPoints.Data[0].y,
		dataPoints.Data.Size, ImPlotLineFlags_None, dataPoints.Offset, 2 * sizeof(float));
}

void ECellEngine::Editor::Utility::Plot::Line::Update(float _x) noexcept
{
	if (ptrY)
	{
		dataPoints.AddPoint(_x, *ptrY);
	}
}