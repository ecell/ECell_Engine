#include "Operand.hpp"

void ECellEngine::Maths::Operand::GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector) const noexcept
{
	if (clearOutVector)
	{
		out_involvedSpecies.clear();
	}
}

void ECellEngine::Maths::Operand::GetInvolvedSimpleParameters(std::vector<std::string>& out_involvedSimpleParameters, bool clearOutVector) const noexcept
{
	if (clearOutVector)
	{
		out_involvedSimpleParameters.clear();
	}
}

void ECellEngine::Maths::Operand::GetInvolvedComputedParameters(std::vector<std::string>& out_involvedComputedParameters, bool clearOutVector) const noexcept
{
	if (clearOutVector)
	{
		out_involvedComputedParameters.clear();
	}
}