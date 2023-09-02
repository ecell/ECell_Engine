#include "Data/DataState.hpp"

Operand* ECellEngine::Data::DataState::GetOperand(const std::string& _operandName)
{
	std::list<std::pair<const std::string, std::shared_ptr<Parameter>>>::iterator s1 = parameters.find(_operandName);
	if (s1 != parameters.end())
	{
		return s1->second.get();
	}

	std::list<std::pair<const std::string, std::shared_ptr<Species>>>::iterator s3 = species.find(_operandName);
	if (s3 != species.end())
	{
		return s3->second.get();
	}

	return s3->second.get();//this is null
}

void ECellEngine::Data::DataState::Reset() noexcept
{
	for (auto& [spName, _sp] : species)
	{
		_sp->Reset();
	}

	for (auto& [pName, _p] : parameters)
	{
		_p->Reset();
	}

	for (auto& [eqName, eq] : equations)
	{
		eq->Reset();
	}

	for (auto& [rName, _r] : reactions)
	{
		_r->ComputeKineticLaw();
	}
}
