#pragma once

#include <memory>	//std::shared_ptr
#include <vector>

#include "Core/Events/Event.hpp"

namespace ECellEngine::Core
{
	class Watcher
	{
	public:
		enum class Comparator
		{
			Greater,
			GreaterOrEqual,
			Equal,
			NotEqual,
			Less,
			LessOrEqual
		};

	private:
		/*!
		@brief Collection of events that should be triggered when the condition held by this watcher is fulfilled.
		*/
		std::vector<std::shared_ptr<Events::Event>> triggeredEvents;

		/*!
		@brief Left value.
		*/
		float lhs;

		/*!
		@brief Type of operator to use between lhs and rhs.
		*/
		Comparator comparator;

		/*!
		@brief Right value.
		*/
		float rhs;

	public:
		/*!
		@brief Checks whether or not the lhs comparator rhs condition is true or not.
		*/
		bool IsConditionVerified() noexcept;

		/*!
		@brief Call the internal events if the lhs comparator rhs condition is true.
		*/
		void CallEventsIfConditionVerified() noexcept;

		/*!
		@brief Add an event to the list of internal events for this watcher.
		*/
		void AddEvent(std::shared_ptr<Events::Event> event) noexcept;

		/*!
		@brief Remove an event from the list of internal events for this watcher.

		@return true if an event was removed, else false.
		*/
		bool RemoveEvent(std::shared_ptr<Events::Event> event) noexcept;

		/*!
		@brief Get the list of all events contained in this watcher.

		@return The list of all events contained in this watcher.
		*/
		std::vector<std::shared_ptr<Events::Event>> const& GetEvents() const noexcept;
	};
}