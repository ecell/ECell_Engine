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
		BiochemicalSolver(ECellEngine::Data::DataState& _dataState) :
			Solver(_dataState)
		{

		}

		virtual void Initialize(const ECellEngine::Data::Module* _module) override;

		virtual void Update(const float& _deltaTime) override = 0;
	};
}