#include "simulation_timer.hpp"

float Timer::GetDuration(float& _t1, float& _t2)
{
	return _t2 - _t1;
}

float Timer::ReadHighResTimer()
{
	return (float)std::clock()/CLOCKS_PER_SEC;
};

void Timer::SetStartTime()
{
	start_time = (float)std::clock() / CLOCKS_PER_SEC;
}

