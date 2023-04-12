#pragma once

namespace ECellEngine::Solvers
{
	class Solver;
}

#include "DataState.hpp"
#include "Logger.hpp"
#include "Solver.hpp"

namespace ECellEngine::Data
{
	class Module
	{
	private:
		char* name;

	protected:
		DataState& dataState;

	public:
		Module(DataState& _dataState, char* _name = "NewAsset") :
			dataState{_dataState}, name{_name}
		{

		}

		inline char* GetName() noexcept
		{
			return name;
		}

		inline void SetName(char* _name) noexcept
		{
			name = _name;
		}

		virtual bool IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept = 0;

	};
}