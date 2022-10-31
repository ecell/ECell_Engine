#pragma once

#include <vector>

#include "imgui.h"
#include "implot.h"

#include "ASTNode_parser.hpp"
#include "Gillespie_NRM_R.hpp"
#include "SimulationLoop.hpp"
#include "CommandsManager.hpp"
#include "Widget.hpp"

using namespace ECellEngine::Core;
using namespace ECellEngine::IO;

namespace ECellEngine::Editor
{
    // utility structure for realtime plot
    /*
    @brief Utility structure for realtime plot
    @remark Copied from implot_demo.cpp
    @remark Added the ::Reset function.
    */
    struct ScrollingBuffer
    {
        int MaxSize;//The capacity of the buffer.
        int Offset;//Offset of the buffer when adding point after the ::MaxSize has been reached.
        ImVector<ImVec2> Data;//The data points.
        ScrollingBuffer(int max_size = 2000)
        {
            MaxSize = max_size;
            Offset = 0;
            Data.reserve(MaxSize);
        }

        /*
        @brief Adds a point to the buffer.
        */
        void AddPoint(float x, float y)
        {
            if (Data.size() < MaxSize)
                Data.push_back(ImVec2(x, y));
            else
            {
                Data[Offset] = ImVec2(x, y);
                Offset = (Offset + 1) % MaxSize;
            }
        }

        /*
        @brief Erases the buffer.
        */
        void Erase()
        {
            if (Data.size() > 0)
            {
                Data.shrink(0);
                Offset = 0;
            }
        }

        /*
        @brief Erases the buffer and reserve space to ::MaxSize
        */
        void Reset()
        {
            Data.shrink(0);
            Offset = 0;
            Data.reserve(MaxSize);
        }
    };

    /*
    @brief The base struct to store the individual information of instantiated tabs.
    @details When a widget instantiates several tabs that will contain the same 
            graphics but used to visualize different objects (e.g. the tabs in 
            ModelStateVisualizationWidget::lpTabsData), it is necessary to store
            the relevant graphics variables used for ImGui visuals independantly.
            Indeed, using static variables inside the functions to keep track of the
            state of the GUI is not possible anymore if several tabs use the same
            funtion.
    */
    struct TabData
    {
    public:
        char name[32];//The unique name of the tab. It is fed to ImGui::BeginTabItem() as the label.

        /*
        @brief Intializes the value for ::id and ::name.
        */
        TabData(short _id)
        {
            sprintf(name, "Tab %d", _id);
        }
    };

    /*
    @brief A struct to store the parameters and comput the limits of an axis for a line plot.
    */
    struct LinePlotAxisGraphicsData
    {
    public:
        float low, up;//Corresponding limit of the axis.
        float lowTolerance, upTolerance;//In [0,1]. Tells how much we rescale realtively to the value on the axis.
        float axisRescaleLowThreshold, axisRescaleUpThreshold;//In [0,1]. Tells the threshold that triggers a rescale of the axis relatively to the value.
        const char* label;

        LinePlotAxisGraphicsData()
        {

        };

        LinePlotAxisGraphicsData(const char* _label) :
            label(_label)
        {
            low = 0;
            up = 0;
            lowTolerance = 0.5f;
            upTolerance = 0.5f;
            axisRescaleLowThreshold = 0.1f;
            axisRescaleUpThreshold = 0.9f;
        };

        /*
        @brief Computes the value of the low limit defined by ::axisRescaleLowThreshold
        @details Computed as @f$ (up - low) * axisRescaleLowThreshold + low @f$.
        @return The value of the low threshold (see formula).
        */
        inline float getLowThresholdedValue()
        {
            return (up - low) * axisRescaleLowThreshold + low;
        }

        /*
        @brief Computes the value of the up limit defnied by ::axisRescaleLowThreshold
        @details Computed as @f$ (up - low) * axisRescaleLowThreshold + low @f$.
        @return The value of the up threshold (see formula).
        */
        inline float getUpThresholdedValue()
        {
            return (up - low) * axisRescaleUpThreshold + low;
        }

        /*
        @brief Sets the value for ::low using ::lowTolerance relatively to @p _x
        @details Computed as @f$ (1 - lowTolerance) * _x @f$
        @remark Based on the formula, if lowTolerance is set to 0, then the low limit
                of the axis will be equal to @p _x. This is usefull in case we want the
                plot axis scale to follow the value @p _x when the plot goes backward.
                This happens when we reverse a simulation (therefore time goes backward)
                and we are plotting a value against time.
        */
        inline void setLowLimit(float const& _x)
        {
            low = (1-lowTolerance) * _x;
        }
        
        /*
        @brief Sets the value for ::up using ::upTolerance relatively to @p _x
        @details Computed as @f$ (1 + upTolerance) * _x @f$
        @remark Based on the formula, if upTolerance is set to 0, then the up limit
                of the axis will be equal to @p _x. This is usefull in case we want the
                plot axis scale to follow the value @p _x when the plot goes forward.
                This happens when we simulate a model (therefore time goes forward
                and we are plotting a value against time.
        */
        inline void setUpLimit(float const& _x)
        {
            up = (1+upTolerance) * _x;
        }

        /*
        @brief Tests whether @p _x is above the up threshold computed in ::getUpThresholdedValue()
        @return True if @p _x is strictly greater than the up threshold. False, otherwise.
        */
        inline bool isAboveUpThreshold(float const& _x)
        {
            return (_x > (up - low) * axisRescaleUpThreshold + low);
        }

        /*
        @brief Tests whether @p _x is below the low threshold computed in ::getLowThresholdedValue()
        @return True if @p _x is strictly lower than the low threshold. False, otherwise.
        */
        inline bool isBelowLowThreshold(float const& _x)
        {
            return (_x < (up - low) * axisRescaleLowThreshold + low);
        }
    };

    /*
    @brief A struct to store the parameters and local data of each tab that
            should each draw a realtime line plot.
    */
    struct LinePlotTabData : public TabData
    {
    public:
        float x, y;//The coordinate of the last data point.
        bool timeOnX, timeOnY;//Helper boolean indicating whether we are plotting time on the axis and not a species quantity.
        int dataIdxToPlotOnX, dataIdxToPlotOnY;//The index of the species the plot on the axis.
        LinePlotAxisGraphicsData xAxisGraphicsData, yAxisGraphicsData;//The helper struct to manage the axis.
        
        ScrollingBuffer buffer;//The scrolling buffer to store the data points of the plotted line.

        LinePlotTabData(short _id) :
            TabData(_id), xAxisGraphicsData("X Axis"), yAxisGraphicsData("Y Axis")
        {
            sprintf(name, "Line Plot %d", _id);
            x = 0.f;
            y = 0.f;
            timeOnX = false;
            timeOnY = false;
            dataIdxToPlotOnX = 0;
            dataIdxToPlotOnY = 0;
        }

        /*
        @brief Gets the value of LinePlotAxisGraphicsData::low for ::xAxisGraphicsData.
        */
        inline float getXAxisLowLimit()
        {
            return xAxisGraphicsData.low;
        }

        /*
        @brief Gets the value of LinePlotAxisGraphicsData::up for ::xAxisGraphicsData.
        */
        inline float getXAxisUpLimit()
        {
            return xAxisGraphicsData.up;
        }

        /*
        @brief Gets the value of LinePlotAxisGraphicsData::low for ::yAxisGraphicsData.
        */
        inline float getYAxisLowLimit()
        {
            return yAxisGraphicsData.low;
        }

        /*
        @brief Gets the value of LinePlotAxisGraphicsData::up for ::yAxisGraphicsData.
        */
        inline float getYAxisUpLimit()
        {
            return yAxisGraphicsData.up;
        }

        /*
        @brief Sets the value for LinePlotAxisGraphicsData::low and LinePlotAxisGraphicsData::up
                for ::xAxisGraphicsData relatively to ::x.
        */
        inline void setScaleXAxis()
        {
            xAxisGraphicsData.setLowLimit(x);
            xAxisGraphicsData.setUpLimit(x);
        }

        /*
        @brief Sets the value for LinePlotAxisGraphicsData::low and LinePlotAxisGraphicsData::up
                for ::yAxisGraphicsData relatively to ::y.
        */
        inline void setScaleYAxis()
        {
            yAxisGraphicsData.setLowLimit(y);
            yAxisGraphicsData.setUpLimit(y);
        }

        /*
        @brief Adds the point (::x, ::y) to ::buffer.
        */
        inline void bufferAddPoint()
        {
            buffer.AddPoint(x, y);
        }
        
        /*
        @brief Updates the values LinePlotAxisGraphicsData::low & LinePlotAxisGraphicsData::up
                of ::xAxisGraphicsData if needed.
        @details The values are updated only if ::x is beyond the thresholds set in ::xAxisGraphicsData.
        @return True if the values were updated. False, otherwise.
        */
        bool rescaleXAxis()
        {
            if (xAxisGraphicsData.isAboveUpThreshold(x) || xAxisGraphicsData.isBelowLowThreshold(x))
            {
                setScaleXAxis();
                return true;
            }
            return false;
        }

        /*
        @brief Updates the values LinePlotAxisGraphicsData::low & LinePlotAxisGraphicsData::up
                of ::yAxisGraphicsData if needed.
        @details The values are updated only if ::y is beyond the thresholds set in ::yAxisGraphicsData.
        @return True if the values were updated. False, otherwise.
        */
        bool rescaleYAxis()
        {
            if (yAxisGraphicsData.isAboveUpThreshold(y) || yAxisGraphicsData.isBelowLowThreshold(y))
            {
                setScaleYAxis();
                return true;
            }
            return false;
        }
    };

    /*
    @brief The class for the widget to visuals the data stored in the model state.
    @remark There can be several instance of this widget. The instances are managed
            by the ::EngineDataVisualizationWidget.
    */
	class ModelStateVisualizationWidget : public Widget
	{
	private:
        char widgetNameID[32];//The unique name of the instance of the widget.
		SimulationLoop* simulationLoop;//a pointer to the member of the same name in the Engine class.

        const char* visualizationOptions[2] = { "Line Plot", "Quantity Table" };//The names of the tabs that can be instantiated within the widget.
        std::vector<LinePlotTabData> lpTabsData;//The vector of tabs displaying a line plot.
        std::vector<TabData> qtTabsData;//The vector of tabs displaying the table of quantity of species in the model state.

		int nbSpecies;//A buffer integer to know the number of species in the model state without querying every time.
		ASTEvaluator* astEvaluator;//A pointer to the model state
        std::vector<std::string> speciesNamesBuffer;//A buffer vector to know the names of the species in the model state without querying every time.

        /*
        @brief The function used to draw all the widgets of a line plot tab.
        */
		void drawLinePlotTab(LinePlotTabData* _lpTabData);

        /*
        @brief The function used to draw all the widgets of a quantity table.
        */
		void drawQuantityTableTab(TabData* _tabData);

	public:
		ModelStateVisualizationWidget(CommandsManager* _cmdsManager, SimulationLoop* _simulationLoop, short _id) :
			Widget(_cmdsManager), simulationLoop(_simulationLoop)
		{
			astEvaluator = nullptr;
			nbSpecies = 0;
            sprintf(widgetNameID, "Model State Visualization##%d", _id);
		}

		void draw() override;
	};
}