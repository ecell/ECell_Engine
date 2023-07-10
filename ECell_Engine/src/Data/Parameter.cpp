#include "Data/Parameter.hpp"

void ECellEngine::Data::Parameter::GetInvolvedParameters(std::vector<std::string>& out_involvedParameters, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedParameters(out_involvedParameters, clearOutVector);

	out_involvedParameters.emplace_back(name);
}

void ECellEngine::Data::Parameter::Set(const float _value) noexcept
{
	previousValue = value;
	value = _value;

	if (previousValue != value)
	{
		onValueChange(previousValue, value);
	}
}

void ECellEngine::Data::Parameter::UpdateValue(const float _previousValue, const float _newValue) noexcept
{
	previousValue = _previousValue;
	value = _newValue;

	if (previousValue != value)
	{
		onValueChange(previousValue, value);
	}
}