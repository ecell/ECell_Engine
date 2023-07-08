#pragma once

#include <cassert>
#include <algorithm> //std::find_if
#include <memory>	//std::shared_ptr
#include <vector>

//Simulation Manager will be required in the future when we
// will have several simulations running at the same time
// in order to pass the index of the simulation where the event
// should be executed. But it creates a circular dependency
// TODO: Find a way to remove this circular dependency
//#include "Core/SimulationsManager.hpp"

#include "Core/Events/Event.hpp"

namespace ECellEngine::Core
{
	/*!
	@brief Class that can be used to watch a condition and trigger events when
			this condition is fulfilled.
	@details The target and threshold must implement all binary operators used
			 in the comparison. Namely: >, >=, ==, !=, <, <=.
	@tparam TargetType Type of the object being watched. Must be a pointer.
	@tparam ThresholdType Type of the object to which the target is being compared.
	*/
	template <typename TargetType, typename ThresholdType>
	class Trigger
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

		bool triggered = false;

		/*!
		@brief Pointer to the value being watched. Always on the left hand side
				in the comparison.
		*/
		TargetType target;

		/*!
		@brief Type of operator to use between target and threshold.
		*/
		Comparator comparator;

		/*!
		@brief Pointer to the value to which ::target is being compared. Always 
				the right hand side in the comparison.
		*/
		ThresholdType threshold;

	public:
		Trigger() :
			target(nullptr), comparator(Comparator::Greater), threshold(nullptr)
		{
			
		}

		/*!
		@brief Gets the reference to the value of the comparator used between target and threshold.
		*/
		inline Comparator& GetComparator() noexcept
		{
			return comparator;
		}

		/*!
		@brief Gets the pointer (because TargetType MUST be a pointer) to the value
				under surveillance.
		*/
		inline TargetType GetTarget() const noexcept
		{
			return target;
		}

		/*!
		@brief Gets the pointer (because ThresholdType MUST be a pointer) to the value
				to which the ::target is compared.
		*/
		inline ThresholdType GetThreshold() const noexcept
		{
			return threshold;
		}

		/*!
		@brief Sets the value of the comparator to use between target and threshold.
		*/
		inline void SetComparator(const Comparator _comparator) noexcept
		{
			comparator = _comparator;
		}

		/*!
		@brief Sets the pointer to the value being watched. It is the left hand side
				in the comparison.
		*/
		inline void SetTarget(TargetType _target) noexcept
		{
			target = _target;
		}

		/*!
		@brief Sets the pointer to the value to which the ::target is being compared.
				It is the right hand side in the comparison.
		*/
		inline void SetThreshold(ThresholdType _threshold) noexcept
		{
			threshold = _threshold;
		}

		/*!
		@brief Checks whether or not the target comparator threshold condition is true or not.
		*/
		bool IsConditionVerified() noexcept
		{
			switch (comparator)
			{
			case Comparator::Greater:
				return *target > *threshold;

			case Comparator::GreaterOrEqual:
				return *target >= *threshold;

			case Comparator::Equal:
				return *target == *threshold;

			case Comparator::NotEqual:
				return *target != *threshold;

			case Comparator::Less:
				return *target < *threshold;

			case Comparator::LessOrEqual:
				return *target <= *threshold;

			default:
				//Should never reach this point
				assert(false);
				return false;
			}
		}

		/*!
		@brief Checks whether the condition held by this watcher transitioned
				from false to true.
		*/
		bool IsConditionNewlyVerified() noexcept
		{
			if (!triggered)
			{
				return triggered = IsConditionVerified();
			}
			else
			{
				if(!IsConditionVerified())
				{
					triggered = false;
				}
				return false;
			}
		}

		/*!
		@brief Call the internal events for this watcher.
		*/
		void CallEvents() noexcept
		{
			//Call events
			for (std::shared_ptr<Events::Event>& event : triggeredEvents)
			{
				event->Execute(0);//TODO: Change this to use the index of the simulation where that event is coming from
			}
		}

		/*!
		@brief Call the internal events if the target comparator threshold condition is true.
		*/
		//void CallEventsIfConditionVerified() noexcept
		//{
		//	if (IsConditionVerified())
		//	{
		//		//Call events
		//		for (std::shared_ptr<Events::Event>& event : triggeredEvents)
		//		{
		//			event->Execute(0);//TODO: Change this to use the index of the simulation where that event is coming from
		//		}
		//	}
		//}

		/*!
		@brief Add an event to the list of internal events for this watcher.
		*/
		void AddEvent(std::shared_ptr<Events::Event> _event) noexcept
		{
			triggeredEvents.push_back(_event);
		}

		/*!
		@brief Remove an event from the list of internal events for this watcher.

		@return true if an event was removed, else false.
		*/
		bool RemoveEvent(std::shared_ptr<Events::Event> _event) noexcept
		{
			auto it = std::find_if(triggeredEvents.begin(), triggeredEvents.end(), [_event](std::shared_ptr<Events::Event> const& e) { return _event == e; });

			if (it != triggeredEvents.end())
			{
				triggeredEvents.erase(it);
				return true;
			}
			else
			{
				return false;
			}
		}

		/*!
		@brief Get the list of all events contained in this watcher.

		@return The list of all events contained in this watcher.
		*/
		std::vector<std::shared_ptr<Events::Event>> const& GetEvents() const noexcept
		{
			return triggeredEvents;
		}
	};
}