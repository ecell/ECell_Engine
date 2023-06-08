#pragma once 

#include <vector>

#include "Utility/Plot/Line.hpp"

namespace ECellEngine::Editor::Utility::Plot
{
	struct LinePlot
	{
		char name[64] = "Line Plot";
		char plotTitle[64] = "PlotTitle";
		char xAxisLabel[64] = "x";
		char yAxisLabel[64] = "y";
		float plotSize[2] = { ImPlot::GetStyle().PlotDefaultSize.x, ImPlot::GetStyle().PlotDefaultSize.y };

		float* ptrX = nullptr;
		unsigned short linesMaxNbDataPoints = 1024;

		std::vector<Line> lines;

		static const ImGuiWindowFlags plotWindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
		ImPlotFlags plotFlags = ImPlotFlags_NoLegend | ImPlotFlags_NoInputs;
		ImPlotScale xAxisScale = ImPlotScale_Linear;
		ImPlotScale yAxisScale = ImPlotScale_Linear;
		ImPlotAxisFlags xAxisFlags = ImPlotAxisFlags_AutoFit;
		ImPlotAxisFlags yAxisFlags = ImPlotAxisFlags_AutoFit;

		LinePlot() = default;

		LinePlot(const LinePlot& _lp) :
			ptrX{ _lp.ptrX }, lines{ _lp.lines },
			plotSize{ _lp.plotSize[0], _lp.plotSize[1] },
			plotFlags{ _lp.plotFlags },
			xAxisScale{ _lp.xAxisScale }, yAxisScale{ _lp.yAxisScale },
			xAxisFlags{ _lp.xAxisFlags }, yAxisFlags{ _lp.yAxisFlags }
		{
			std::memcpy(plotTitle, _lp.plotTitle, 64);
			std::memcpy(xAxisLabel, _lp.xAxisLabel, 64);
			std::memcpy(yAxisLabel, _lp.yAxisLabel, 64);
		}

		Line& AddLine(std::size_t _matchedDataID) noexcept;

		void Draw() noexcept;

		void Update() noexcept;
	};
}