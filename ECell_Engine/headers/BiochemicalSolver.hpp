#pragma once

#include "Solver.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::Solvers
{
	class BiochemicalSolver : public Solver
	{
	public:
		BiochemicalSolver(DataState* _dataState) :
			Solver(_dataState)
		{

		}

		virtual void Initialize(const Module&) override = 0;

		virtual void Update(const Module& _module, const float& _deltaTime) override = 0;
	};
}