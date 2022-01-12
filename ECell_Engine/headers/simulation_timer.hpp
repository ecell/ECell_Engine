/*
	Defines the interface to manipulate the simulation time
*/
#include <ctime>
#include "system_time.hpp"

class Timer
{
public:
	//Members
	float deltaTime = DEFAULT_SIMULATION_DELTA_TIME;
	float startTime = .0f;
	float elapsedTime = .0f;

#pragma region Mutators
	/// <summary>
	/// Set the <see cref="startTime"/> public member.
	/// Uses the high resolution current time.
	/// </summary>
	void SetStartTime();
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
	float GetDuration(float& _t1, float& _t2);

	/// <summary>
	/// Returns the time used by the program normalised by the CPU cycles per seconds.
	/// </summary>
	/// <returns></returns>
	float ReadHighResTimer();

	/// <summary>
	/// Sets the time member values back to default.
	/// </summary>
	void ResetTimes();
#pragma endregion
	
};

