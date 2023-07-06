//#include <cassert>
//#include <algorithm> //std::find_if
//
//#include "Core/SimulationsManager.hpp"
//#include "Core/Watcher.hpp"
//
//template <typename TargetType, typename ThresholdType>
//bool ECellEngine::Core::Watcher<TargetType, ThresholdType>::IsConditionVerified() noexcept
//{
//	switch (comparator)
//	{
//	case Comparator::Greater:
//		return *lhs > *rhs;
//
//	case Comparator::GreaterOrEqual:
//		return *lhs >= *rhs;
//
//	case Comparator::Equal:
//		return *lhs == *rhs;
//
//	case Comparator::NotEqual:
//		return *lhs != *rhs;
//
//	case Comparator::Less:
//		return *lhs < *rhs;
//
//	case Comparator::LessOrEqual:
//		return *lhs <= *rhs;
//
//	default:
//		//Should never reach this point
//		assert(false);
//		return false;
//	}
//}
//
//template <typename TargetType, typename ThresholdType>
//void ECellEngine::Core::Watcher<TargetType, ThresholdType>::CallEvents() noexcept
//{
//	//Call events
//	for (std::shared_ptr<Events::Event>& event : triggeredEvents)
//	{
//		event->Execute(0);//TODO: Change this to use the index of the simulation where that event is coming from
//	}
//}
//
//template <typename TargetType, typename ThresholdType>
//void ECellEngine::Core::Watcher<TargetType, ThresholdType>::CallEventsIfConditionVerified() noexcept
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
//
//template <typename TargetType, typename ThresholdType>
//void ECellEngine::Core::Watcher<TargetType, ThresholdType>::AddEvent(std::shared_ptr<Events::Event> event) noexcept
//{
//	triggeredEvents.push_back(event);
//}
//
//template <typename TargetType, typename ThresholdType>
//bool ECellEngine::Core::Watcher<TargetType, ThresholdType>::RemoveEvent(std::shared_ptr<Events::Event> event) noexcept
//{
//	auto it = std::find_if(triggeredEvents.begin(), triggeredEvents.end(), [event](std::shared_ptr<Events::Event> const& e) { return event == e; });
//
//	if (it != triggeredEvents.end())
//	{
//		triggeredEvents.erase(it);
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//template <typename TargetType, typename ThresholdType>
//std::vector<std::shared_ptr<ECellEngine::Core::Events::Event>> const& ECellEngine::Core::Watcher<TargetType, ThresholdType>::GetEvents() const noexcept
//{
//	return triggeredEvents;
//}