#pragma once

#include <functional>
#include <memory>
#include <string>

#include "Core/Events/Event.hpp"

namespace ECellEngine::Core::Events
{
	/*!
	@brief An event to modify a value of in an instance of ECellEngine::Data::DataState (callback).
	*/
	class ModifyDataStateValueEvent : public Event
	{
	public:
		enum class DataStateValueType
		{
			Species,
			Parameter
		};
		
		/*!
		@brief ID of the value this event will modify (species or parameter).
		*/
		std::size_t dataStateValueId = SIZE_MAX;

		/*!
		@brief The condition controlling the execution of this event.
		*/
		bool condition = false;

		/*!
		@brief Value to assign to the data state.
		*/
		float value = 0.f;

	public:
		/*!
		@brief Type of the dataStateValueId field.
		*/
		DataStateValueType valueType;

		std::shared_ptr<std::function<void(bool, bool)>> conditionCallbackToken = nullptr;
		std::shared_ptr<std::function<void(float, float)>> valueCallbackToken = nullptr;

		ModifyDataStateValueEvent(const std::size_t _id) :
			Event(_id), valueType(DataStateValueType::Species)
		{

		}

		inline bool GetCondition() const noexcept
		{
			return condition;
		}

		inline float GetValue() const noexcept
		{
			return value;
		}

		virtual void Execute(std::size_t targetSimulationIndex) noexcept override;

		void UpdateCondition(const bool _previousValue, const bool _newValue) noexcept;

		void UpdateDataStateValueId(const std::size_t _previousValue, const std::size_t _newValue) noexcept;

		void UpdateValue(const float _previousValue, const float _newValue) noexcept;
	};
}