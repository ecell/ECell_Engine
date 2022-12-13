#pragma once


namespace ECellEngine::Data
{
	class Module;
}

#include "DataState.hpp"
#include "Module.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::Solvers
{
	class Solver
	{
	protected:
		DataState* datastate;

	public:
		Solver(DataState* _dataState) :
			datastate{ _dataState }
		{

		}

		virtual void Initialize(const Module&) = 0;

		virtual void Update(const Module& _module, const float& _deltaTime) = 0;
	};
}