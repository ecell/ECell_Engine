#pragma once

#include <vector>
#include <unordered_map>

#include "BiochemicalModule.hpp"

namespace ECellEngine::Data
{
	class SBMLModule : public BiochemicalModule
	{
	private:
		std::vector<std::string> computedParameters;
	public:
		SBMLModule(DataState& _dataState) :
			BiochemicalModule(_dataState)
		{
			
		}

		inline const std::vector<std::string>& GetAllComputedParameter() const noexcept
		{
			return computedParameters;
		}

		inline const std::string& GetComputedParameter(const std::size_t& _idx) const noexcept
		{
			return computedParameters[_idx];
		}

		inline void AddComputedParameters(const std::string _parameterName, const Operation _parameterOp)
		{
			if (dataState.AddComputedParameter(_parameterName, _parameterOp))
			{
				computedParameters.push_back(_parameterName);
				dataState.LinkParametersWithComputedParameters(_parameterName);
			}
		}

		inline void ResizeComputedParameters(std::size_t _size)
		{
			computedParameters.resize(_size);
		}

	};
}