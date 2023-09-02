#include "Data/BiochemicalModule.hpp"

void ECellEngine::Data::BiochemicalModule::AddEquation(Operand* _lhs, Operation& _rhs)
{
	if (dataState.AddEquation(_lhs, _rhs))
	{
		equations.push_back(_lhs->name);
	}
}

void ECellEngine::Data::BiochemicalModule::AddReaction(const std::string _reactionName,
	const std::vector<std::string> _products,
	const std::vector<std::string> _reactants,
	const Operation _kineticLaw)
{
	if (dataState.AddReaction(_reactionName, _products, _reactants, _kineticLaw))
	{
		reactions.push_back(_reactionName);
	}
}

void ECellEngine::Data::BiochemicalModule::AddParameter(const std::string _parameterName, const float _value)
{
	if (dataState.AddParameter(_parameterName, _value))
	{
		parameters.push_back(_parameterName);
	}
}

void ECellEngine::Data::BiochemicalModule::AddSpecies(const std::string _speciesName, const float _quantity)
{
	if (dataState.AddSpecies(_speciesName, _quantity))
	{
		species.push_back(_speciesName);
	}
}

bool ECellEngine::Data::BiochemicalModule::IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept
{
	return dynamic_cast<const ECellEngine::Solvers::BiochemicalSolver*>(_solver) != nullptr;
}

void ECellEngine::Data::BiochemicalModule::Reset() noexcept
{
	for (auto& [spName, sp] : dataState.GetAllSpecies())
	{
		sp->Reset();
	}

	for (auto& [paramName, param] : dataState.GetParameters())
	{
		param->Reset();
	}

	for (auto& [eqName, eq] : dataState.GetEquations())
	{
		eq->Reset();
	}

	for (auto& [reactionName, reaction] : dataState.GetReactions())
	{
		reaction->ComputeKineticLaw();
	}
}