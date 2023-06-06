#include "Core/SimulationsManager.hpp"

ECellEngine::Core::SimulationsManager& ECellEngine::Core::SimulationsManager::GetSingleton() noexcept
{
	static ECellEngine::Core::SimulationsManager instance;
	return instance;
}

void ECellEngine::Core::SimulationsManager::UpdatePlayingSimulations(const float& _deltaTime) noexcept
{
	for (std::vector<Simulation*>::iterator it = playingSimulations.begin(); it != playingSimulations.end(); it++)
	{
		(*it)->Update(_deltaTime);
	}
}