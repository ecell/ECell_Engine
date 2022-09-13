#pragma once

#include <vector>

#include "imgui.h"
#include "implot.h"

#include "ASTNode_parser.hpp"
#include "Gillespie_NRM_R.hpp"
#include "SimulationLoop.hpp"
#include "CommandsManager.hpp"
#include "Widget.hpp"

namespace ECellEngine::Editor
{
    // utility structure for realtime plot
    struct RollingBuffer
    {
        float Span;
        ImVector<ImVec2> Data;
        RollingBuffer()
        {
            Span = 10.0f;
            Data.reserve(2000);
        }
        void AddPoint(float x, float y)
        {
            float xmod = fmodf(x, Span);
            if (!Data.empty() && xmod < Data.back().x)
                Data.shrink(0);
            Data.push_back(ImVec2(xmod, y));
        }
    };

    // utility structure for realtime plot
    struct ScrollingBuffer
    {
        int MaxSize;
        int Offset;
        ImVector<ImVec2> Data;
        ScrollingBuffer(int max_size = 2000)
        {
            MaxSize = max_size;
            Offset = 0;
            Data.reserve(MaxSize);
        }
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
        void Erase()
        {
            if (Data.size() > 0)
            {
                Data.shrink(0);
                Offset = 0;
            }
        }
        void Reset()
        {
            Data.shrink(0);
            Offset = 0;
            Data.reserve(MaxSize);
        }
    };

    struct TabData
    {
    private :
        short id;
    public:
        char name[32];

        TabData(short _id) : id(_id)
        {
            sprintf(name, "Tab %d", _id);
        }
    };

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
            lowTolerance = 0.5f;
            upTolerance = 0.5f;
            axisRescaleLowThreshold = 0.1f;
            axisRescaleUpThreshold = 0.9f;
        };

        inline float getLowLimit()
        {
            return low;
        }

        inline float getLowThresholdedValue()
        {
            return (up - low) * axisRescaleLowThreshold + low;
        }

        inline float getUpLimit()
        {
            return up;
        }

        inline float getUpThresholdedValue()
        {
            return (up - low) * axisRescaleUpThreshold + low;
        }

        inline void setLowLimit(float const& _x)
        {
            low = (1-lowTolerance) * _x;
        }
        
        inline void setUpLimit(float const& _x)
        {
            up = (1+upTolerance) * _x;
        }

        inline bool isAboveUpThreshold(float const& _x)
        {
            return (_x > (up - low) * axisRescaleUpThreshold + low);
        }

        inline bool isBelowLowThreshold(float const& _x)
        {
            return (_x < (up - low)* axisRescaleLowThreshold + low);
        }
    };

    struct LinePlotTabData : public TabData
    {
    public:
        float x, y;
        bool timeOnX, timeOnY;
        int dataIdxToPlotOnX, dataIdxToPlotOnY;
        LinePlotAxisGraphicsData xAxisGraphicsData;
        LinePlotAxisGraphicsData yAxisGraphicsData;
        
        ScrollingBuffer buffer;

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

        inline float getXAxisLowLimit()
        {
            return xAxisGraphicsData.getLowLimit();
        }

        inline float getXAxisUpLimit()
        {
            return xAxisGraphicsData.getUpLimit();
        }

        inline float getYAxisLowLimit()
        {
            return yAxisGraphicsData.getLowLimit();
        }

        inline float getYAxisUpLimit()
        {
            return yAxisGraphicsData.getUpLimit();
        }

        inline void setScaleXAxis()
        {
            xAxisGraphicsData.setLowLimit(x);
            xAxisGraphicsData.setUpLimit(x);
        }

        inline void setScaleYAxis()
        {
            yAxisGraphicsData.setLowLimit(y);
            yAxisGraphicsData.setUpLimit(y);
        }

        inline void bufferAddPoint()
        {
            buffer.AddPoint(x, y);
        }
        
        bool rescaleXAxis()
        {
            if (xAxisGraphicsData.isAboveUpThreshold(x) || xAxisGraphicsData.isBelowLowThreshold(x))
            {
                setScaleXAxis();
                return true;
            }
            return false;
        }

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

	class ModelStateVisualizationWidget : public Widget
	{
	private:
        char widgetNameID[32];
		SimulationLoop* simulationLoop;//a pointer to the member of the same name in the Engine class.

        const char* visualizationOptions[3] = { "Line Plot", "Quantity Table" };
        std::vector<LinePlotTabData> lpTabsData;
        std::vector<TabData> qtTabsData;

		int nbSpecies;
		ASTEvaluator* astEvaluator;
        std::vector<std::string> speciesNamesBuffer;

		void drawLinePlotTab(LinePlotTabData* _lpTabData);

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