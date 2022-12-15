#pragma once

#include <cstddef>
#include <memory>
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
		std::vector<std::shared_ptr<Reaction>> reactions;
		std::vector<std::shared_ptr<SimpleParameter>> simpleParameters;
		std::vector<std::shared_ptr<Species>> species;

	public:
		BiochemicalModule(DataState* _dataState) :
			Module(_dataState)
		{

		}

		virtual const std::shared_ptr<Reaction> GetReaction(const std::size_t& _idx) const noexcept = 0;

		virtual const std::shared_ptr<SimpleParameter> GetSimpleParameter(const std::size_t& _idx) const noexcept = 0;

		virtual const std::shared_ptr<Species> GetSpecies(const std::size_t& _idx) const noexcept = 0;


		inline void AddReaction(const std::string& _name,
								const std::vector<std::shared_ptr<Species>>& _products,
								const std::vector<std::shared_ptr<Species>>& _reactants,
								const Operation& _kineticLaw)
		{
			dataState->AddKineticLaw(_name, _kineticLaw.Get(*dataState));
			reactions.emplace_back(std::make_shared<Reaction>(_name, _products, _reactants, _kineticLaw));
		}

		inline void AddSimpleParameter(const std::string& _name, const float& _value)
		{
			dataState->AddParameter(_name, _value);
			simpleParameters.emplace_back(std::make_shared<SimpleParameter>(_name));
		}

		inline void AddSpecies(const std::string& _name, const float& _value)
		{
			dataState->AddSpecies(_name, _value);
			species.emplace_back(std::make_shared<Species>(_name));
		}

		inline virtual bool IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept override
		{
			return dynamic_cast<const ECellEngine::Solvers::BiochemicalSolver*>(_solver) != nullptr;
		}

		inline void ResizeReactions(std::size_t _size)
		{
			reactions.resize(_size);
		}
		
		inline void ResizeSimpleParameters(std::size_t _size)
		{
			simpleParameters.resize(_size);
		}
		
		inline void ResizeSpecies(std::size_t _size)
		{
			species.resize(_size);
		}
	};
}