#pragma once

#include <cstdint>

namespace ECellEngine::Core
{
	class SimulationsManager;

	namespace Events
	{
		/*!
		@brief Class used to execute arbitrary code (callback).
		*/
		class Event
		{
		public:
			Event() = default;
			virtual ~Event() = default;

			/*!
			@brief Execute the callback.

			@param _simulationsManager Engine simulation manager.
			@param _targetSimulationIndex Index of the simulation containing this event in the simulation manager.
			*/
			virtual void Execute(ECellEngine::Core::SimulationsManager& _simulationsManager, std::size_t _targetSimulationIndex) noexcept = 0;
		};
	}
}