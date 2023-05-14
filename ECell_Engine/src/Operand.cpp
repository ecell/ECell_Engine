#include "Operand.hpp"

void ECellEngine::Maths::Operand::GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector) const noexcept
{
	if (clearOutVector)
	{
		out_involvedSpecies.clear();
	}
}