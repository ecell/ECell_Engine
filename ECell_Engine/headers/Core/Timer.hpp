#pragma once

/*
	Defines the interface to manipulate the simulation time
*/
#include <ctime>
#include "Core/Callback.hpp"
#include "system_time.hpp"

namespace ECellEngine::Core
{
	struct Timer
	{
		/*!
		@brief The last delta time to reach the current ::elapsedTime.
		*/
		float deltaTime = DEFAULT_SIMULATION_DELTA_TIME;

		/*!
		@brief System time at the start of the simulation.
		*/
		float startTime = 0.f;

		/*!
		@brief The elapsed time since the start of the simulation.
		*/
		float elapsedTime = 0.f;

		/*!
		@brief The callback whenever the ::deltaTime is updated.
		@details The first parameter is the previous delta time.
				 The second parameter is the new delta time.
		*/
		Core::Callback<const float, const float> onDeltaTimeUpdate;

		/*!
		@brief The callback whenever the ::elapsedTime is updated.
		@details The first parameter is the previous elapsed time since the
				 start of the simulation (::startTime).
				 The second parameter is the new elapsed time since the
				 start of the simulation (::startTime).
		*/
		Core::Callback<const float, const float> onElapsedTimeUpdate;

		/*!
		@brief The callback whenever the ::startTime is updated.
		@details The first parameter is the previous start time.
				 The second parameter is the new start time.
		*/
		Core::Callback<const float, const float> onStartTimeUpdate;

		Timer() = default;

#pragma region Mutators
		/// <summary>
		/// Set the <see cref="startTime"/> public member.
		/// Uses the high resolution current time.
		/// </summary>
		
		/*!
		@brief Sets the ::startTime to the system clock time.
		*/
		inline void SetStartTime()
		{
			startTime = (float)std::clock() / CLOCKS_PER_SEC;
		}
#pragma endregion


#pragma region Logic		
		/*!
		@brief Returns the duration between two times.
		@param _t1 The first time.
		@param _t2 The second time.
		@returns @p _t2 - @p _t1.
		*/
		inline float GetDuration(float& _t1, float& _t2) const
		{
			return _t2 - _t1;
		}

		/*!
		@brief Decrements the ::elapsedTime by the given @p _deltaTime.
		@details  The ::onTimeUpdate callback is called after decrementing
				 the current elapsed time. So, previous value is ::elapsedTime 
				 + ::deltaTime and the new value is ::elapsedTime.
		*/
		inline void Decrement(float _deltaTime);

		/*!
		@brief Increments the ::elapsedTime by the given @p _deltaTime.
		@details  The ::onTimeUpdate callback is called after incrementing
				 the current elapsed time. So, previous value is ::elapsedTime 
				 - ::deltaTime and the new value is ::elapsedTime.
		*/
		void Increment(float _deltaTime);

		/*!
		@brief Returns the time used by the program normalised by the CPU cycles per seconds.
		*/
		inline float ReadHighResTimer() const
		{
			return (float)std::clock() / CLOCKS_PER_SEC;
		}
		
		/*!
		@brief Resets the ::startTime, ::elapsedTime and ::deltaTime to their
			   default values.
		*/
		void ResetTimes();
#pragma endregion
	};
}