#pragma once

namespace ECellEngine::Data
{
	class BiochemicalModule;
}

#include "Solver.hpp"

namespace ECellEngine::Solvers
{
	class BiochemicalSolver : public Solver
	{
	protected:
		const ECellEngine::Data::BiochemicalModule* module;

	public:
		BiochemicalSolver(ECellEngine::Data::DataState& _dataState, const std::string& _name) :
			Solver(_dataState, _name), module{ nullptr }
		{
			//ECellEngine::Logging::Logger::GetSingleton().LogError(std::to_string(dataState.GetElapsedTime()));

		}

		virtual void Initialize(const ECellEngine::Data::Module* _module) override = 0;

		virtual void Start() override = 0;

		virtual void Update(const ECellEngine::Core::Timer& _timer) override = 0;
	};
}