#include "ComputedParameter.hpp"

void ECellEngine::Data::ComputedParameter::GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector) const noexcept
{
	Parameter::GetInvolvedSpecies(out_involvedSpecies, clearOutVector);

	operation.GetInvolvedSpecies(out_involvedSpecies, false);
}