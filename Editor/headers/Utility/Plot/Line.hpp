#pragma once

#include <cstring>

#include "Core/Callback.hpp"
#include "Utility/Plot/UpdateStrategy.hpp"
#include "Utility/Plot/ScrollingBuffer.hpp"

//forward declaration of NodeId
namespace ax::NodeEditor
{
	struct NodeId;
}

namespace ECellEngine::Editor::Utility::Plot
{
	/*!
	@brief A line to plot.
	@see ECellEngine::Editor::Utility::Plot::LinePlot, ECellEngine::Editor::Utility::Plot::LinePlot::lines,
		 ECellEngine::Editor::Utility::MNBV::LinePlotNodeData
	*/
	struct Line
	{
	public:
		/*!
		@brief The id of the data this line is matching.
		@details It is not used within this class.
				 It is used to be able to use lower_bound (binary search) on the
				 vector of lines.
		*/
		std::size_t id;

		/*!
		@brief The placeholder for the subscription of ::UpdateLine to a matching
				callback.
		*/
		std::shared_ptr<std::function<void(const float, const float)>> updateLineSubToken = nullptr;

		/*
		@brief Pointer to the update controller to be used to control adding points
				to this line.
		*/
		std::shared_ptr<UpdateStrategy> updateStrategy;

		/*!
		@brief The buffer containing the data points of the line.
		*/
		ScrollingBuffer dataPoints;

		/*!
		@brief The pointer to the data to use for the X coordinate of each new
				point of this line.
		*/
		float* ptrX = nullptr;

		/*!
		@brief The buffer to the Y coordinate of each new point of this line.
		*/
		float Y = 0.f;

		/*!
		@brief The color of the line.
		*/
		ImVec4 color{ 1.f, 1.f, 1.f, 1.f };

		/*!
		@brief The legend of the line.
		*/
		char lineLegend[64] = "f(x)";

		Line(std::size_t _matchingDataId, unsigned short _maxNbDataPoints, float* _ptrX) :
			id{ _matchingDataId }, ptrX{ _ptrX }
		{
			dataPoints.Data.reserve(_maxNbDataPoints);
			dataPoints.AddPoint(0.f, 0.f);

			updateStrategy = std::shared_ptr<UpdateStrategy>(new UpdateStrategyNever());
		}

		/*!
		@brief Conversion to std::size_t.
		@details Defined to be able to use lower_bound (binary search) on the
				 vector of lines.
		*/
		operator std::size_t();

		/*!
		@brief "Less than" operator.
		@details Defined to be able to use lower_bound (binary search) on the
				 vector of lines.
		*/
		friend inline bool operator< (const Line& lhs, const Line& rhs)
		{
			return lhs.id < rhs.id;
		}

		/*!
		@brief "Less than" operator.
		@details Defined to be able to use lower_bound (binary search) on the
				 vector of lines.
		*/
		friend inline bool operator< (const std::shared_ptr<Line> lhs, const std::size_t _id)
		{
			return lhs->id < _id;
		}

		/*!
		@brief Translates the update scheme of ::updateStrategy into a string.
		*/
		inline const char* GetCurrentUpdateSchemeName() noexcept
		{
			return updateStrategy->GetName();
		}

		/*!
		@brief Uses ImPlot API to draw the line.
		@remarks It expects to be called between ImPlot::BeginPlot() and
				 ImPlot::EndPlot(). No check is done to ensure that.
		*/
		void Draw();

		/*!
		@brief Updates the pointer ::updateStrategy to point to the
			   corresponding update strategy.
		@see ::updateStrategy_Always, ::updateStrategy_EveryNthFrame,
			 ::updateStrategy_EveryXSeconds, ::updateStrategy_Never,
			 ::updateStrategy_OnChange
		*/
		void SwitchUpdateStrategy(unsigned char _updateScheme) noexcept;

		/*!
		@biref Decides to add a new point to ::dataPoints or not based on
			   the ::updateStrategy.
		*/
		void UpdateLine(const float _previousValue, const float _newValue) noexcept;

	};
}