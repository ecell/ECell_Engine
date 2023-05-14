#include "Parameter.hpp"

void ECellEngine::Data::Parameter::GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedSpecies(out_involvedSpecies, clearOutVector);

	//Nothing more to do
}