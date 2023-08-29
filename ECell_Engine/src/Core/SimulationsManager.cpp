#include "Core/SimulationsManager.hpp"
#include "Util/BinarySearch.hpp"	
#include "system_time.hpp"

ECellEngine::Core::SimulationsManager& ECellEngine::Core::SimulationsManager::GetSingleton() noexcept
{
	static ECellEngine::Core::SimulationsManager instance;
	return instance;
}

std::pair<bool, std::vector<std::unique_ptr<ECellEngine::Core::Simulation>>::iterator> ECellEngine::Core::SimulationsManager::FindSimulation(const std::size_t _id) noexcept
{
	CompareSimulationIDs compareSimulationIDs;
	std::vector<std::unique_ptr<Simulation>>::iterator it = Util::BinarySearch::LowerBound(simulations.begin(), simulations.end(), _id, compareSimulationIDs);
	if (it != simulations.end() && (*it)->id == _id)
	{
		return std::pair<bool, std::vector<std::unique_ptr<Simulation>>::iterator>(true, it);
	}
	return std::pair<bool, std::vector<std::unique_ptr<Simulation>>::iterator>(false, it);
}


bool ECellEngine::Core::SimulationsManager::PauseSimulation(const size_t _id) noexcept
{
	//playingSimulations.erase(playingSimulations.begin() + _idx);
	return true;
}


bool ECellEngine::Core::SimulationsManager::PlaySimulation(const size_t _id) noexcept
{
	/*Simulation* simulation = FindSimulation(_id);
	if (simulation == nullptr)
	{
		ECellEngine::Logging::Logger::LogError("")
	}

	simulations[_idx]->Start();
	playingSimulations.push_back(simulations[_idx].get());*/
	return true;
}


bool ECellEngine::Core::SimulationsManager::StopSimulation(const size_t _id) noexcept
{
	//TODO: Reset Data in Simulation;
	//playingSimulations.erase(playingSimulations.begin() + _idx);
	return true;
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