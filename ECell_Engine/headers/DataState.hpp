#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace ECellEngine::Data
{
	struct DataState
	{
	private:
		float elapsedTime;
		std::unordered_map<const std::string, float> kineticLaws;
		std::unordered_map<const std::string, float> parameters;
		std::unordered_map<const std::string, float> species;

	public:
		DataState()
		{

		}

		inline float GetElapsedTime() const
		{
			return elapsedTime;
		}

		inline float GetKineticLaw(const std::string _kineticLawName) const
		{
			return parameters.at(_kineticLawName);
		}

		inline float GetParameter(const std::string _parameterName) const
		{
			return parameters.at(_parameterName);
		}

		inline float GetSpecies(const std::string _speciesName) const
		{
			return parameters.at(_speciesName);
		}

		inline void AddKineticLaw(const std::string _kineticLawName, const float _kineticLawValue)
		{
			kineticLaws.try_emplace(_kineticLawName, _kineticLawValue);
		}
		
		inline void AddParameter(const std::string _parameterName, const float _parameterValue)
		{
			parameters.try_emplace(_parameterName, _parameterValue);
		}
		
		inline void AddSpecies(const std::string _speciesName, const float _speciesValue)
		{
			species.try_emplace(_speciesName, _speciesValue);
		}

		inline void SetElapsedTime(const float _elapsedTime)
		{
			elapsedTime = _elapsedTime;
		}

		inline void SetKineticLaw(const std::string _kineticLawName, const float _kineticLawValue)
		{
			kineticLaws[_kineticLawName] = _kineticLawValue;
		}

		inline void SetParameter(const std::string _parameterName, const float _parameterValue)
		{
			parameters[_parameterName] = _parameterValue;
		}

		inline void SetSpecies(const std::string _speciesName, const float _speciesValue)
		{
			species[_speciesName] = _speciesValue;
		}

		void ClearKineticLaws(const std::vector<std::string>& _kineticLawNames);
		
		void ClearParameters(const std::vector<std::string>& _parameterNames);
		
		void ClearSpecies(const std::vector<std::string>& _speciesNames);

	};
}