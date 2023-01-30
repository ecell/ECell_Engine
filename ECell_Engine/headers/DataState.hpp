#pragma once

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
		std::unordered_map<std::string, Reaction> reactions;
		std::unordered_map<std::string, ComputedParameter> computedParameters;
		std::unordered_map<std::string, SimpleParameter> simpleParameters;
		std::unordered_map<std::string, Species> species;

	public:
		DataState()
		{

		}

		inline float GetElapsedTime() const
		{
			return elapsedTime;
		}

		inline float GetKineticLaw(const std::string _kineticLawName) const
		inline Reaction& GetReaction(const std::string _reactionName)
		{
			return reactions.at(_reactionName);
		}

		inline ComputedParameter& GetComputedParameter(const std::string _parameterName)
		{
			return computedParameters.at(_parameterName);
		}

		inline SimpleParameter& GetSimpleParameter(const std::string _parameterName)
		{
			return simpleParameters.at(_parameterName);
		}

		inline Species& GetSpecies(const std::string _speciesName)
		{
			return species.at(_speciesName);
		}

		inline bool AddReaction(const std::string _reactionName,
								const std::vector<std::string> _products,
								const std::vector<std::string> _reactants,
								const Operation _kineticLaw)
		{
			return reactions.emplace(_reactionName, Reaction(_reactionName, _products, _reactants, _kineticLaw)).second;
		}
		
		inline bool AddComputedParameter(const std::string _parameterName, const Operation _parameterOp)
		{
			return computedParameters.emplace(_parameterName, ComputedParameter(_parameterName, _parameterOp)).second;
		}

		inline bool AddSimpleParameter(const std::string _parameterName, const float _value)
		{
			return simpleParameters.emplace(_parameterName, SimpleParameter(_parameterName, _value)).second;
		}
		
		inline bool AddSpecies(const std::string _speciesName, const float _quantity)
		{
			return species.emplace(_speciesName, Species(_speciesName, _quantity)).second;
		}

		inline void SetElapsedTime(const float _elapsedTime)
		{
			elapsedTime = _elapsedTime;
		}

		/*inline void SetReaction(const std::string _reactionName, const float _kineticLawValue)
		{
			reactions[_reactionName] = _kineticLawValue;
		}

		inline void SetParameter(const std::string _parameterName, const float _parameterValue)
		{
			parameters[_parameterName] = _parameterValue;
		}

		inline void SetSpecies(const std::string _speciesName, const float _speciesValue)
		{
			species[_speciesName] = _speciesValue;
		}*/

		void ClearReactions(const std::vector<std::string>& _reactionNames);
		
		void ClearComputedParameters(const std::vector<std::string>& _parameterNames);
		
		void ClearSimpleParameters(const std::vector<std::string>& _parameterNames);
		
		void ClearSpecies(const std::vector<std::string>& _speciesNames);

	};
}