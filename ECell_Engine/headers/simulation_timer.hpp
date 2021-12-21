/*
	Defines the interface to manipulate the simulation time
*/
#include <ctime>

#include "system_time.hpp"

class Timer
{
public:

	/// <summary>
	/// Returns the time used by the program normalised by the CPU cycles per seconds.
	/// </summary>
	/// <returns></returns>
	float readHighResTimer();
	
};

