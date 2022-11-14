#pragma once

#include "DataState.hpp"
#include "Module.hpp"

using namespace ECellEngine::Data;

namespace ECellEngine::Solvers
{
	class Solver
	{
	protected:
		DataState* datastate;
		bool enabled;

	public:
		Solver(DataState* _dataState) :
			datastate{ _dataState }
		{

		}

		virtual void Initialize(const Module&) = 0;

		inline const bool IsEnabled() const noexcept
		{
			return enabled;
		}

		virtual void Update(const Module& _module, const float& _deltaTime) = 0;
	};
}