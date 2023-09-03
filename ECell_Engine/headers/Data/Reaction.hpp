#pragma once

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
		std::size_t id;

		std::vector<std::size_t> products;
		std::vector<std::size_t> reactants;

		float kineticLawValueCache = 0;
		Operation kineticLaw;

	public:
		const std::string name;

		Reaction(const std::string _name, const std::size_t _id,
				 const std::vector<std::size_t> _products,
				 const std::vector<std::size_t> _reactants,
				 const Operation _kineticLaw):
			name{_name}, products{_products}, reactants{_reactants}, kineticLaw{_kineticLaw}
		{

		}

		inline const std::size_t GetID() const noexcept
		{
			return id;
		}

		inline const float ComputeKineticLaw() noexcept
		{
			kineticLawValueCache = kineticLaw.Get();
			return kineticLawValueCache;
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
	};
}