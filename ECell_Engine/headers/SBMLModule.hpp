#pragma once

#include <vector>
#include <unordered_map>

#include "BiochemicalModule.hpp"
#include "DataState.hpp"
#include "SimpleParameter.hpp"
#include "ComputedParameter.hpp"

namespace ECellEngine::Data
{
	class SBMLModule : public BiochemicalModule
	{
	private:
		std::vector<std::shared_ptr<ComputedParameter>> computedParameters;
		std::unordered_map<std::string, std::vector<std::size_t>> parameterDependancies;

	public:
		SBMLModule(DataState* _dataState) :
			BiochemicalModule(_dataState)
		{
			
		}

		inline const std::shared_ptr<ComputedParameter> GetComputedParameter(const std::size_t& _idx) const noexcept
		{
			return computedParameters[_idx];
		}

		inline virtual const std::shared_ptr<Reaction> GetReaction(const std::size_t& _idx) const noexcept override
		{
			return reactions[_idx];
		}

		inline virtual const std::shared_ptr<SimpleParameter> GetSimpleParameter(const std::size_t& _idx) const noexcept override
		{
			return simpleParameters[_idx];
		}

		inline virtual const std::shared_ptr<Species> GetSpecies(const std::size_t& _idx) const noexcept override
		{
			return species[_idx];
		}

		inline std::shared_ptr<ComputedParameter> AddComputedParameters(const std::string& _name, const Operation& _paramOp)
		{
			dataState->AddParameter(_name, _paramOp.Get(*dataState));
			computedParameters.push_back(std::make_shared<ComputedParameter>(_name, _paramOp));
			return computedParameters.back();
		}

		inline void ResizeComputedParameters(std::size_t _size)
		{
			computedParameters.resize(_size);
		}

		void ComputeDependantParameters(const std::string& _parentParameterName);

	};
}