#pragma once

namespace ECellEngine::Data
{
	class Module;
}

#include "DataState.hpp"
#include "Module.hpp"
#include "Timer.hpp"

namespace ECellEngine::Solvers
{
	class Solver
	{
	private:
		float time;

	protected:
		ECellEngine::Data::DataState& dataState;

		inline void SetTime(const float _newTime) noexcept
		{
			time = _newTime;
		}

	public:
		Solver(ECellEngine::Data::DataState& _dataState) :
			dataState{ _dataState }
		{
			time = 0;
		}

		inline const float GetTime() noexcept
		{
			return time;
		}

		virtual void Initialize(const ECellEngine::Data::Module*) = 0;

		virtual void Update(const ECellEngine::Core::Timer& _timer) = 0;
	};
}