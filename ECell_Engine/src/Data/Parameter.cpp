#include "Data/Parameter.hpp"

void ECellEngine::Data::Parameter::GetInvolvedParameters(std::vector<std::size_t>& out_involvedParameters, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedParameters(out_involvedParameters, clearOutVector);

	out_involvedParameters.emplace_back(id);
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

float ECellEngine::Data::Parameter::Reset() noexcept
{
	previousValue = value;
	value = initialValue;

	if (previousValue != value)
	{
		onValueChange(previousValue, value);
	}

	return value;
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