#include "Data/DependenciesDatabase.hpp"

void ECellEngine::Data::DependenciesDatabase::RefreshEquationDependencies(const DataState& dataState, std::shared_ptr<Maths::Equation> _equation) noexcept
{
	std::vector<std::string> dependencies;

	//Resolve species -> equation dependencies
	_equation->GetOperation().GetInvolvedSpecies(dependencies, true);
	for (const std::string& species : dependencies)
	{
		SpeciesDependencies& dependencies = speciesDependencies[dataState.GetSpecies(species)];

		dependencies.partOfEquation.emplace_back(_equation);
	}

	//Resolve parameter -> equation dependencies
	_equation->GetOperation().GetInvolvedParameters(dependencies, true);
	for (const std::string& dependentParam : dependencies)
	{
		ParameterDependencies& dependencies = parameterDependencies[dataState.GetParameter(dependentParam)];

		dependencies.partOfEquation.emplace_back(_equation);
	}

	//Resolve equation -> equation dependencies
	_equation->GetOperation().GetInvolvedEquations(dependencies, true);
	for (const std::string& dependentEquation : dependencies)
	{
		ParameterDependencies& dependencies = equationDependencies[dataState.GetEquation(dependentEquation)];

		dependencies.partOfEquation.emplace_back(_equation);
	}
}

void ECellEngine::Data::DependenciesDatabase::RefreshReactionDependencies(const DataState& dataState, std::shared_ptr<Reaction> reaction)  noexcept
{
	//Resolve species (as reactant) -> reaction dependencies
	for (const std::string& reactant : reaction->GetReactants())
	{
		//Might have duplicatas here, must use another datastructure to make sure we don't add duplicatas (std::unordered_set or ordered std::vector)
		SpeciesDependencies& dependencies = speciesDependencies[dataState.GetSpecies(reactant)];

		dependencies.partOfReactionAsReactant.emplace_back(reaction);
	}

	//Resolve species (as product) -> reaction dependencies
	for (const std::string& product : reaction->GetProducts())
	{
		//Might have duplicatas here, must use another datastructure to make sure we don't add duplicatas (std::unordered_set or ordered std::vector)
		SpeciesDependencies& dependencies = speciesDependencies[dataState.GetSpecies(product)];

		dependencies.partOfReactionAsProduct.emplace_back(reaction);
	}

	//Resolve species -> kinetic law dependencies
	std::vector<std::string> kineticLawDependencies;

	reaction->GetKineticLaw().GetInvolvedSpecies(kineticLawDependencies, true);
	for (const std::string& species : kineticLawDependencies)
	{
		SpeciesDependencies& dependencies = speciesDependencies[dataState.GetSpecies(species)];

		dependencies.partOfReactionKineticLaw.emplace_back(reaction);
	}

	//Resolve parameter -> kinetic law dependencies
	reaction->GetKineticLaw().GetInvolvedParameters(kineticLawDependencies, true);
	for (const std::string& dependentParam : kineticLawDependencies)
	{
		ParameterDependencies& dependencies = parameterDependencies[dataState.GetParameter(dependentParam)];

		dependencies.partOfReactionKineticLaw.emplace_back(reaction);
	}

	//Resolve equation -> kinetic law dependencies
	reaction->GetKineticLaw().GetInvolvedEquations(kineticLawDependencies, true);
	for (const std::string& dependentEquation : kineticLawDependencies)
	{
		ParameterDependencies& dependencies = equationDependencies[dataState.GetEquation(dependentEquation)];

		dependencies.partOfReactionKineticLaw.emplace_back(reaction);
	}
}

void ECellEngine::Data::DependenciesDatabase::RefreshDependencies(const DataState& dataState) noexcept
{
	speciesDependencies.clear();
	parameterDependencies.clear();
	equationDependencies.clear();

	//Refresh reaction dependencies (products, reactants and kinetic laws)
	for (auto [reactionName, reaction] : dataState.GetReactions())
	{
		RefreshReactionDependencies(dataState, reaction);
	}

	//Refresh equations dependencies
	std::vector<std::string> computedParamInvolvedSpecies;
	for (auto [computedParamName, computedParam] : dataState.GetEquations())
	{
		RefreshEquationDependencies(dataState, computedParam);
	}
}