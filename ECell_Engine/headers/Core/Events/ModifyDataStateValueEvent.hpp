#pragma once

#include <string>

#include "Core/Events/Event.hpp"

namespace ECellEngine::Core::Events
{
	/*!
	@brief Class used to execute arbitrary code (callback).
	*/
	class ModifyDataStateValueEvent : public Event
	{
	public:
		enum class DataStateValueType
		{
			Species,
			Parameter
		};

	private:
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
		float newValue;

	public:
		virtual void Execute(ECellEngine::Core::SimulationsManager& simulationsManager, std::size_t targetSimulationIndex) noexcept override;
	};
}