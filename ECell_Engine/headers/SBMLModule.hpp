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

		inline const std::vector<std::shared_ptr<ComputedParameter>>& GetAllComputedParameter() const noexcept
		{
			return computedParameters;
		}

		inline const std::shared_ptr<ComputedParameter>& GetComputedParameter(const std::size_t& _idx) const noexcept
		{
			return computedParameters[_idx];
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