#include "Core/Track.hpp"

#include <algorithm>

void ECellEngine::Core::Track::SetCurrentTimestamp(float time, bool callIntermediateEvents) noexcept
{
	//TODO
}

void ECellEngine::Core::Track::AdvanceTime(float deltaTime, bool callIntermediateEvents) noexcept
{
	//TODO
}

void ECellEngine::Core::Track::AddEvent(float timestamp, std::shared_ptr<Events::Event>& toAddEvent, bool executeImmediately) noexcept
{
	auto it = std::find_if(events.begin(), events.end(), [timestamp](TimestampedEvent const& timestampedEvent)
						   {
							   return timestamp > timestampedEvent.timestamp;
						   }
						);

	events.emplace(it, nextGeneratedId++, timestamp, toAddEvent);
}

void ECellEngine::Core::Track::Reset() noexcept
{
	currentTimestamp = 0.0f;
	lastEventCallTimestamp = -1.0f;
}

bool ECellEngine::Core::Track::GetFirstEventTimestampInInterval(float startTimestamp, float endTimestamp, float& out_timestamp) const noexcept
{
	//TODO
	return true;
}

float ECellEngine::Core::Track::GetCurrentTimestamp() const noexcept
{
	return currentTimestamp;
}