#include "simulation_timer.hpp"

float Timer::readHighResTimer()
{
	return (float)std::clock()/CLOCKS_PER_SEC;
};

