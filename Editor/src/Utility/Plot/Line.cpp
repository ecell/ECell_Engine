#include "Logging/Logger.hpp"
#include "Utility/Plot/Line.hpp"
#include "implot.h"

ECellEngine::Editor::Utility::Plot::Line::operator std::size_t()
{
	return id;
}

void ECellEngine::Editor::Utility::Plot::Line::Draw()
{
	ImPlot::SetNextLineStyle(color);
	ImPlot::PlotLine(
		lineLegend,
		&dataPoints.Data[0].x, &dataPoints.Data[0].y,
		dataPoints.Data.Size, ImPlotLineFlags_None, dataPoints.Offset, 2 * sizeof(float));
}

void ECellEngine::Editor::Utility::Plot::Line::SwitchUpdateController(unsigned short _scheme) noexcept
{
	switch (_scheme)
	{
	case(UpdateScheme_Never):
		updateController = &updateControllerNever;
		break;
	case(UpdateScheme_Always):
		updateController = &updateControllerAlways;
		break;
	case(UpdateScheme_OnChange):
		updateController = &updateControllerOnChange;
		updateController->Set(ptrY);
		break;
	case(UpdateScheme_EveryNthFrame):
		updateController = &updateControllerEveryNthFrame;
		break;
	case(UpdateScheme_EveryXSeconds):
		updateController = &updateControllerEveryXSeconds;
		break;
	default:
		ECellEngine::Logging::Logger::GetSingleton().GetSingleton().LogError("Invalid update scheme. The data of Line " + std::string(lineLegend) + "is likely corrupted.");
		break;
	}
}

void ECellEngine::Editor::Utility::Plot::Line::Update(float _x) noexcept
{
	if (ptrY)
	{
		if (updateController->TestUpdate())
		{
			dataPoints.AddPoint(_x, *ptrY);
			updateController->Reset();
		}
	}
}