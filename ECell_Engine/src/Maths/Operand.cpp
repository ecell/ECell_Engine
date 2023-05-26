#include "Maths/Operand.hpp"

void ECellEngine::Maths::Operand::GetInvolvedEquations(std::vector<std::string>& out_involvedEquations, bool clearOutVector) const noexcept
{
	if (clearOutVector)
	{
		out_involvedEquations.clear();
	}
}

void ECellEngine::Maths::Operand::GetInvolvedParameters(std::vector<std::string>& out_involvedParameters, bool clearOutVector) const noexcept
{
	if (clearOutVector)
	{
		out_involvedParameters.clear();
	}
}

void ECellEngine::Maths::Operand::GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector) const noexcept
{
	if (clearOutVector)
	{
		out_involvedSpecies.clear();
	}
}