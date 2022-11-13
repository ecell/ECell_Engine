#pragma once

#include <vector>

#include "Module.hpp"
#include "Parameter.hpp"
#include "Reaction.hpp"
#include "Species.hpp"

namespace ECellEngine::Data
{
	class BiochemicalModule : public Module
	{
	protected:
		std::vector<Reaction> reactions;
		std::vector<SimpleParameter> simpleParameters;
		std::vector<Species> species;

	public:
		BiochemicalModule(DataState* _dataState) :
			Module(_dataState)
		{

		}

		virtual void Initialize() override = 0;

		virtual const Reaction* GetReaction(const int& _idx) const noexcept = 0;

		virtual const SimpleParameter* GetSimpleParameter(const int& _idx) const noexcept = 0;

		virtual const Species* GetSpecies(const int& _idx) const noexcept = 0;
	};
}