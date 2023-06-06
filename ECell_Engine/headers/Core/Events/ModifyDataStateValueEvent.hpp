#pragma once

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

	public:
		/*!
		@brief Type of the dataStateValueId field.
		*/
		DataStateValueType valueType;

		/*!
		@brief Name of the value this event will modify (species or parameter).
		*/
		std::string dataStateValueId;

		/*!
		@brief Value to attribute.
		*/
		float* newValue;

		ModifyDataStateValueEvent() :
			Event(), valueType(DataStateValueType::Species), dataStateValueId(""), newValue(nullptr)
		{

		}

		virtual void Execute(std::size_t targetSimulationIndex) noexcept override;
	};
}