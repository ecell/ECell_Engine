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

		std::vector<std::size_t> products;
		std::vector<std::size_t> reactants;

		float kineticLawValueCache = 0;
		Operation kineticLaw;

	public:

		Reaction(const char* _name, const std::size_t _id,
				 const std::vector<std::size_t> _products,
				 const std::vector<std::size_t> _reactants,
				 const Operation _kineticLaw):
			id{_id}, products{_products}, reactants{_reactants}, kineticLaw{_kineticLaw}
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

		inline const float ComputeKineticLaw() noexcept
		{
			kineticLawValueCache = kineticLaw.Get();
			return kineticLawValueCache;
		}
	};
}