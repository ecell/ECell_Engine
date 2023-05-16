#include "Data/ComputedParameter.hpp"

void ECellEngine::Data::ComputedParameter::GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedSpecies(out_involvedSpecies, clearOutVector);

	operation.GetInvolvedSpecies(out_involvedSpecies, false);
}

void ECellEngine::Data::ComputedParameter::GetInvolvedSimpleParameters(std::vector<std::string>& out_involvedSimpleParameters, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedSimpleParameters(out_involvedSimpleParameters, clearOutVector);

	operation.GetInvolvedSimpleParameters(out_involvedSimpleParameters, false);
}

void ECellEngine::Data::ComputedParameter::GetInvolvedComputedParameters(std::vector<std::string>& out_involvedComputedParameters, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedComputedParameters(out_involvedComputedParameters, clearOutVector);
	
	out_involvedComputedParameters.emplace_back(name);

	operation.GetInvolvedComputedParameters(out_involvedComputedParameters, false);
}