#include "DataState.hpp"

Operand& ECellEngine::Data::DataState::GetOperand(const std::string& _operandName)
{
	std::list<std::pair<const std::string, ComputedParameter>>::iterator s1 = computedParameters.find(_operandName);
	if (s1 != computedParameters.end())
	{
		return s1->second;
	}

	std::list<std::pair<const std::string, SimpleParameter>>::iterator s2 = simpleParameters.find(_operandName);
	if (s2 != simpleParameters.end())
	{
		return s2->second;
	}

	std::list<std::pair<const std::string, Species>>::iterator s3 = species.find(_operandName);
	if (s3 != species.end())
	{
		return s3->second;
	}

	return s3->second;//this is null
}

void ECellEngine::Data::DataState::ClearReactions(const std::vector<std::string>& _reactionNames)
{
	for (auto it = _reactionNames.begin(); it != _reactionNames.end(); it++)
	{
		reactions.erase(*it);
	}
}

void ECellEngine::Data::DataState::ClearComputedParameters(const std::vector<std::string>& _parameterNames)
{
	for (auto it = _parameterNames.begin(); it != _parameterNames.end(); it++)
	{
		computedParameters.erase(*it);
	}
}

void ECellEngine::Data::DataState::ClearSimpleParameters(const std::vector<std::string>& _parameterNames)
{
	for (auto it = _parameterNames.begin(); it != _parameterNames.end(); it++)
	{
		simpleParameters.erase(*it);
	}
}

void ECellEngine::Data::DataState::ClearSpecies(const std::vector<std::string>& _speciesNames)
{
	for (auto it = _speciesNames.begin(); it != _speciesNames.end(); it++)
	{
		species.erase(*it);
	}
}

void ECellEngine::Data::DataState::LinkSpeciesWithKineticLaw(const std::string& _reactionName)
{
	std::vector<std::string> speciesInKineticLaw{ _reactionName };
	reactions.at(_reactionName).GetKineticLaw().GetOperandsNames<Species>(speciesInKineticLaw);
	for (std::vector<std::string>::iterator it = speciesInKineticLaw.begin(); it != speciesInKineticLaw.end(); it++)
	{
		operandsToOperations.emplace(*it, _reactionName);
	}
}
