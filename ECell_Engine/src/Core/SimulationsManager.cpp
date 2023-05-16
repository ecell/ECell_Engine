#include "Core/SimulationsManager.hpp"

void ECellEngine::Core::SimulationsManager::UpdatePlayingSimulations(const float& _deltaTime) noexcept
{
	for (std::vector<Simulation*>::iterator it = playingSimulations.begin(); it != playingSimulations.end(); it++)
	{
		(*it)->Update(_deltaTime);
	}
}