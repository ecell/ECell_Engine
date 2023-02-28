#include "Timer.hpp"

void ECellEngine::Core::Timer::CheckSimulationDeltaTime()
{
	if (deltaTime > MAX_SIMULATION_DELTA_TIME)
	{
		deltaTime = DEFAULT_SIMULATION_DELTA_TIME;
	}
}

void ECellEngine::Core::Timer::ResetTimes()
{
	startTime = (float)std::clock() / CLOCKS_PER_SEC;
	deltaTime = DEFAULT_SIMULATION_DELTA_TIME;
	elapsedTime = .0f;
}

