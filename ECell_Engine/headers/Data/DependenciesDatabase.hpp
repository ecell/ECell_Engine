#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "Data/Reaction.hpp"
#include "Data/DataState.hpp"
#include "Data/SimpleParameter.hpp"
#include "Data/ComputedParameter.hpp"

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
		struct SpeciesDependencies
		{
			/*!
			@brief Collection of reactions that contain the linked species in their list of reactants.
			*/
			std::vector<std::weak_ptr<Reaction>>			partOfReactionAsReactant;

			/*!
			@brief Collection of reactions that contain the linked species in their list of products.
			*/
			std::vector<std::weak_ptr<Reaction>>			partOfReactionAsProduct;

			/*!
			@brief Collection of reactions that contain the linked species in their kinetic law.
			*/
			std::vector<std::weak_ptr<Reaction>>			partOfReactionKineticLaw;

			/*!
			@brief Collection of computed parameters that contain the linked species.
			*/
			std::vector<std::weak_ptr<ComputedParameter>>	partOfComputedParameter;
		};

		struct ParameterDependencies
		{
			/*!
			@brief Collection of reactions that contain the linked parameter in their kinetic law.
			*/
			std::vector<std::weak_ptr<Reaction>>			partOfReactionKineticLaw;

			/*!
			@brief Collection of computed parameters that contain the linked parameter in their internal operands.
			*/
			std::vector<std::weak_ptr<ComputedParameter>>	partOfComputedParameter;
		};

		/*!
		@brief Collection associating a species to all its tracked dependencies (see SpeciesDependencies).
		*/
		std::unordered_map<std::shared_ptr<Species>, SpeciesDependencies>				speciesDependencies;

		/*!
		@brief Collection associating a simple parameter to all its tracked
				dependencies (see ParameterDependencies).
		*/
		std::unordered_map<std::shared_ptr<SimpleParameter>, ParameterDependencies>		simpleParameterDependencies;

		/*!
		@brief Collection associating a computed parameter to all its tracked
				dependencies (see ParameterDependencies).
		*/
		std::unordered_map<std::shared_ptr<ComputedParameter>, ParameterDependencies>	computedParameterDependencies;

		/*!
		@brief Explore content of a computed parameter and fills the species and
				parameter (both simple and computed) dependencies.

		@param dataState The data state.
		@param computedParameter The target computed parameter.
		*/
		void RefreshComputedParameterDependencies(const DataState& dataState, std::shared_ptr<ComputedParameter> computedParameter) noexcept;

		/*!
		@brief Explore the @p reaction's reactants, products and kinetic law to fill
				the species, and parameters dependencies to this @p reaction.

		@param dataState The data state.
		@param reaction The explored reaction.
		*/
		void RefreshReactionDependencies(const DataState& dataState, std::shared_ptr<Reaction> reaction)  noexcept;

	public:

		/*!
		@brief Gets the reference to ::speciesDependencies.
		*/
		inline const std::unordered_map<std::shared_ptr<Species>, SpeciesDependencies>& GetSpeciesDependencies() const noexcept
		{
			return speciesDependencies;
		}

		/*!
		@brief Gets the reference to ::simpleParameterDependencies.
		*/
		inline const std::unordered_map<std::shared_ptr<SimpleParameter>, ParameterDependencies>& GetSimpleParameterDependencies() const noexcept
		{
			return simpleParameterDependencies;
		}

		/*!
		@brief Gets the reference to ::computedParameterDependencies.
		*/
		inline const std::unordered_map<std::shared_ptr<ComputedParameter>, ParameterDependencies>& GetComputedParameterDependencies() const noexcept
		{
			return computedParameterDependencies;
		}

		void RefreshDependencies(const DataState& dataState) noexcept;
	};
}