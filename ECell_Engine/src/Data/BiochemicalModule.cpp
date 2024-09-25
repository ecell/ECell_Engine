#include "Data/BiochemicalModule.hpp"

void ECellEngine::Data::BiochemicalModule::AddEquation(Operand* _lhs, Operation& _rhs)
{
	dataState.AddEquation(_lhs, _rhs);
	onEquationDestroySubTokens.push_back(std::move(dataState.GetEquation(_lhs->GetID())->onDestroy += std::bind(&ECellEngine::Data::BiochemicalModule::OnEquationDestroy, this, std::placeholders::_1)));
	equations.push_back(_lhs->GetID());
}

std::size_t ECellEngine::Data::BiochemicalModule::AddReaction(const std::string _reactionName,
	const std::vector<std::size_t> _products,
	const std::vector<std::size_t> _reactants,
	const Operation _kineticLaw)
{
	std::shared_ptr<ECellEngine::Data::Reaction> reaction = dataState.AddReaction(_reactionName.c_str(), _products, _reactants, _kineticLaw);
	onReactionDestroySubTokens.push_back(std::move(reaction->onDestroy += std::bind(&ECellEngine::Data::BiochemicalModule::OnReactionDestroy, this, std::placeholders::_1)));
	reactions.push_back(reaction->GetID());
	return reaction->GetID();
}

std::size_t ECellEngine::Data::BiochemicalModule::AddParameter(const std::string _parameterName, const float _value)
{
	std::shared_ptr<ECellEngine::Data::Parameter> param = dataState.AddParameter(_parameterName.c_str(), _value);
	onParameterDestroySubTokens.push_back(std::move(param->onDestroy += std::bind(&ECellEngine::Data::BiochemicalModule::OnParameterDestroy, this, std::placeholders::_1)));
	parameters.push_back(param->GetID());
	return param->GetID();
}

std::size_t ECellEngine::Data::BiochemicalModule::AddSpecies(const std::string _speciesName, const float _quantity)
{
	std::shared_ptr<ECellEngine::Data::Species> sp = dataState.AddSpecies(_speciesName.c_str(), _quantity);
	onSpeciesDestroySubTokens.push_back(std::move(sp->onDestroy += std::bind(&ECellEngine::Data::BiochemicalModule::OnSpeciesDestroy, this, std::placeholders::_1)));
	species.push_back(sp->GetID());
	return sp->GetID();
}

bool ECellEngine::Data::BiochemicalModule::IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept
{
	return dynamic_cast<const ECellEngine::Solvers::BiochemicalSolver*>(_solver) != nullptr;
}

void ECellEngine::Data::BiochemicalModule::OnEquationDestroy(Equation* _eq) noexcept
{
	int idx = 0;
	while (idx < equations.size() && equations[idx] != _eq->GetID())
	{
		++idx;
	}

	if (idx < equations.size())
	{
		equations.erase(equations.begin() + idx);
		onEquationDestroySubTokens.erase(onEquationDestroySubTokens.begin() + idx);
	}
	else
	{
		ECellEngine::Logging::Logger::LogError("BiochemicalModule (%s) OnEquationDestroy: Equation (ID: %llu) not found in the list of equations.", GetName(), _eq->GetID());
	}

}

void ECellEngine::Data::BiochemicalModule::OnParameterDestroy(Operand* _param) noexcept
{
	int idx = 0;
	while (idx < parameters.size() && parameters[idx] != _param->GetID())
	{
		++idx;
	}

	if (idx < parameters.size())
	{
		parameters.erase(parameters.begin() + idx);
		onParameterDestroySubTokens.erase(onParameterDestroySubTokens.begin() + idx);
	}
	else
	{
		ECellEngine::Logging::Logger::LogError("BiochemicalModule (%s) OnParameterDestroy: Parameter (ID: %llu) not found in the list of parameters.", GetName(), _param->GetID());
	}
}

void ECellEngine::Data::BiochemicalModule::OnReactionDestroy(Reaction* _reaction) noexcept
{
	int idx = 0;
	while (idx < reactions.size() && reactions[idx] != _reaction->GetID())
	{
		++idx;
	}

	if (idx < reactions.size())
	{
		reactions.erase(reactions.begin() + idx);
		onReactionDestroySubTokens.erase(onReactionDestroySubTokens.begin() + idx);
	}
	else
	{
		ECellEngine::Logging::Logger::LogError("BiochemicalModule (%s) OnReactionDestroy: Reaction (ID: %llu) not found in the list of reactions.", GetName(), _reaction->GetID());
	}
}

void ECellEngine::Data::BiochemicalModule::OnSpeciesDestroy(Operand* _op) noexcept
{
	int idx = 0;
	while (idx < species.size() && species[idx] != _op->GetID())
	{
		++idx;
	}

	if (idx < species.size())
	{
		species.erase(species.begin() + idx);
		onSpeciesDestroySubTokens.erase(onSpeciesDestroySubTokens.begin() + idx);
	}
	else
	{
		ECellEngine::Logging::Logger::LogError("BiochemicalModule (%s) OnSpeciesDestroy: Species (ID: %llu) not found in the list of species.", GetName(), _op->GetID());
	}
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