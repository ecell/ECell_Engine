#include "Core/SimulationsManager.hpp"
#include "Util/BinarySearch.hpp"	
#include "system_time.hpp"

ECellEngine::Core::SimulationsManager& ECellEngine::Core::SimulationsManager::GetSingleton() noexcept
{
	static ECellEngine::Core::SimulationsManager instance;
	return instance;
}

ECellEngine::Core::Simulation* ECellEngine::Core::SimulationsManager::FindSimulation(const std::size_t _id) noexcept
{
	CompareSimulationIDs compareSimulationIDs;
	std::vector<std::unique_ptr<Simulation>>::iterator it = Util::BinarySearch::LowerBound(simulations.begin(), simulations.end(), _id, compareSimulationIDs);
	if (it != simulations.end() && (*it)->id == _id)
	{
		return it->get();
	}
	return nullptr;
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