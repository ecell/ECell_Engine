#pragma once

#include "Core/Simulation.hpp"

namespace ECellEngine::Core
{
	/*!
	@brief Manages the collection of simulations existing in the
			engine.
	*/
	class SimulationsManager
	{
	private:
		/*!
		@brief All created simulations.
		*/
		std::vector<std::unique_ptr<Simulation>> simulations;

		/*!
		@brief All simulations currently running and that needs to be updated.
		*/
		std::vector<Simulation*> playingSimulations;

	public:

		/*!
		@brief Gets the number of simulation in ::simulations.
		*/
		inline std::size_t CountSimulations() const noexcept
		{
			return simulations.size();
		}

		/*!
		@brief Gets the number of playing simulation in ::playingSimulations.
		*/
		inline std::size_t CountPlayingSimulations() const noexcept
		{
			return playingSimulations.size();
		}

		/*!
		@brief Gets the simulation at position @p _idx in ::simulations.
		@param _idx The position of the simulation to retrieve from ::simulations.
		@returns The pointer to the target simulation.
		*/
		inline Simulation* GetSimulation(const std::size_t& _idx) const noexcept
		{
			return simulations[_idx].get();
		}

		/*!
		@brief Instantiates a new simulation and adds it to ::simulations.
		*/
		inline void NewSimulation() noexcept
		{
			simulations.push_back(std::make_unique<Simulation>());
		}

		/*!
		@brief Pauses the execution the simulation at position @p _idx in
				::playingSimulations.
		@param _idx The position of the target simulation in ::playingSimulations.
		*/
		inline void PauseSimulation(const size_t& _idx) noexcept
		{
			playingSimulations.erase(playingSimulations.begin() + _idx);
		}

		/*!
		@brief Adds in ::playingSimulations the simulation at position
				@p _idx in ::simulations.
		@details The simulation will start updating at the next frame.
		@param _idx The position of the target simulation in ::simulations.
		*/
		inline void PlaySimulation(const size_t& _idx) noexcept
		{
			playingSimulations.push_back(simulations[_idx].get());
		}

		/*!
		@brief Removes the simulation at position @p _idx from ::playingSimulations.
		@param _idx The position of the target simulation in ::simulations.
		*/
		inline void StopSimulation(const size_t& _idx) noexcept
		{
			//TODO: Reset Data in Simulation;
			playingSimulations.erase(playingSimulations.begin() + _idx);
		}

		/*!
		@brief Updates one step of duration @p _deltaTime for every simulation 
				in ::playingSimulations.
		@param _deltaTime The duration to simulate.
		*/
		void UpdatePlayingSimulations(const float& _deltaTime) noexcept;
	};
}