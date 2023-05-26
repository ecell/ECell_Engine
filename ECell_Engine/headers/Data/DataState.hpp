#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Data/Reaction.hpp"
#include "Data/Parameter.hpp"
#include "Data/Species.hpp"
#include "Maths/Equation.hpp"

namespace ECellEngine::Data
{
	struct DataState
	{
	private:
		float elapsedTime;
		std::unordered_map<std::string, std::shared_ptr<ECellEngine::Maths::Equation>> equations;
		std::unordered_map<std::string, std::shared_ptr<Reaction>> reactions;
		std::unordered_map<std::string, std::shared_ptr<Parameter>> parameters;
		std::unordered_map<std::string, std::shared_ptr<Species>> species;

		std::unordered_multimap<std::string, std::string> operandsToOperations;

	public:
		DataState()
		{

		}


		inline std::shared_ptr<Equation> GetEquation(const std::string& _equation) const
		{
			return equations.at(_equation);
		}

		inline const std::unordered_map<std::string, std::shared_ptr<Equation>>& GetEquations() const
		{
			return equations;
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

		inline std::shared_ptr<Reaction> GetReaction(const std::string& _reactionName) const
		{
			return reactions.at(_reactionName);
		}

		inline const std::unordered_map<std::string, std::shared_ptr<Reaction>>& GetReactions() const
		{
			return reactions;
		}

		inline std::shared_ptr<Parameter> GetParameter(const std::string& _parameterName) const
		{
			return parameters.at(_parameterName);
		}

		inline const std::unordered_map<std::string, std::shared_ptr<Parameter>>& GetParameters() const
		{
			return parameters;
		}

		inline std::shared_ptr<Species> GetSpecies(const std::string& _speciesName) const
		{
			return species.at(_speciesName);
		}

		inline const std::unordered_map<std::string, std::shared_ptr<Species>>& GetAllSpecies() const
		{
			return species;
		}

		inline bool AddReaction(const std::string& _reactionName,
								const std::vector<std::string> _products,
								const std::vector<std::string> _reactants,
								const Operation _kineticLaw)
		{
			return reactions.emplace(_reactionName, std::make_shared<Reaction>(_reactionName, _products, _reactants, _kineticLaw)).second;
		}
		
		inline bool AddEquation(const std::string& _equation, const Operation _parameterOp)
		{
			return equations.emplace(_equation, std::make_shared<Equation>(_equation, _parameterOp)).second;
		}

		inline bool AddParameter(const std::string& _parameterName, const float _value)
		{
			return parameters.emplace(_parameterName, std::make_shared<Parameter>(_parameterName, _value)).second;
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
		
		void ClearEquations(const std::vector<std::string>& _equations);
		
		void ClearParameters(const std::vector<std::string>& _parameterNames);
		
		void ClearSpecies(const std::vector<std::string>& _speciesNames);

		inline void LinkOperandToOperation(const std::string& _operandName, const std::string& _operationName)
		{
			operandsToOperations.emplace(_operandName, _operationName);
		}
	};
}