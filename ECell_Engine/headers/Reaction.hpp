#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Operation.hpp"

using namespace ECellEngine::Maths;

namespace ECellEngine::Data
{
	struct Reaction
	{
	private:
		const std::string name;
		const std::vector<std::string> products;
		const std::vector<std::string> reactants;
		float kineticLawValueCache;
		Operation kineticLaw;

	public:
		Reaction(const std::string _name,
				 const std::vector<std::string> _products,
				 const std::vector<std::string> _reactants,
				 const Operation _kineticLaw):
			name{_name}, products{_products}, reactants{_reactants}, kineticLaw{_kineticLaw}
		{

		}

		inline const float ComputeKineticLaw() noexcept
		{
			kineticLawValueCache = kineticLaw.Get();
			return kineticLawValueCache;
		}

		inline const Operation& GetKineticLaw() noexcept
		{
			return kineticLaw;
		}

		inline const float GetKineticLawValue() const noexcept
		{
			return kineticLawValueCache;
		}

		inline const std::vector<std::string>& GetProducts() const noexcept
		{
			return products;
		}
		
		inline const std::vector<std::string>& GetReactants() const noexcept
		{
			return reactants;
		}
	};
}