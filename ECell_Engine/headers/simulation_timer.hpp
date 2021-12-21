/*
	Defines the interface to manipulate the simulation time
*/
#include <ctime>

class Timer
{
public:
	//Members
	float start_time = 0.0f;

	//Methods
	
	/// <param name="_t1">Time 1</param>
	/// <param name="_t2">Time 2</param>
	/// <returns> Returns _t2-_t1</returns>
	float GetDuration(float& _t1, float& _t2);

	/// <summary>
	/// Returns the time used by the program normalised by the CPU cycles per seconds.
	/// </summary>
	/// <returns></returns>
	float ReadHighResTimer();

	void SetStartTime();
	
};

