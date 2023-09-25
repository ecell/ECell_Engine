#pragma once

#include <cstdint>

namespace ECellEngine::Core
{
	namespace Events
	{
		/*!
		@brief Class used to execute arbitrary code (callback).
		*/
		class Event
		{
		private:
			/*!
			@brief ID of the event.
			*/
			std::size_t id = SIZE_MAX;

		public:
			Event() = default;

			Event(const std::size_t _id) :
				id(_id)
			{

			}

			virtual ~Event() = default;

			/*!
			@brief Operator to compare two events based on their ID.
			*/
			bool operator<(const Event& _rhs) const noexcept
			{
				return id < _rhs.id;
			}

			/*!
			@brief Operator to compare an event with an ID.
			*/
			bool operator<(const std::size_t _id) const noexcept
			{
				return id < _id;
			}

			/*!
			@brief Get the ID of the event.
			*/
			inline std::size_t GetID() const noexcept
			{
				return id;
			}

			/*!
			@brief Execute the callback.

			@param _targetSimulationIndex Index of the simulation containing this event in the simulation manager.
			*/
			virtual void Execute(std::size_t _targetSimulationIndex) noexcept = 0;
		};
	}
}