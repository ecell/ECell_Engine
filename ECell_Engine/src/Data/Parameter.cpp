#include "Data/Parameter.hpp"

void ECellEngine::Data::Parameter::GetInvolvedParameters(std::vector<std::string>& out_involvedParameters, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedParameters(out_involvedParameters, clearOutVector);

	out_involvedParameters.emplace_back(name);
}