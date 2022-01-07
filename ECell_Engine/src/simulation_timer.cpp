#include "simulation_timer.hpp"

void Timer::CheckSimulationDeltaTime()
{
	if (deltaTime > MAX_SIMULATION_DELTA_TIME)
	{
		deltaTime = DEFAULT_SIMULATION_DELTA_TIME;
	}
}

float Timer::GetDuration(float& _t1, float& _t2)
{
	return _t2 - _t1;
}

float Timer::ReadHighResTimer()
{
	return (float)std::clock() / CLOCKS_PER_SEC;
}

void Timer::SetStartTime()
{
	startTime = (float)std::clock() / CLOCKS_PER_SEC;
}

