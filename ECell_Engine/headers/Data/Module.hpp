#pragma once

namespace ECellEngine::Solvers
{
	class Solver;
}

#include "Data/DataState.hpp"
#include "Logging/Logger.hpp"
#include "Solver/Solver.hpp"

namespace ECellEngine::Data
{
	class Module
	{
	private:
		char name[64];

	protected:
		DataState& dataState;

	public:
		Module(DataState& _dataState, char* _name) :
			dataState{_dataState}, name{*_name}
		{

		}

		inline char* GetName() noexcept
		{
			return name;
		}

		inline void SetName(char* _name) noexcept
		{
			std::memset(name, '\0', std::max(std::strlen(name), std::strlen(_name)));
			std::memcpy(name, _name, std::strlen(_name));
		}

		virtual bool IsValidSolverType(const ECellEngine::Solvers::Solver* _solver) noexcept = 0;

	};
}