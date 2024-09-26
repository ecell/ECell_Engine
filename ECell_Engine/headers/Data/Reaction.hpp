#pragma once

/*!
@file Reaction.hpp
@brief Defines a reaction with a set of products, reactants and a kinetic law.
*/

#include <memory>
#include <string>
#include <vector>

#include "Maths/Operation.hpp"

using namespace ECellEngine::Maths;

namespace ECellEngine::Data
{
	struct Reaction
	{
	private:

		/*!
		@brief The name of the reaction.
		*/
		char name[64] = { '0' };

		/*!
		@brief The ID of the reaction.
		*/
		std::size_t id;

		/*!
		@brief A list of tokens that will keep the reaction informed of the destruction
				of species so that it can remove them from its local list of products.
		@see ::OnProductDestroy, ECellEngine::Core::Callback
		*/
		std::vector<std::shared_ptr<std::function<void(Operand*)>>> onProductDestroySubTokens;

		/*!
		@brief A list of the IDs of the species that are products of this reaction.
		@details The IDs are the keys to the unordered map of species in the data state.
		@see ECellEngine::Data::DataState::species
		*/
		std::vector<std::size_t> products;

		/*!
		@brief A list of tokens that will keep the reaction informed of the destruction
				of species so that it can remove them from its local list of reactants.
		@see ::OnReactantDestroy, ECellEngine::Core::Callback
		*/
		std::vector<std::shared_ptr<std::function<void(Operand*)>>> onReactantDestroySubTokens;

		/*!
		@brief A list of the IDs of the species that are reactants of this reaction.
		@details The IDs are the keys to the unordered map of species in the data state.
		@see ECellEngine::Data::DataState::species
		*/
		std::vector<std::size_t> reactants;

		/*!
		@brief The value of the computation of the ::kineticLaw.
		@details This value is cached to avoid recomputation of the kinetic law.
		@see GetKineticLawValue()
		*/
		float kineticLawValueCache = 0;

		/*!
		@brief The mathematical operation that describes the kinetic law of the reaction.
		*/
		Operation kineticLaw;

	public:
		/*!
		@brief Callback to be called when the reaction is destroyed.
		*/
		Core::Callback<Reaction*> onDestroy;

		Reaction(const char* _name, const std::size_t _id,
				 const Operation _kineticLaw):
			id{_id}, kineticLaw{_kineticLaw}
		{
			Util::StrCopy(name, _name, sizeof(name));
		}

		/*!
		@brief Returns the ID of the reaction.
		@returns ::id
		*/
		inline const std::size_t GetID() const noexcept
		{
			return id;
		}

		/*!
		@brief Returns the name of the reaction.
		@returns ::name
		*/
		inline char* GetName() noexcept
		{
			return name;
		}

		/*!
		@brief Returns the kinetic law of the reaction.
		@returns ::kineticLaw
		*/
		inline Operation& GetKineticLaw() noexcept
		{
			return kineticLaw;
		}

		/*!
		@brief Returns the value of the kinetic law of the reaction.
		@returns ::kineticLawValueCache
		*/
		inline const float GetKineticLawValue() const noexcept
		{
			return kineticLawValueCache;
		}

		/*!
		@brief Returns the list of products of the reaction.
		@details The list contains the IDs of the species that are products of the reaction.
		@return ::products
		*/
		inline const std::vector<std::size_t>& GetProducts() const noexcept
		{
			return products;
		}
		
		/*!
		@brief Returns the list of reactants of the reaction.
		@details The list contains the IDs of the species that are reactants of the reaction.
		@return ::reactants
		*/
		inline const std::vector<std::size_t>& GetReactants() const noexcept
		{
			return reactants;
		}

		/*!
		@brief Sets the name of the reaction.
		@param _name The new name of the reaction.
		*/
		inline void SetName(const char* _name) noexcept
		{
			Util::StrCopy(name, _name, sizeof(name));
		}
		
		/*!
		@brief Adds a product to the reaction.
		@details Note that we are using the pointer to the base class Operand
				 instead of the pointer to the derived class Species. Be careful
				 not to pass a pointer to a different type of operand.
		@param _sp The pointer to the species that is a product of the reaction.
		*/
		void AddProduct(Operand* _sp);

		/*!
		@brief Adds a reactant to the reaction.
		@details Note that we are using the pointer to the base class Operand
				 instead of the pointer to the derived class Species. Be careful
				 not to pass a pointer to a different type of operand.
		@param _sp The pointer to the species that is a reactant of the reaction.
		*/
		void AddReactant(Operand* _sp);

		/*!
		@brief Computes the kinetic law of the reaction.
		@details The value is cached in ::kineticLawValueCache.
		@returns The result of the computation.
		*/
		inline const float ComputeKineticLaw() noexcept
		{
			kineticLawValueCache = kineticLaw.Get();
			return kineticLawValueCache;
		}

		/*!
		@brief A wrapper around the onDestroy callbacks
				for this reaction and its kinetic law.
		*/
		inline void OnDestroy() noexcept
		{
			onDestroy(this);
			kineticLaw.onDestroy((ECellEngine::Maths::Operand*)&kineticLaw);
		}

		/*!
		@brief Callback function to be called when a product is destroyed in the
				data state so that we can remove it from the local list of products
				of this reaction.
		@param _sp The pointer to the species that is being destroyed.
		@see ::onProductDestroySubTokens.
		*/
		void OnProductDestroy(Operand* _sp);

		/*!
		@brief Callback function to be called when a reactant is destroyed in the
				data state so that we can remove it from the local list of reactants
				of this reaction.
		@param _sp The pointer to the species that is being destroyed.
		@see ::onReactantDestroySubTokens.
		*/
		void OnReactantDestroy(Operand* _sp);
	};
}