#include "dependencies/DependenciesDatabase.hpp"

void ECellEngine::Data::DependenciesDatabase::RefreshSpeciesComputedParameterDependencies(const DataState& dataState, std::shared_ptr<ComputedParameter> computerParameter) noexcept
{
	//TODO
}

void ECellEngine::Data::DependenciesDatabase::RefreshSpeciesReactionDependencies(const DataState& dataState, std::shared_ptr<Reaction> reaction)  noexcept
{
	//1) iterates over reaction reactants and build relevant dependencies
	for (const std::string& reactant : *reaction->GetReactants())
	{
		//Might have duplicatas here, must use another datastructure to make sure we don't add duplicatas (std::unordered_set or ordered std::vector)
		SpeciesDependencies& dependencies = speciesDependencies[dataState.GetSpecies(reactant)];

		dependencies.partOfReactionAsReactant.emplace_back(reaction);
	}

	//2) iterates over reaction products and build relevant dependencies
	for (const std::string& product : *reaction->GetProducts())
	{
		//Might have duplicatas here, must use another datastructure to make sure we don't add duplicatas (std::unordered_set or ordered std::vector)
		SpeciesDependencies& dependencies = speciesDependencies[dataState.GetSpecies(product)];

		dependencies.partOfReactionAsProduct.emplace_back(reaction);
	}

	//3) search dependencies in reaction kinetic law and build relevant dependencies
	std::vector<std::string> kineticLawInvolvedSpecies;
	reaction->GetKineticLaw().GetInvolvedSpecies(kineticLawInvolvedSpecies, true);

	for (const std::string& species : kineticLawInvolvedSpecies)
	{
		SpeciesDependencies& dependencies = speciesDependencies[dataState.GetSpecies(species)];

		dependencies.partOfReactionKineticLaw.emplace_back(reaction);
	}
}

void ECellEngine::Data::DependenciesDatabase::RefreshDependencies(const DataState& dataState) noexcept
{
	//Refresh reaction dependencies (products, reactants and kinetic laws)
	for (auto [reactionName, reaction] : dataState.GetReactions())
	{
		RefreshSpeciesReactionDependencies(dataState, reaction);
	}

	//Refresh computed parameters dependencies
	std::vector<std::string> computedParamInvolvedSpecies;
	for (auto [computedParamName, computedParam] : dataState.GetComputedParameters())
	{
		computedParam->GetInvolvedSpecies(computedParamInvolvedSpecies, true);
	}
}