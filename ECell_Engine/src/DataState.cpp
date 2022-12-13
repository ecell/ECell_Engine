#include "DataState.hpp"

void ECellEngine::Data::DataState::ClearKineticLaws(const std::vector<std::string>& _kineticLawNames)
{
	for (auto it = _kineticLawNames.begin(); it != _kineticLawNames.end(); it++)
	{
		kineticLaws.erase(*it);
	}
}

void ECellEngine::Data::DataState::ClearParameters(const std::vector<std::string>& _parameterNames)
{
	for (auto it = _parameterNames.begin(); it != _parameterNames.end(); it++)
	{
		parameters.erase(*it);
	}
}

void ECellEngine::Data::DataState::ClearSpecies(const std::vector<std::string>& _speciesNames)
{
	for (auto it = _speciesNames.begin(); it != _speciesNames.end(); it++)
	{
		species.erase(*it);
	}
}