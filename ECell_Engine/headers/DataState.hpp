#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ComputedParameter.hpp"
#include "Reaction.hpp"
#include "SimpleParameter.hpp"
#include "Species.hpp"

namespace ECellEngine::Data
{
	struct DataState
	{
	private:
		float elapsedTime;
		std::unordered_map<std::string, std::shared_ptr<Reaction>> reactions;
		std::unordered_map<std::string, std::shared_ptr<ComputedParameter>> computedParameters;
		std::unordered_map<std::string, std::shared_ptr<SimpleParameter>> simpleParameters;
		std::unordered_map<std::string, std::shared_ptr<Species>> species;

		std::unordered_multimap<std::string, std::string> operandsToOperations;

	public:
		DataState()
		{

		}

		inline float GetElapsedTime() const
		{
			return elapsedTime;
		}

		Operand* GetOperand(const std::string& _name);

		inline std::unordered_multimap<std::string, std::string>& GetOperandsToOperations()
		{
			return operandsToOperations;
		}

		inline Reaction* GetReaction(const std::string& _reactionName)
		{
			return reactions.at(_reactionName).get();
		}

		inline ComputedParameter* GetComputedParameter(const std::string& _parameterName)
		{
			return computedParameters.at(_parameterName).get();
		}

		inline SimpleParameter* GetSimpleParameter(const std::string& _parameterName)
		{
			return simpleParameters.at(_parameterName).get();
		}

		inline Species* GetSpecies(const std::string& _speciesName)
		{
			return species.at(_speciesName).get();
		}

		inline bool AddReaction(const std::string& _reactionName,
								const std::vector<std::string> _products,
								const std::vector<std::string> _reactants,
								const Operation _kineticLaw)
		{
			return reactions.emplace(_reactionName, std::make_shared<Reaction>(_reactionName, _products, _reactants, _kineticLaw)).second;
		}
		
		inline bool AddComputedParameter(const std::string& _parameterName, const Operation _parameterOp)
		{
			return computedParameters.emplace(_parameterName, std::make_shared<ComputedParameter>(_parameterName, _parameterOp)).second;
		}

		inline bool AddSimpleParameter(const std::string& _parameterName, const float _value)
		{
			return simpleParameters.emplace(_parameterName, std::make_shared<SimpleParameter>(_parameterName, _value)).second;
		}
		
		inline bool AddSpecies(const std::string& _speciesName, const float _quantity)
		{
			return species.emplace(_speciesName, std::make_shared<Species>(_speciesName, _quantity)).second;
		}

		inline void SetElapsedTime(const float _elapsedTime)
		{
			elapsedTime = _elapsedTime;
		}

		void ClearReactions(const std::vector<std::string>& _reactionNames);
		
		void ClearComputedParameters(const std::vector<std::string>& _parameterNames);
		
		void ClearSimpleParameters(const std::vector<std::string>& _parameterNames);
		
		void ClearSpecies(const std::vector<std::string>& _speciesNames);

		inline void LinkOperandToOperation(const std::string& _operandName, const std::string& _operationName)
		{
			operandsToOperations.emplace(_operandName, _operationName);
		}
	};
}