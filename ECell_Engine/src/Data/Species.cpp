#include "Data/Species.hpp"

void ECellEngine::Data::Species::GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedSpecies(out_involvedSpecies, clearOutVector);

	out_involvedSpecies.emplace_back(name);
}

void ECellEngine::Data::Species::UpdateQuantity(const float _previousValue, const float _newValue) noexcept
{
	previousQuantity = quantity;
	quantity = _newValue;

	if (_previousValue != _newValue)
	{
		onValueChange(previousQuantity, quantity);
	}
}