#include "Core/SimulationsManager.hpp"
#include "system_time.hpp"
ECellEngine::Core::SimulationsManager& ECellEngine::Core::SimulationsManager::GetSingleton() noexcept
{
	static ECellEngine::Core::SimulationsManager instance;
	return instance;
}

void ECellEngine::Core::SimulationsManager::UpdatePlayingSimulations(const float _deltaTime) noexcept
{
	float dt = _deltaTime;
	if (_deltaTime > MAX_SIMULATION_DELTA_TIME)
	{
		dt = MAX_SIMULATION_DELTA_TIME;
	}

	for (std::vector<Simulation*>::iterator it = playingSimulations.begin(); it != playingSimulations.end(); it++)
	{
		(*it)->Update(dt);
	}
}