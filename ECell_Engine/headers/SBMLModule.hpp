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
		std::unordered_map<std::string, std::vector<int>> parameterDependancies;

	public:
		SBMLModule(DataState* _dataState) :
			BiochemicalModule(_dataState)
		{
			
		}

		inline const ComputedParameter* GetComputedParameter(const int& _idx) const noexcept
		{
			return &computedParameters[_idx];
		}

		inline virtual const Reaction* GetReaction(const int& _idx) const noexcept override
		{
			return &reactions[_idx];
		}

		inline virtual const SimpleParameter* GetSimpleParameter(const int& _idx) const noexcept override
		{
			return &simpleParameters[_idx];
		}

		inline virtual const Species* GetSpecies(const int& _idx) const noexcept override
		{
			return &species[_idx];
		}

		void ComputeDependantParameters(const std::string& _parentParemeterName);

		virtual void Initialize() override;
		
	};
}