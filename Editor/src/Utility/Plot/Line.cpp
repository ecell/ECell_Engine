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

void ECellEngine::Editor::Utility::Plot::Line::SwitchUpdateStrategy(unsigned char _strategy) noexcept
{
	switch (_strategy)
	{
	case(UpdateStrategyType_Never):
		updateStrategy.reset(new UpdateStrategyNever());
		break;
	case(UpdateStrategyType_Always):
		updateStrategy.reset(new UpdateStrategyAlways());
		break;
	case(UpdateStrategyType_OnChange):
		updateStrategy.reset(new UpdateStrategyOnChange());
		updateStrategy->Set(&Y);
		break;
	case(UpdateStrategyType_EveryNthFrame):
		updateStrategy.reset(new UpdateStrategyEveryNthFrame());
		break;
	case(UpdateStrategyType_EveryXSeconds):
		updateStrategy.reset(new UpdateStrategyEveryXSeconds());
		break;
	default:
		ECellEngine::Logging::Logger::LogError("Invalid update scheme. The data of Line %s is likely corrupted.", lineLegend);
		break;
	}
}

void ECellEngine::Editor::Utility::Plot::Line::UpdateLine(const float _previousValue, const float _newValue) noexcept
{
	Y = _newValue;
	if (updateStrategy->TestUpdate())
	{
		dataPoints.AddPoint(*ptrX, _newValue);
		updateStrategy->Reset();
	}
}