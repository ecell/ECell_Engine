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
	out_involvedComputedParameters.emplace_back(name);

	//The base Operand::GetInvolvedComputedParameters(...) is used to control the clearing
	//of out_involvedComputedParameters. We must put this clearing check after the 
	//out_involvedComputedParameters.emplace_back(name) because a computed parameter (an
	//operation in general) cannot be part of itself. So, we take advantage of the fact that 
	//out_involvedComputedParameters is cleared only when we start resolving new dependencies.
	//We can then keep a computed parameter out of its own dependencies by clearing right
	//after we added its name.
	Operand::GetInvolvedComputedParameters(out_involvedComputedParameters, clearOutVector);

	operation.GetInvolvedComputedParameters(out_involvedComputedParameters, false);
}