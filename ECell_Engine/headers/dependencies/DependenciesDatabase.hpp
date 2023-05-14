#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "Reaction.hpp"
#include "DataState.hpp"
#include "ComputedParameter.hpp"

#include <string>

namespace std
{
	template <>
	struct hash<ECellEngine::Data::Reaction>
	{
		std::size_t operator()(const ECellEngine::Data::Reaction& k) const
		{
			return std::hash<std::string>()(k.name);
		}
	};
}

namespace ECellEngine::Data
{
	/*!
	@brief Class responsible for computing and storing dependencies between all biochemical entities.
			Mostly used in the editor to represent relationships between entities.
			Might evolve later to contain dependencies between assets.
	*/
	class DependenciesDatabase
	{
	private:
		/*struct SpeciesHash
		{
			std::size_t operator()(Reaction const& reaction) const
			{
				return std::hash<std::string>()(reaction.name);
			}
		};*/

		struct SpeciesDependencies
		{
			/*!
			@brief Collection of reactions that contain a species in their list of reactants.
			*/
			std::vector<std::weak_ptr<Reaction>>			partOfReactionAsReactant;

			/*!
			@brief Collection of reactions that contain a species in their list of products.
			*/
			std::vector<std::weak_ptr<Reaction>>			partOfReactionAsProduct;

			/*!
			@brief Collection of reactions that contain a species in their kinetic law.
			*/
			std::vector<std::weak_ptr<Reaction>>			partOfReactionKineticLaw;

			/*!
			@brief Collection of computed parameters that contain a species.
			*/
			std::vector<std::weak_ptr<ComputedParameter>>	partOfComputedParameter;
		};

		/*!
		@brief Collection associating a species to all its tracked dependencies (see SpeciesDependencies).
		*/
		std::unordered_map<std::shared_ptr<Species>, SpeciesDependencies> speciesDependencies;

		/*!
		@brief Explore content of a computed parameter and fills the species dependencies.

		@param dataState The data state.
		@param computerParameter The target computed parameter.
		*/
		void RefreshSpeciesComputedParameterDependencies(const DataState& dataState, std::shared_ptr<ComputedParameter> computerParameter) noexcept;

		/*!
		@brief Iterate over the content of a reaction and progressively fills the species dependencies.

		@param dataState The data state.
		@param reaction The iterated reaction.
		*/
		void RefreshSpeciesReactionDependencies(const DataState& dataState, std::shared_ptr<Reaction> reaction)  noexcept;

	public:
		void RefreshDependencies(const DataState& dataState)  noexcept;
	};
}