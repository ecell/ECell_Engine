#pragma once

#include "Solver.hpp"

namespace ECellEngine::Solvers
{
	class BiochemicalSolver : public Solver
	{
	public:
		BiochemicalSolver(ECellEngine::Data::DataState& _dataState) :
			Solver(_dataState)
		{

		}

		virtual void Initialize(const ECellEngine::Data::Module*) override = 0;

		virtual void Update(const float& _deltaTime) override = 0;
	};
}