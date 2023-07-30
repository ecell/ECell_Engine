#pragma once

#include <cstring>

#include "Core/Callback.hpp"
#include "Utility/Plot/UpdateController.hpp"
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
	protected:
		/*!
		@brief An instance of the update controller for this lines that never
				accepts new points.
		*/
		UpdateController_Never updateControllerNever;

		/*!
		@brief An instance of the update controller for this lines that always
				accepts new points.
		*/
		UpdateController_Always updateControllerAlways;

		/*!
		@brief An instance of the update controller for this lines that accepts
				new points only when the value pointed at ::ptrY changes.
		*/
		UpdateController_OnChange updateControllerOnChange;

		/*!
		@brief An instance of the update controller for this lines that accepts
				new points every N frame.
		*/
		UpdateController_EveryNthFrame updateControllerEveryNthFrame;

		/*!
		@brief An instance of the update controller for this lines that accepts
				new points every X seconds.
		*/
		UpdateController_EveryXSeconds updateControllerEveryXSeconds;

	public:
		/*!
		@brief The id of the data this line is matching.
		@details It is not used within this class.
				 It is used to be able to use lower_bound (binary search) on the
				 vector of lines.
		*/
		std::size_t id;

		/*!
		@brief The pointer to the callback that will trigger the update of this
				line.
		@details It is necessary to keep it because Lines are stored in a vector
				 and the pointer to ::UpdateLine will be invalidated within the
				 callback if this Line is copied/moved because of vector reallocation.
		*/
		std::shared_ptr<Core::Callback<const float, const float>*> updateLineCallback = nullptr;

		/*!
		@brief The placeholder for the subscription of ::UpdateLine to a matching
				callback.
		*/
		std::shared_ptr<std::function<void(const float, const float)>> updateLineSubToken = nullptr;

		/*
		@brief Pointer to the update controller to be used to control adding points
				to this line.	
		*/
		UpdateController* updateController = &updateControllerAlways;
		
		/*!
		@brief An array of strings to display in the combo box to select the update
				scheme.
		*/
		char* updateSchemeNames[5] { "Never", "Always", "OnChange", "EveryNthFrame", "EveryXSeconds" };

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
		}

		/*!
		@brief Custom copy constructor.
		@details It is used to copy the callback and the subscription token.
		*/
		Line(const Line& _other) :
			id(_other.id), dataPoints(_other.dataPoints),
			ptrX(_other.ptrX), Y(_other.Y), color(_other.color),
			updateControllerNever(_other.updateControllerNever),
			updateControllerAlways(_other.updateControllerAlways),
			updateControllerOnChange(_other.updateControllerOnChange),
			updateControllerEveryNthFrame(_other.updateControllerEveryNthFrame),
			updateControllerEveryXSeconds(_other.updateControllerEveryXSeconds)
		{
			if (_other.updateLineSubToken != nullptr)
			{
				**(_other.updateLineCallback.get()) -= _other.updateLineSubToken;
				updateLineCallback = _other.updateLineCallback;
				updateLineSubToken = std::move( **(updateLineCallback.get()) += std::bind(&Line::UpdateLine, this, std::placeholders::_1, std::placeholders::_2) );
			}
			SwitchUpdateController(_other.updateController->scheme);
			std::memcpy(lineLegend, _other.lineLegend, 64);
		}

		/*!
		@brief Custom move constructor.
		@details It is used to move the callback and the subscription token.
		*/
		Line(const Line&& _other) noexcept :
			id(_other.id), dataPoints(_other.dataPoints),
			ptrX(_other.ptrX), Y(_other.Y), color(_other.color),
			updateControllerNever(_other.updateControllerNever),
			updateControllerAlways(_other.updateControllerAlways),
			updateControllerOnChange(_other.updateControllerOnChange),
			updateControllerEveryNthFrame(_other.updateControllerEveryNthFrame),
			updateControllerEveryXSeconds(_other.updateControllerEveryXSeconds)
		{
			if (_other.updateLineSubToken != nullptr)
			{
				**(_other.updateLineCallback.get()) -= _other.updateLineSubToken;
				updateLineCallback = _other.updateLineCallback;
				updateLineSubToken = std::move(**(updateLineCallback.get()) += std::bind(&Line::UpdateLine, this, std::placeholders::_1, std::placeholders::_2));
			}
			SwitchUpdateController(_other.updateController->scheme);
			std::memcpy(lineLegend, _other.lineLegend, 64);
		}

		/*!
		@brief Custom copy assignment operator.
		@details It is used to copy the callback and the subscription token.
		*/
		Line& operator=(Line& _other)
		{
			if (*this == _other)
				return *this;

			id = _other.id;
			dataPoints = _other.dataPoints;
			ptrX = _other.ptrX;
			Y = _other.Y;
			color = _other.color;
			updateControllerNever = _other.updateControllerNever;
			updateControllerAlways = _other.updateControllerAlways;
			updateControllerOnChange = _other.updateControllerOnChange;
			updateControllerEveryNthFrame = _other.updateControllerEveryNthFrame;
			updateControllerEveryXSeconds = _other.updateControllerEveryXSeconds;
			if (_other.updateLineSubToken != nullptr)
			{
				**(_other.updateLineCallback.get()) -= _other.updateLineSubToken;
				updateLineCallback = _other.updateLineCallback;
				updateLineSubToken = std::move(**(updateLineCallback.get()) += std::bind(&Line::UpdateLine, this, std::placeholders::_1, std::placeholders::_2));
			}
			SwitchUpdateController(_other.updateController->scheme);
			std::memcpy(lineLegend, _other.lineLegend, 64);
			return *this;
		}

		/*!
		@brief Custom move assignment operator.
		@details It is used to move the callback and the subscription token.
		*/
		Line& operator=(Line&& _other) noexcept
		{
			if(*this == _other)
				return *this;

			id = _other.id;
			dataPoints = _other.dataPoints;
			ptrX = _other.ptrX;
			Y = _other.Y;
			color = _other.color;
			updateControllerNever = _other.updateControllerNever;
			updateControllerAlways = _other.updateControllerAlways;
			updateControllerOnChange = _other.updateControllerOnChange;
			updateControllerEveryNthFrame = _other.updateControllerEveryNthFrame;
			updateControllerEveryXSeconds = _other.updateControllerEveryXSeconds;
			if (_other.updateLineSubToken != nullptr)
			{
				**(_other.updateLineCallback.get()) -= _other.updateLineSubToken;
				updateLineCallback = _other.updateLineCallback;
				updateLineSubToken = std::move(**(updateLineCallback.get()) += std::bind(&Line::UpdateLine, this, std::placeholders::_1, std::placeholders::_2));
			}
			SwitchUpdateController(_other.updateController->scheme);
			std::memcpy(lineLegend, _other.lineLegend, 64);
			return *this;
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
		@brief Translates the update scheme of ::updateController into a string.
		*/
		inline const char* GetCurrentUpdateSchemeName() noexcept
		{
			return updateSchemeNames[updateController->scheme];
		}

		/*!
		@brief Gets the string corresponding to a given update scheme.
		@param _updateScheme The integer matching to a ECellEngine::Editor::Utility::Plot::UpdateScheme.
		*/
		inline const char* GetUpdateSchemeName(unsigned short _updateScheme) noexcept
		{
			return updateSchemeNames[_updateScheme];
		}

		/*!
		@brief Uses ImPlot API to draw the line.
		@remarks It expects to be called between ImPlot::BeginPlot() and
				 ImPlot::EndPlot(). No check is done to ensure that.
		*/
		void Draw();

		/*!
		@brief Updates the pointer ::updateController to point to the
			   corresponding update controller.
		@see ::updateController_Always, ::updateController_EveryNthFrame,
			 ::updateController_EveryXSeconds, ::updateController_Never,
			 ::updateController_OnChange
		*/
		void SwitchUpdateController(unsigned short _updateScheme) noexcept;

		/*!
		@biref Decides to add a new point to ::dataPoints or not based on
			   the ::updateController.
		*/
		void UpdateLine(const float _previousValue, const float _newValue) noexcept;

	};
}