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
		@brief The last delata time to reach the current ::elapsedTime.
		*/
		float deltaTime;

		/*!
		@brief System time at the start of the simulation.
		*/
		float startTime;

		/*!
		@brief The elapsed time since the start of the simulation.
		*/
		float elapsedTime;

		/*!
		@brief The callback whenever the simulation time is updated.
		@details The first parameter is the previous elapsed time since the
				 start of the simulation.
				 The second parameter is the new elapsed time since the
				 start of the simulation.
		*/
		Core::Callback<const float, const float> onTimeUpdate;

		Timer()
		{
			deltaTime = DEFAULT_SIMULATION_DELTA_TIME;
			startTime = .0f;
			elapsedTime = .0f;
		}

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
		@brief Checks that the current simulation delta time 
			   does not exceed the value defined by the macro
			   MAX_SIMULATION_DELTA_TIME. If it does, its
			   value is set back to DEFAULT_SIMULATION_DELTA_TIME
		*/
		void CheckSimulationDeltaTime();
		
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
		@brief Increments the ::elapsedTime by the given @p _deltaTime.
		*/
		inline void Increment(float _deltaTime)
		{
			deltaTime = _deltaTime;
			elapsedTime += _deltaTime;

			onTimeUpdate(elapsedTime-_deltaTime, elapsedTime);
		}

		/*!
		@brief Decrements the ::elapsedTime by the given @p _deltaTime.
		@details  The ::onTimeUpdate callback is called with the new
				 ::elapsedTime and - ::deltaTime (hence, negative).
		*/
		inline void Decrement(float _deltaTime)
		{
			deltaTime = _deltaTime;
			elapsedTime -= _deltaTime;

			onTimeUpdate(elapsedTime+deltaTime, elapsedTime);
		}
		
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