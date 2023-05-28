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

			@param simulationsManager Engine simulation manager.
			@param targetSimulationIndex Index of the simulation containing this event in the simulation manager.
			*/
			virtual void Execute(ECellEngine::Core::SimulationsManager& simulationsManager, std::size_t targetSimulationIndex) noexcept = 0;
		};
	}
}