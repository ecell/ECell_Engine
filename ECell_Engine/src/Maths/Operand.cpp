#include "Maths/Operand.hpp"

void ECellEngine::Maths::Operand::GetInvolvedEquations(std::vector<std::size_t>& out_involvedEquations, bool clearOutVector) const noexcept
{
	if (clearOutVector)
	{
		out_involvedEquations.clear();
	}
}

void ECellEngine::Maths::Operand::GetInvolvedParameters(std::vector<std::size_t>& out_involvedParameters, bool clearOutVector) const noexcept
{
	if (clearOutVector)
	{
		out_involvedParameters.clear();
	}
}

void ECellEngine::Maths::Operand::GetInvolvedSpecies(std::vector<std::size_t>& out_involvedSpecies, bool clearOutVector) const noexcept
{
	if (clearOutVector)
	{
		out_involvedSpecies.clear();
	}
}