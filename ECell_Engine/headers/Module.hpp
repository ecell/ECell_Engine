#pragma once

namespace ECellEngine::Solvers
{
	class Solver;
}

#include "DataState.hpp"
#include "Solver.hpp"

namespace ECellEngine::Data
{
	class Module
	{
	protected:
		DataState* dataState;

	public:
		Module(DataState* _dataState) :
			dataState{_dataState}
		{

		}

		virtual bool IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept = 0;

	};
}