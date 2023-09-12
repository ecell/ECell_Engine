#pragma once 

#include "implot.h"

#include "Core/Callback.hpp"
#include "Util/StrUtil.hpp"
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
		@brief The buffer to the data to use for the X coordinate of each new
				point of all lines in this plot.
		*/
		float X = 0.f;

		std::shared_ptr<std::function<void(const float, const float)>> updateXSubToken = nullptr;

		/*!
		@brief The maximum number of data points to store for each line.
		@remarks The lines implement a scrolling buffer, so data points will cycle
				 through the buffer of this size.
		*/
		unsigned short linesMaxNbDataPoints = 1024;

		/*!
		@brief All the lines to plot.
		*/
		std::vector<std::shared_ptr<Line>> lines;

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

		/*!
		@brief Gets the name of this type of plot.
		*/
		inline char* GetName() noexcept
		{
			return name;
		}

		/*!
		@brief Sets the name of this type of plot.
		*/
		inline void SetName(const char* _name) noexcept
		{
			Util::StrCopy(name, _name, sizeof(name));
		}

		/*!
		@brief Add a new line to the plot.
		*/
		std::shared_ptr<Line> AddLine(std::size_t _matchedDataID) noexcept;

		/*!
		@brief Draws the plot. Calls ImPlot::BeginPlot() and ImPlot::EndPlot().
		*/
		void Draw() noexcept;

		/*!
		@brief Updates the X coordinate of this plot.
		*/
		void UpdateX(const float _previousValue, const float _newValue);
	};
}