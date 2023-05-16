#pragma once

#include <vector>

#include "BiochemicalSolver.hpp"
#include "Module.hpp"

namespace ECellEngine::Data
{
	class BiochemicalModule : public Module
	{
	protected:
		std::vector<std::string> reactions;
		std::vector<std::string> simpleParameters;
		std::vector<std::string> species;

	public:
		BiochemicalModule(DataState& _dataState, char* _name = "NewBioChemicalAsset") :
			Module(_dataState, _name)
		{

		}

		inline const std::vector<std::string>& GetAllReaction() const noexcept
		{
			return reactions;
		}

		inline const std::vector<std::string>& GetAllSimpleParameter () const noexcept
		{
			return simpleParameters;
		}

		inline const std::vector<std::string>& GetAllSpecies() const noexcept
		{
			return species;
		}

		inline const std::string& GetReaction(const std::size_t& _idx) const noexcept
		{
			return reactions[_idx];
		}

		inline const std::string& GetSimpleParameter(const std::size_t& _idx) const noexcept
		{
			return simpleParameters[_idx];
		}

		inline const std::string& GetSpecies(const std::size_t& _idx) const noexcept
		{
			return species[_idx];
		}

		inline void AddReaction(const std::string _reactionName,
								const std::vector<std::string> _products,
								const std::vector<std::string> _reactants,
								const Operation _kineticLaw)
		{
			if (dataState.AddReaction(_reactionName, _products, _reactants, _kineticLaw))
			{
				reactions.push_back(_reactionName);
			}
		}

		inline void AddSimpleParameter(const std::string _parameterName, const float _value)
		{
			if (dataState.AddSimpleParameter(_parameterName, _value))
			{
				simpleParameters.push_back(_parameterName);
			}
		}

		inline void AddSpecies(const std::string _speciesName, const float _quantity)
		{
			if (dataState.AddSpecies(_speciesName, _quantity))
			{
				species.push_back(_speciesName);
			}
		}

		inline virtual bool IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept override
		{
			return dynamic_cast<const ECellEngine::Solvers::BiochemicalSolver*>(_solver) != nullptr;
		}
	};
}