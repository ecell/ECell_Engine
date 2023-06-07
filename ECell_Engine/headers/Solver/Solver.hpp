#pragma once

namespace ECellEngine::Data
{
	class Module;
}

#include "Data/DataState.hpp"
#include "Data/Module.hpp"
#include "Core/Timer.hpp"

namespace ECellEngine::Solvers
{
	class Solver
	{
	private:
		std::string name;

	protected:
		ECellEngine::Data::DataState& dataState;

	public:
		Solver(ECellEngine::Data::DataState& _dataState, const std::string& _name) :
			dataState{ _dataState }, name{_name}
		{

		}

		inline std::string& GetName() noexcept
		{
			return name;
		}

		inline void SetName(std::string& _name) noexcept
		{
			name = _name;
		}

		virtual void Initialize(const ECellEngine::Data::Module*) = 0;

		virtual void Update(const ECellEngine::Core::Timer& _timer) = 0;
	};
}