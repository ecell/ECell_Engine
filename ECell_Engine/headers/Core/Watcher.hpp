#pragma once

#include <memory>	//std::shared_ptr
#include <vector>

#include "Core/Events/Event.hpp"

namespace ECellEngine::Core
{
	/*!
	@brief Class that can be used to watch a condition and trigger events when
			this condition is fulfilled.
	*/
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
		@brief Collection of events that should be triggered when the condition
				held by this watcher is fulfilled.
		*/
		std::vector<std::shared_ptr<Events::Event>> triggeredEvents;

		/*!
		@brief Pointer to the value to use as the left hand side in the comparison.
		*/
		float* lhs;

		/*!
		@brief Type of operator to use between lhs and rhs.
		*/
		Comparator comparator;

		/*!
		@brief Pointer to the value to use as the right hand side in the comparison.
		*/
		float* rhs;

	public:
		Watcher() :
			lhs(nullptr), comparator(Comparator::Greater), rhs(nullptr)
		{
			
		}

		/*!
		@brief Gets the reference to the value of the comparator used between lhs and rhs.
		*/
		inline Comparator& GetComparator() noexcept
		{
			return comparator;
		}

		/*!
		@brief Gets the pointer to the value of the left hand side of the comparison.
		*/
		inline float* GetLHS() const noexcept
		{
			return lhs;
		}

		/*!
		@brief Gets the pointer to the value of the right hand side of the comparison.
		*/
		inline float* GetRHS() const noexcept
		{
			return rhs;
		}

		/*!
		@brief Sets the value of the comparator to use between lhs and rhs.
		*/
		inline void SetComparator(const Comparator _comparator) noexcept
		{
			comparator = _comparator;
		}

		/*!
		@brief Sets the pointer to the value of the left hand side of the comparison.
		*/
		inline void SetLHS(float* _lhs) noexcept
		{
			lhs = _lhs;
		}

		/*!
		@brief Sets the pointer to the value of the right hand side of the comparison.
		*/
		inline void SetRHS(float* _rhs) noexcept
		{
			rhs = _rhs;
		}

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
		void AddEvent(std::shared_ptr<Events::Event> _event) noexcept;

		/*!
		@brief Remove an event from the list of internal events for this watcher.

		@return true if an event was removed, else false.
		*/
		bool RemoveEvent(std::shared_ptr<Events::Event> _event) noexcept;

		/*!
		@brief Get the list of all events contained in this watcher.

		@return The list of all events contained in this watcher.
		*/
		std::vector<std::shared_ptr<Events::Event>> const& GetEvents() const noexcept;
	};
}