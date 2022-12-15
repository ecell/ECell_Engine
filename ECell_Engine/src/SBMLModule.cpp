#include "SBMLModule.hpp"

void ECellEngine::Data::SBMLModule::ComputeDependantParameters(const std::string& _parentParameterName)
{
	auto search = parameterDependancies.find(_parentParameterName);
	if (search != parameterDependancies.end())
	{
		for (auto it = search->second.begin(); it != search->second.end(); it++)
		{
			computedParameters[*it].get()->ComputeOperation(*dataState);
			ComputeDependantParameters(computedParameters[*it].get()->GetName());
		}
	}
}