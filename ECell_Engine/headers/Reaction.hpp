#pragma once

#include <string>
#include <vector>

#include "DataState.hpp"
#include "Operand.hpp"
#include "Species.hpp"

using namespace ECellEngine::Maths;

namespace ECellEngine::Data
{
	struct Reaction
	{
	private:
		std::string name;
		std::vector<Species*> products;
		std::vector<Species*> reactants;
		Operation kineticLaw;

	public:
		Reaction(std::string _name):
			name{_name}
		{

		}

		inline void AddProduct(const Species* _product)
		{
			products.emplace_back(_product);
		}

		inline void AddReactant(const Species* _reactant)
		{
			products.emplace_back(_reactant);
		}

		void ApplyBackward(DataState& _dataState);

		void ApplyForward(DataState& _dataState);

		inline float ComputeKineticLaw(const DataState& _dataState)
		{
			return kineticLaw.Get(_dataState);
		}

	};
}