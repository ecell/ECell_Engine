#include "Core/SimulationsManager.hpp"
#include "Util/BinarySearch.hpp"	
#include "system_time.hpp"

ECellEngine::Core::SimulationsManager& ECellEngine::Core::SimulationsManager::GetSingleton() noexcept
{
	static ECellEngine::Core::SimulationsManager instance;
	return instance;
}

std::pair<bool, std::vector<ECellEngine::Core::Simulation*>::iterator> ECellEngine::Core::SimulationsManager::FindPlayingSimulation(const std::size_t _id) noexcept
{
	static CompareSimulationIDs compareSimulationIDs;
	std::vector<Simulation*>::iterator it = Util::BinarySearch::LowerBound(playingSimulations.begin(), playingSimulations.end(), _id, compareSimulationIDs);
	if (it != playingSimulations.end() && (*it)->id == _id)
	{
		return std::pair<bool, std::vector<Simulation*>::iterator>(true, it);
	}
	return std::pair<bool, std::vector<Simulation*>::iterator>(false, it);
}

std::pair<bool, std::vector<std::unique_ptr<ECellEngine::Core::Simulation>>::iterator> ECellEngine::Core::SimulationsManager::FindSimulation(const std::size_t _id) noexcept
{
	static CompareSimulationIDs compareSimulationIDs;
	std::vector<std::unique_ptr<Simulation>>::iterator it = Util::BinarySearch::LowerBound(simulations.begin(), simulations.end(), _id, compareSimulationIDs);
	if (it != simulations.end() && (*it)->id == _id)
	{
		return std::pair<bool, std::vector<std::unique_ptr<Simulation>>::iterator>(true, it);
	}
	return std::pair<bool, std::vector<std::unique_ptr<Simulation>>::iterator>(false, it);
}


bool ECellEngine::Core::SimulationsManager::PauseSimulation(std::vector<Simulation*>::iterator _playingSimulation) noexcept
{
	playingSimulations.erase(_playingSimulation);
	return true;
}


bool ECellEngine::Core::SimulationsManager::PlaySimulation(std::vector<std::unique_ptr<Simulation>>::iterator _simulation) noexcept
{
	//Check if simulation is already playing
	std::pair<bool, std::vector<Simulation*>::iterator> search = FindPlayingSimulation((*_simulation)->id);
	if (search.first)
	{
		ECellEngine::Logging::Logger::LogError("PlaySimulation Failed: Simulation with ID \"%llu\" is already playing.", (*_simulation)->id);
		return false;
	}

	(*_simulation)->Start();
	playingSimulations.insert(search.second, (*_simulation).get());
	return true;
}


bool ECellEngine::Core::SimulationsManager::StopSimulation(std::vector<Simulation*>::iterator _playingSimulation) noexcept
{
	//Reset Data in Simulation;
	(*_playingSimulation)->Reset();
	playingSimulations.erase(_playingSimulation);
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