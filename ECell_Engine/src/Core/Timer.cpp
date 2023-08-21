#include "Core/Timer.hpp"

void ECellEngine::Core::Timer::SetStartTime()
{
	float temp = startTime;
	startTime = (float)std::clock() / CLOCKS_PER_SEC;
	onStartTimeUpdate(temp, startTime);
}

void ECellEngine::Core::Timer::Decrement(float _deltaTime)
{
	float temp = deltaTime;
	deltaTime = _deltaTime;
	if (deltaTime > MAX_SIMULATION_DELTA_TIME)
	{
		deltaTime = DEFAULT_SIMULATION_DELTA_TIME;
	}
	onDeltaTimeUpdate(temp, deltaTime);

	temp = elapsedTime;
	elapsedTime -= _deltaTime;
	onElapsedTimeUpdate(temp, elapsedTime);
}

void ECellEngine::Core::Timer::Increment(float _deltaTime)
{
	float temp = deltaTime;
	deltaTime = _deltaTime;
	if (deltaTime > MAX_SIMULATION_DELTA_TIME)
	{
		deltaTime = DEFAULT_SIMULATION_DELTA_TIME;
	}
	onDeltaTimeUpdate(deltaTime, deltaTime);

	temp = elapsedTime;
	elapsedTime += _deltaTime;
	onElapsedTimeUpdate(temp, elapsedTime);
}

void ECellEngine::Core::Timer::ResetTimes()
{
	float temp = startTime;
	startTime = (float)std::clock() / CLOCKS_PER_SEC;
	onStartTimeUpdate(temp, startTime);

	temp = deltaTime;
	deltaTime = DEFAULT_SIMULATION_DELTA_TIME;
	onDeltaTimeUpdate(temp, deltaTime);
	
	temp = elapsedTime;
	elapsedTime = .0f;
	onElapsedTimeUpdate(temp, elapsedTime);
}

