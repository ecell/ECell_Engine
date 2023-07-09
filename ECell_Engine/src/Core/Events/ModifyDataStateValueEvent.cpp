#include "Core/Events/ModifyDataStateValueEvent.hpp"

#include "Core/SimulationsManager.hpp"

void ECellEngine::Core::Events::ModifyDataStateValueEvent::UpdateCondition(const bool _previousValue, const bool _newValue) noexcept
{
	condition = _newValue;
}

void ECellEngine::Core::Events::ModifyDataStateValueEvent::UpdateDataStateValueId(const std::string& _previousValue, const std::string& _newValue) noexcept
{
	dataStateValueId = _newValue;
}

void ECellEngine::Core::Events::ModifyDataStateValueEvent::UpdateValue(const float _previousValue, const float _newValue) noexcept
{
	value = _newValue;
}

void ECellEngine::Core::Events::ModifyDataStateValueEvent::Execute(std::size_t _targetSimulationIndex) noexcept
{
	if (condition)
	{
		switch (valueType)
		{
			case DataStateValueType::Species:
				SimulationsManager::GetSingleton().GetSimulation(_targetSimulationIndex)->GetDataState()->GetSpecies(dataStateValueId)->Set(value);
				break;

			case DataStateValueType::Parameter:
				SimulationsManager::GetSingleton().GetSimulation(_targetSimulationIndex)->GetDataState()->GetParameter(dataStateValueId)->Set(value);
				break;
		}
		condition = false;
	}
}