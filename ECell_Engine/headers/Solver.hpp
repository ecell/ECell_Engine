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
		char* name;

	protected:
		ECellEngine::Data::DataState& dataState;

		inline void SetTime(const float _newTime) noexcept
		{
			time = _newTime;
		}

	public:
		Solver(ECellEngine::Data::DataState& _dataState, char* _name = "NewSolver") :
			dataState{ _dataState }, name{_name}
		{
			time = 0;
		}

		inline const float GetTime() noexcept
		{
			return time;
		}

		inline char* GetName() noexcept
		{
			return name;
		}

		inline void SetName(char* _name) noexcept
		{
			name = _name;
		}

		virtual void Initialize(const ECellEngine::Data::Module*) = 0;

		virtual void Update(const ECellEngine::Core::Timer& _timer) = 0;
	};
}