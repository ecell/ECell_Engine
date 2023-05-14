#include "SimpleParameter.hpp"

void ECellEngine::Data::SimpleParameter::GetInvolvedSimpleParameters(std::vector<std::string>& out_involvedSimpleParameters, bool clearOutVector) const noexcept
{
	Operand::GetInvolvedSimpleParameters(out_involvedSimpleParameters, clearOutVector);

	out_involvedSimpleParameters.emplace_back(name);
}