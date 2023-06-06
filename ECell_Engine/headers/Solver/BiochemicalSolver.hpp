#pragma once

namespace ECellEngine::Data
{
	class BiochemicalModule;
}

#include "Solver.hpp"

namespace ECellEngine::Solvers
{
	class BiochemicalSolver : public Solver
	{
	protected:
		const ECellEngine::Data::BiochemicalModule* module;

	public:
		BiochemicalSolver(ECellEngine::Data::DataState& _dataState, char* _name = "NewBioChemicalSolver") :
			Solver(_dataState, _name), module{ nullptr }
		{

		}

		virtual void Initialize(const ECellEngine::Data::Module* _module) override;

		virtual void Update(const ECellEngine::Core::Timer& _timer) override = 0;
	};
}