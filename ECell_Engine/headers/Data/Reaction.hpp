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
		char name[64] = { '0' };

		std::size_t id;

		std::vector<std::shared_ptr<std::function<void(Operand*)>>> onProductDestroySubTokens;
		std::vector<std::size_t> products;

		std::vector<std::shared_ptr<std::function<void(Operand*)>>> onReactantDestroySubTokens;
		std::vector<std::size_t> reactants;

		float kineticLawValueCache = 0;
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

		inline const std::size_t GetID() const noexcept
		{
			return id;
		}

		inline char* GetName() noexcept
		{
			return name;
		}


		inline Operation& GetKineticLaw() noexcept
		{
			return kineticLaw;
		}

		inline const float GetKineticLawValue() const noexcept
		{
			return kineticLawValueCache;
		}

		inline const std::vector<std::size_t>& GetProducts() const noexcept
		{
			return products;
		}
		
		inline const std::vector<std::size_t>& GetReactants() const noexcept
		{
			return reactants;
		}

		inline void SetName(const char* _name) noexcept
		{
			Util::StrCopy(name, _name, sizeof(name));
		}
		
		void AddProduct(Operand* _sp);

		void AddReactant(Operand* _sp);

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

		void OnProductDestroy(Operand* _sp);

		void OnReactantDestroy(Operand* _sp);
	};
}