#include "Core/Events/ModifyDataStateValueEvent.hpp"

#include "Core/SimulationsManager.hpp"

void ECellEngine::Core::Events::ModifyDataStateValueEvent::Execute(std::size_t _targetSimulationIndex) noexcept
{
	switch (valueType)
	{
		case DataStateValueType::Species:
			SimulationsManager::GetSingleton().GetSimulation(_targetSimulationIndex)->GetDataState()->GetSpecies(dataStateValueId)->Set(newValue);
			break;

		case DataStateValueType::Parameter:
			SimulationsManager::GetSingleton().GetSimulation(_targetSimulationIndex)->GetDataState()->GetParameter(dataStateValueId)->Set(newValue);
			break;
	}
}