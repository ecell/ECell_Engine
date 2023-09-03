#include "Data/BiochemicalModule.hpp"

void ECellEngine::Data::BiochemicalModule::AddEquation(Operand* _lhs, Operation& _rhs)
{
	dataState.AddEquation(_lhs, _rhs);
	equations.push_back(_lhs->GetID());
}

std::size_t ECellEngine::Data::BiochemicalModule::AddReaction(const std::string _reactionName,
	const std::vector<std::size_t> _products,
	const std::vector<std::size_t> _reactants,
	const Operation _kineticLaw)
{
	std::shared_ptr<ECellEngine::Data::Reaction> reaction = dataState.AddReaction(_reactionName, _products, _reactants, _kineticLaw);
	reactions.push_back(reaction->GetID());
	return reaction->GetID();
}

std::size_t ECellEngine::Data::BiochemicalModule::AddParameter(const std::string _parameterName, const float _value)
{
	std::shared_ptr<ECellEngine::Data::Parameter> param = dataState.AddParameter(_parameterName, _value);
	parameters.push_back(param->GetID());
	return param->GetID();
}

std::size_t ECellEngine::Data::BiochemicalModule::AddSpecies(const std::string _speciesName, const float _quantity)
{
	std::shared_ptr<ECellEngine::Data::Species> sp = dataState.AddSpecies(_speciesName, _quantity);
	species.push_back(sp->GetID());
	return sp->GetID();
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