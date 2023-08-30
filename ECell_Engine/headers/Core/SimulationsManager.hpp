#pragma once

#include <cstdint>

#include "Core/IDProvider.hpp"
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
		@brief The ID provider for the simulations.
		*/
		IDProvider simulationIDProvider;

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
		@brief The comparison function object to compare simulations based on
				their IDs.
		*/
		struct CompareSimulationIDs
		{
			/*!
			@brief Compares two simulations based on their IDs.
			@param _lhs The left hand side simulation to compare.
			@param _rhs The right hand side simulation to compare.
			@returns True if the ID of @p _lhs->id < @p _rhs->id; false otherwise.
			*/
			bool operator()(std::unique_ptr<Simulation>& _lhs, std::unique_ptr<Simulation>& _rhs) const noexcept
			{
				return _lhs->id < _rhs->id;
			}
			
			/*!
			@brief Compares a simulation based on its ID and a given ID.
			@param _lhs The left hand side simulation to compare.
			@param _id The ID to compare with.
			@returns True if the ID of @p _lhs->id < @p _id; false otherwise.
			*/
			bool operator()(std::unique_ptr<Simulation>& _lhs, std::size_t _id) const noexcept
			{
				return _lhs->id < _id;
			}
			
			/*!
			@brief Compares a simulation based on its ID and a given ID.
			@param _lhs The left hand side simulation to compare.
			@param _id The ID to compare with.
			@returns True if the ID of @p _lhs->id < @p _id; false otherwise.
			*/
			bool operator()(Simulation* _lhs, std::size_t _id) const noexcept
			{
				return _lhs->id < _id;
			}
		};

		static SimulationsManager& GetSingleton() noexcept;

		/*!
		@brief Gets the number of simulation in ::simulations.
		*/
		/*inline std::size_t CountSimulations() const noexcept
		{
			return simulations.size();
		}*/

		/*!
		@brief Gets the number of playing simulation in ::playingSimulations.
		*/
		/*inline std::size_t CountPlayingSimulations() const noexcept
		{
			return playingSimulations.size();
		}*/

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
		@brief Finds the simulation with ID @p _id in ::playingSimulations.
		@details Performs a binary search.
		@param _id The ID of the simulation to retrieve from ::playingSimulations.
		@returns A pair with a bool at the first place and the pointer to the 
				result of the binary search. The bool is true if the simulation
				was found; false otherwise.
		*/
		std::pair<bool, std::vector<ECellEngine::Core::Simulation*>::iterator> FindPlayingSimulation(const std::size_t _id) noexcept;

		/*!
		@brief Finds the simulation with ID @p _id in ::simulations.
		@details Performs a binary search.
		@param _id The ID of the simulation to retrieve from ::simulations.
		@returns A pair with a bool at the first place and the pointer to the 
				result of the binary search. The bool is true if the simulation
				was found; false otherwise.
		*/
		std::pair<bool, std::vector<std::unique_ptr<Simulation>>::iterator> FindSimulation(const std::size_t _id) noexcept;

		/*!
		@brief Instantiates a new simulation and adds it to ::simulations.
		*/
		inline void NewSimulation() noexcept
		{
			simulations.push_back(std::make_unique<Simulation>(++simulationIDProvider));
		}

		/*!
		@brief Pauses the execution the simulation with ID @p _id in ::playingSimulations.
		@param _playingSimulation The iterator to the target simulation in ::playingSimulations.
		*/
		bool PauseSimulation(std::vector<Simulation*>::iterator _playingSimulation) noexcept;

		/*!
		@brief Adds in ::playingSimulations the simulation with ID @p _id in
				::simulations.
		@param _simulation The iterator to the target simulation in ::simulations.
		*/
		bool PlaySimulation(std::vector<std::unique_ptr<Simulation>>::iterator _simulation) noexcept;

		/*!
		@brief Removes the playing simulation with ID @p _id from ::playingSimulations.
		@param _playingSimulation The iterator to the target simulation in ::playingSimulations.
		*/
		bool StopSimulation(std::vector<Simulation*>::iterator _playingSimulation) noexcept;

		/*!
		@brief Updates one step of duration @p _deltaTime for every simulation 
				in ::playingSimulations.
		@param _deltaTime The duration to simulate.
		*/
		void UpdatePlayingSimulations(const float _deltaTime) noexcept;
	};
}