#pragma once

#include <memory>
#include <string>
#include <vector>

#include "DataState.hpp"
#include "Operation.hpp"
#include "Species.hpp"

using namespace ECellEngine::Maths;

namespace ECellEngine::Data
{
	struct Reaction
	{
	private:
		std::string name;
		std::vector<std::shared_ptr<Species>> products;
		std::vector<std::shared_ptr<Species>> reactants;
		Operation kineticLaw;

	public:
		Reaction(std::string _name,
				std::vector<std::shared_ptr<Species>> _products,
				std::vector<std::shared_ptr<Species>> _reactants,
				Operation _kineticLaw):
			name{_name}, products{_products}, reactants{_reactants}, kineticLaw{_kineticLaw}
		{

		}

		inline void ComputeKineticLaw(DataState& _dataState)
		{
			_dataState.SetKineticLaw(name, kineticLaw.Get(_dataState));
		}

		inline const float& GetKineticLaw(const DataState& _dataState) const noexcept
		{
			_dataState.GetKineticLaw(name);
		}
	};
}