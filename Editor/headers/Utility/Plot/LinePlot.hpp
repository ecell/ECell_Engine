#pragma once 

#include <vector>

#include "implot.h"

#include "Utility/Plot/Line.hpp"

namespace ECellEngine::Editor::Utility::Plot
{
	/*!
	@brief A Plot to display lines.
	@see ECellEngine::Editor::Utility::Plot::Line, ECellEngine::Editor::Utility::MNBV::LinePlotNodeData
	*/
	struct LinePlot
	{
		/*!
		@brief The name of this type of plot
		@remark Will probably disappear when Reflexion is implemented.
		*/
		char name[64] = "Line Plot";

		/*!
		@brief The title describing what the plot is about.
		*/
		char plotTitle[64] = "PlotTitle";

		/*!
		@brief The label describing the X axis.
		*/
		char xAxisLabel[64] = "x";

		/*!
		@brief The label describing the Y axis.
		*/
		char yAxisLabel[64] = "y";

		/*!
		@brief The size of the plot.
		*/
		float plotSize[2] = { ImPlot::GetStyle().PlotDefaultSize.x, ImPlot::GetStyle().PlotDefaultSize.y };

		/*!
		@brief The pointer to the data to use for the X coordinate of each new
				point of all lines in this plot.
		*/
		float* ptrX = nullptr;

		/*!
		@brief The maximum number of data points to store for each line.
		@remarks The lines implement a scrolling buffer, so data points will cycle
				 through the buffer of this size.
		*/
		unsigned short linesMaxNbDataPoints = 1024;

		/*!
		@brief All the lines to plot.
		*/
		std::vector<Line> lines;

		/*!
		@brief The flags to use for the plot window.
		*/
		static const ImGuiWindowFlags plotWindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
		
		/*!
		@brief The flags to use for the plot.
		*/
		ImPlotFlags plotFlags = ImPlotFlags_NoLegend | ImPlotFlags_NoInputs;

		/*!
		@brief The flags to use to control the scale to apply on the X axis.
		*/
		ImPlotScale xAxisScale = ImPlotScale_Linear;

		/*!
		@brief The flags to use to control the scale to apply on the Y axis.
		*/
		ImPlotScale yAxisScale = ImPlotScale_Linear;

		/*!
		@brief The flags to use to control the options of the X axis.
		*/
		ImPlotAxisFlags xAxisFlags = ImPlotAxisFlags_AutoFit;

		/*!
		@brief The flags to use to control the options of the Y axis.
		*/
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

		/*!
		@brief Add a new line to the plot.
		*/
		Line& AddLine(std::size_t _matchedDataID) noexcept;

		/*!
		@brief Draws the plot. Calls ImPlot::BeginPlot() and ImPlot::EndPlot().
		*/
		void Draw() noexcept;

		/*!
		@brief Adds points to each line if ::xPtr is not null and depending on the
				update scheme of each line.
		@see Line::Update()
		*/
		void Update() noexcept;
	};
}