#pragma once

#include "DataState.hpp"
#include "Module.hpp"
#include "SolverTypes.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::Solvers
{
	class Solver
	{
	protected:
		DataState* datastate;
		SolverTypes solverType;

	public:
		Solver(DataState* _dataState) :
			datastate{ _dataState }
		{

		}

		inline const SolverTypes& GetType() const noexcept
		{
			return solverType;
		}

		virtual void Initialize(const Module&) = 0;

		virtual void Update(const Module& _module, const float& _deltaTime) = 0;
	};
}