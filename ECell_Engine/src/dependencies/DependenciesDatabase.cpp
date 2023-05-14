#include "dependencies/DependenciesDatabase.hpp"

void ECellEngine::Data::DependenciesDatabase::RefreshComputedParameterDependencies(const DataState& dataState, std::shared_ptr<ComputedParameter> computedParameter) noexcept
{
	std::vector<std::string> dependencies;

	//Resolve species -> computed parameter dependencies
	computedParameter->GetInvolvedSpecies(dependencies, true);
	for (const std::string& species : dependencies)
	{
		SpeciesDependencies& dependencies = speciesDependencies[dataState.GetSpecies(species)];

		dependencies.partOfComputedParameter.emplace_back(computedParameter);
	}

	//Resolve simple parameter -> computed parameter dependencies
	computedParameter->GetInvolvedSimpleParameters(dependencies, true);
	for (const std::string& dependentSimpleParam : dependencies)
	{
		ParameterDependencies& dependencies = simpleParameterDependencies[dataState.GetSimpleParameter(dependentSimpleParam)];

		dependencies.partOfComputedParameter.emplace_back(computedParameter);
	}

	//Resolve computed parameter -> computed parameter dependencies
	computedParameter->GetInvolvedComputedParameters(dependencies, true);
	for (const std::string& dependentComputedParam : dependencies)
	{
		ParameterDependencies& dependencies = computedParameterDependencies[dataState.GetComputedParameter(dependentComputedParam)];

		dependencies.partOfComputedParameter.emplace_back(computedParameter);
	}
}

void ECellEngine::Data::DependenciesDatabase::RefreshReactionDependencies(const DataState& dataState, std::shared_ptr<Reaction> reaction)  noexcept
{
	//Resolve species (as reactant) -> reaction dependencies
	for (const std::string& reactant : *reaction->GetReactants())
	{
		//Might have duplicatas here, must use another datastructure to make sure we don't add duplicatas (std::unordered_set or ordered std::vector)
		SpeciesDependencies& dependencies = speciesDependencies[dataState.GetSpecies(reactant)];

		dependencies.partOfReactionAsReactant.emplace_back(reaction);
	}

	//Resolve species (as product) -> reaction dependencies
	for (const std::string& product : *reaction->GetProducts())
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

	//Resolve simple parameter -> kinetic law dependencies
	reaction->GetKineticLaw().GetInvolvedSimpleParameters(kineticLawDependencies, true);
	for (const std::string& dependentSimpleParam : kineticLawDependencies)
	{
		ParameterDependencies& dependencies = simpleParameterDependencies[dataState.GetSimpleParameter(dependentSimpleParam)];

		dependencies.partOfReactionKineticLaw.emplace_back(reaction);
	}

	//Resolve computed parameter -> kinetic law dependencies
	reaction->GetKineticLaw().GetInvolvedComputedParameters(kineticLawDependencies, true);
	for (const std::string& dependentComputedParam : kineticLawDependencies)
	{
		ParameterDependencies& dependencies = computedParameterDependencies[dataState.GetComputedParameter(dependentComputedParam)];

		dependencies.partOfReactionKineticLaw.emplace_back(reaction);
	}
}

void ECellEngine::Data::DependenciesDatabase::RefreshDependencies(const DataState& dataState) noexcept
{
	speciesDependencies.clear();
	simpleParameterDependencies.clear();
	computedParameterDependencies.clear();

	//Refresh reaction dependencies (products, reactants and kinetic laws)
	for (auto [reactionName, reaction] : dataState.GetReactions())
	{
		RefreshReactionDependencies(dataState, reaction);
	}

	//Refresh computed parameters dependencies
	std::vector<std::string> computedParamInvolvedSpecies;
	for (auto [computedParamName, computedParam] : dataState.GetComputedParameters())
	{
		RefreshComputedParameterDependencies(dataState, computedParam);
	}
}