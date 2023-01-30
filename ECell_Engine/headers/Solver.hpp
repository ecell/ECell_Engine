#pragma once


namespace ECellEngine::Data
{
	class Module;
}

#include "DataState.hpp"
#include "Module.hpp"

namespace ECellEngine::Solvers
{
	class Solver
	{
	protected:
		ECellEngine::Data::DataState& dataState;

	public:
		Solver(ECellEngine::Data::DataState& _dataState) :
			dataState{ _dataState }
		{

		}

		virtual void Initialize(const ECellEngine::Data::Module&) = 0;

		virtual void Update(const ECellEngine::Data::Module& _module, const float& _deltaTime) = 0;
	};
}