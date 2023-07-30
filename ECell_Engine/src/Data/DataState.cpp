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

void ECellEngine::Data::DataState::ClearReactions(const std::vector<std::string>& _reactionNames)
{
	for (auto it = _reactionNames.begin(); it != _reactionNames.end(); it++)
	{
		reactions.erase(*it);
	}
}

void ECellEngine::Data::DataState::ClearParameters(const std::vector<std::string>& _parameterNames)
{
	for (auto it = _parameterNames.begin(); it != _parameterNames.end(); it++)
	{
		parameters.erase(*it);
	}
}

void ECellEngine::Data::DataState::ClearEquations(const std::vector<std::string>& _equationNames)
{
	for (auto it = _equationNames.begin(); it != _equationNames.end(); it++)
	{
		equations.erase(*it);
	}
}

void ECellEngine::Data::DataState::ClearSpecies(const std::vector<std::string>& _speciesNames)
{
	for (auto it = _speciesNames.begin(); it != _speciesNames.end(); it++)
	{
		species.erase(*it);
	}
}
