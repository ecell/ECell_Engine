#pragma once

#include "Simulation.hpp"

namespace ECellEngine::Core
{
	class SimulationsManager
	{
	private:
		std::vector<std::unique_ptr<Simulation>> simulations;

		std::vector<Simulation*> playingSimulations;

	public:
		inline Simulation* GetSimulation(const std::size_t& _idx) noexcept
		{
			return simulations[_idx].get();
		}

		inline void NewSimulation() noexcept
		{
			simulations.push_back(std::make_unique<Simulation>());
		}

		inline void PauseSimulation(const size_t& _idx) noexcept
		{
			playingSimulations.erase(playingSimulations.begin() + _idx);
		}

		inline void PlaySimulation(const size_t& _idx) noexcept
		{
			playingSimulations.push_back(simulations[_idx].get());
		}

		inline void StopSimulation(const size_t& _idx) noexcept
		{
			//TODO: Reset Data in Simulation;
			playingSimulations.erase(playingSimulations.begin() + _idx);
		}

		void UpdatePlayingSimulations(const float& _deltaTime) noexcept;
	};
}