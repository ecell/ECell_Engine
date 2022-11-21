#pragma once

#include <cstddef>
#include <vector>

#include "Module.hpp"
#include "Parameter.hpp"
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

		inline virtual const bool& CheckSolverType(const ECellEngine::Solvers::SolverTypes& _solverType) noexcept override
		{
			return _solverType == ECellEngine::Solvers::SolverTypes::Biochemical;
		}

	};
}