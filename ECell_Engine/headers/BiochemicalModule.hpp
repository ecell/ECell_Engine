#pragma once

#include <cstddef>
#include <vector>

#include "BiochemicalSolver.hpp"
#include "Module.hpp"
#include "SimpleParameter.hpp"
#include "Reaction.hpp"
#include "Species.hpp"

namespace ECellEngine::Data
{
	class BiochemicalModule : public Module
	{
	protected:
		std::vector<Reaction> reactions;
		std::vector<SimpleParameter> simpleParameters;
		std::vector<Species> species;

	public:
		BiochemicalModule(DataState* _dataState) :
			Module(_dataState)
		{

		}

		virtual const Reaction* GetReaction(const std::size_t& _idx) const noexcept = 0;

		virtual const SimpleParameter* GetSimpleParameter(const std::size_t& _idx) const noexcept = 0;

		virtual const Species* GetSpecies(const std::size_t& _idx) const noexcept = 0;


		inline void AddReaction(const std::string& _name,
								const std::vector<std::shared_ptr<Species>>& _products,
								const std::vector<std::shared_ptr<Species>>& _reactants,
								const Operation& _kineticLaw)
		{
			dataState->AddKineticLaw(_name, _kineticLaw.Get(*dataState));
			reactions.emplace_back(Reaction(_name, _products, _reactants, _kineticLaw));
		}

		inline void AddSimpleParameter(const std::string& _name, const float& _value)
		{
			dataState->AddParameter(_name, _value);
			simpleParameters.emplace_back(SimpleParameter(_name));
		}

		inline void AddSpecies(const std::string& _name, const float& _value)
		{
			dataState->AddSpecies(_name, _value);
			species.emplace_back(Species(_name));
		}

		inline virtual bool IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept override
		{
			return dynamic_cast<const ECellEngine::Solvers::BiochemicalSolver*>(_solver) != nullptr;
		}
	};
}