#include "Species.hpp"

void ECellEngine::Data::Species::GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedSpecies(out_involvedSpecies, clearOutVector);

	out_involvedSpecies.emplace_back(name);
}