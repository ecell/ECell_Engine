#pragma once

/*
	Defines the interface to manipulate the simulation time
*/
#include <ctime>
#include "system_time.hpp"

namespace ECellEngine::Core
{
	struct Timer
	{
		float deltaTime;
		float startTime;
		float elapsedTime;

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
		inline void SetStartTime()
		{
			startTime = (float)std::clock() / CLOCKS_PER_SEC;
		}
#pragma endregion


#pragma region Logic
		/// <summary>
		/// Checks that the current simulation delta time 
		/// does not exceed the value defined by the macro
		/// MAX_SIMULATION_DELTA_TIME. If it does, its
		/// value is set back to DEFAULT_SIMULATION_DELTA_TIME
		/// </summary>
		void CheckSimulationDeltaTime();

		/// <param name="_t1">Time 1</param>
		/// <param name="_t2">Time 2</param>
		/// <returns> Returns _t2-_t1</returns>
		inline float GetDuration(float& _t1, float& _t2) const
		{
			return _t2 - _t1;
		}

		inline void Increment(float _deltaTime)
		{
			deltaTime = _deltaTime;
			elapsedTime += _deltaTime;
		}

		inline void Decrement(float _deltaTime)
		{
			deltaTime = _deltaTime;
			elapsedTime -= _deltaTime;
		}

		/// <summary>
		/// Returns the time used by the program normalised by the CPU cycles per seconds.
		/// </summary>
		/// <returns></returns>
		inline float ReadHighResTimer() const
		{
			return (float)std::clock() / CLOCKS_PER_SEC;
		}

		/// <summary>
		/// Sets the time member values back to default.
		/// </summary>
		void ResetTimes();
#pragma endregion
	};
}