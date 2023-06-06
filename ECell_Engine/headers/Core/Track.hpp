#pragma once

#include <vector>
#include <memory>	//shared_ptr

#include "Core/Events/Event.hpp"

namespace ECellEngine::Core
{
	class Track
	{
		public:
			struct TimestampedEvent
			{
				unsigned int id;
				float timestamp;
				std::shared_ptr<Events::Event> event;

				TimestampedEvent(unsigned int _id, float _timestamp, std::shared_ptr<Events::Event> _event):
					id{_id},
					timestamp{_timestamp},
					event(_event)
				{}
			};

		private:
			std::vector<TimestampedEvent> events;

			float currentTimestamp;
			float lastEventCallTimestamp = -1.0f;

			//TODO: Id must be saved
			unsigned int nextGeneratedId = 0u;

		public:
			void SetCurrentTimestamp(float time, bool callIntermediateEvents) noexcept;

			void AdvanceTime(float deltaTime, bool callIntermediateEvents) noexcept;

			void AddEvent(float timestamp, std::shared_ptr<Events::Event>& toAddEvent, bool executeImmediately) noexcept;

			void Reset() noexcept;

			/*!
			@brief Get the timestamp of the first event in the provided interval.

			@param startTimestamp Interval lower bound (included).
			@param endTimestamp Interval greater bound (excluded).
			@param out_timestamp Timestamp of the first found event if any was found in the interval.

			@return true if an event was found (and out_timestamp filled), else false.
			*/
			bool GetFirstEventTimestampInInterval(float startTimestamp, float endTimestamp, float& out_timestamp) const noexcept;

			float GetCurrentTimestamp() const noexcept;
	};
}