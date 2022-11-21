#pragma once

#include "DataState.hpp"
#include "SolverTypes.hpp"

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

		virtual const bool& CheckSolverType(const ECellEngine::Solvers::SolverTypes& _solverType) noexcept = 0;

	};
}