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

		inline const std::vector<std::shared_ptr<Reaction>>& GetAllReaction() const noexcept
		{
			return reactions;
		}

		inline const std::vector<std::shared_ptr<SimpleParameter>>& GetAllSimpleParameter () const noexcept
		{
			return simpleParameters;
		}

		inline const std::vector<std::shared_ptr<Species>>& GetAllSpecies() const noexcept
		{
			return species;
		}

		inline const std::shared_ptr<Reaction>& GetReaction(const std::size_t& _idx) const noexcept
		{
			return reactions[_idx];
		}

		inline const std::shared_ptr<SimpleParameter>& GetSimpleParameter(const std::size_t& _idx) const noexcept
		{
			return simpleParameters[_idx];
		}

		inline const std::shared_ptr<Species>& GetSpecies(const std::size_t& _idx) const noexcept
		{
			return species[_idx];
		}

		inline std::shared_ptr<Reaction> AddReaction(const std::string& _name,
													const std::vector<std::shared_ptr<Species>>& _products,
													const std::vector<std::shared_ptr<Species>>& _reactants,
													const Operation& _kineticLaw)
		{
			dataState->AddKineticLaw(_name, _kineticLaw.Get(*dataState));
			reactions.push_back(std::make_shared<Reaction>(_name, _products, _reactants, _kineticLaw));
			return reactions.back();
		}

		inline std::shared_ptr<SimpleParameter> AddSimpleParameter(const std::string& _name, const float& _value)
		{
			dataState->AddParameter(_name, _value);
			simpleParameters.push_back(std::make_shared<SimpleParameter>(_name));
			return simpleParameters.back();
		}

		inline std::shared_ptr<Species> AddSpecies(const std::string& _name, const float& _value)
		{
			dataState->AddSpecies(_name, _value);
			species.push_back(std::make_shared<Species>(_name));
			return species.back();
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