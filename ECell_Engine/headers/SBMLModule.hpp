#pragma once

#include <vector>
#include <unordered_map>

#include "BiochemicalModule.hpp"
#include "DataState.hpp"
#include "Operand.hpp"
#include "Parameter.hpp"

namespace ECellEngine::Data
{
	class SBMLModule : public BiochemicalModule
	{
	private:
		std::vector<ComputedParameter> computedParameters;
		std::unordered_map<std::string, std::vector<std::size_t>> parameterDependancies;

	public:
		SBMLModule(DataState* _dataState) :
			BiochemicalModule(_dataState)
		{
			
		}

		inline const ComputedParameter* GetComputedParameter(const std::size_t& _idx) const noexcept
		{
			return &computedParameters[_idx];
		}

		inline virtual const Reaction* GetReaction(const std::size_t& _idx) const noexcept override
		{
			return &reactions[_idx];
		}

		inline virtual const SimpleParameter* GetSimpleParameter(const std::size_t& _idx) const noexcept override
		{
			return &simpleParameters[_idx];
		}

		inline virtual const Species* GetSpecies(const std::size_t& _idx) const noexcept override
		{
			return &species[_idx];
		}

		inline void AddComputedParameters(const std::string& _name, const Operation& _paramOp)
		{
			dataState->AddParameter(_name, _paramOp.Get(*dataState));
			computedParameters.emplace_back(ComputedParameter(_name, _paramOp));
		}

		void ComputeDependantParameters(const std::string& _parentParameterName);

	};
}