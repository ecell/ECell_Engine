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
		char name[64];

	protected:
		ECellEngine::Data::DataState& dataState;

	public:
		Solver(ECellEngine::Data::DataState& _dataState, char* _name) :
			dataState{ _dataState }, name{*_name}
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

		virtual void Initialize(const ECellEngine::Data::Module*) = 0;

		virtual void Update(const ECellEngine::Core::Timer& _timer) = 0;
	};
}