#include "Timer.hpp"

void ECellEngine::Core::Timer::CheckSimulationDeltaTime()
{
	if (deltaTime > MAX_SIMULATION_DELTA_TIME)
	{
		deltaTime = DEFAULT_SIMULATION_DELTA_TIME;
	}
}

float ECellEngine::Core::Timer::GetDuration(float& _t1, float& _t2)
{
	return _t2 - _t1;
}

float ECellEngine::Core::Timer::ReadHighResTimer()
{
	return (float)std::clock() / CLOCKS_PER_SEC;
}

void ECellEngine::Core::Timer::ResetTimes()
{
	startTime = .0f;
	deltaTime = DEFAULT_SIMULATION_DELTA_TIME;
	elapsedTime = .0f;
}

void ECellEngine::Core::Timer::SetStartTime()
{
	startTime = (float)std::clock() / CLOCKS_PER_SEC;
}

