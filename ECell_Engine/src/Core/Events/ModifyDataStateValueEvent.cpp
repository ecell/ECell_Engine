#include "Core/Events/ModifyDataStateValueEvent.hpp"

#include "Core/SimulationsManager.hpp"

void ECellEngine::Core::Events::ModifyDataStateValueEvent::Execute(ECellEngine::Core::SimulationsManager& _simulationManager, std::size_t _targetSimulationIndex) noexcept
{
	switch (valueType)
	{
		case DataStateValueType::Species:
			_simulationManager.GetSimulation(_targetSimulationIndex)->GetDataState()->GetSpecies(dataStateValueId)->Set(newValue);
			break;

		case DataStateValueType::Parameter:
			_simulationManager.GetSimulation(_targetSimulationIndex)->GetDataState()->GetParameter(dataStateValueId)->Set(newValue);
			break;
	}
}