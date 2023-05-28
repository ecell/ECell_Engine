#include "Core/Events/ModifyDataStateValueEvent.hpp"

#include "Core/SimulationsManager.hpp"

void ECellEngine::Core::Events::ModifyDataStateValueEvent::Execute(ECellEngine::Core::SimulationsManager& simulationManager, std::size_t targetSimulationIndex) noexcept
{
	switch (valueType)
	{
		case DataStateValueType::Species:
			simulationManager.GetSimulation(targetSimulationIndex)->GetDataState()->GetSpecies(dataStateValueId)->Set(newValue);
			break;

		case DataStateValueType::Parameter:
			simulationManager.GetSimulation(targetSimulationIndex)->GetDataState()->GetParameter(dataStateValueId)->Set(newValue);
			break;
	}
}