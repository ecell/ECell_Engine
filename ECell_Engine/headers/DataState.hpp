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

		inline float GetKineticLaw(const std::string _kineticLawId) const
		{
			return parameters.at(_kineticLawId);
		}

		inline float GetParameter(const std::string _parameterId) const
		{
			return parameters.at(_parameterId);
		}

		inline float GetSpecies(const std::string _speciesId) const
		{
			return parameters.at(_speciesId);
		}

		inline void AddKineticLaw(const std::string _kineticLawId, const float _kineticLawValue)
		{
			kineticLaws.try_emplace(_kineticLawId, _kineticLawValue);
		}
		
		inline void AddParameter(const std::string _parameterId, const float _parameterValue)
		{
			parameters.try_emplace(_parameterId, _parameterValue);
		}
		
		inline void AddSpecies(const std::string _speciesId, const float _speciesValue)
		{
			species.try_emplace(_speciesId, _speciesValue);
		}

		inline void SetElapsedTime(const float _elapsedTime)
		{
			elapsedTime = _elapsedTime;
		}

		inline void SetKineticLaw(const std::string _kineticLawId, const float _kineticLawValue)
		{
			kineticLaws[_kineticLawId] = _kineticLawValue;
		}

		inline void SetParameter(const std::string _parameterId, const float _parameterValue)
		{
			parameters[_parameterId] = _parameterValue;
		}

		inline void SetSpecies(const std::string _speciesId, const float _speciesValue)
		{
			species[_speciesId] = _speciesValue;
		}

		void ClearKineticLaws(const std::vector<std::string> _kineticLawIds);
		
		void ClearParameters(const std::vector<std::string> _parameterIds);
		
		void ClearSpecies(const std::vector<std::string> _speciesIds);

	};
}