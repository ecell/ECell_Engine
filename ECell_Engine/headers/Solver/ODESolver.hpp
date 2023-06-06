#pragma once

#include "Solver/BiochemicalSolver.hpp"

namespace ECellEngine::Solvers
{
	class ODESolver : public BiochemicalSolver
	{
	private:
		std::vector<Maths::Operation> system;

	public:
		
		ODESolver(ECellEngine::Data::DataState& _dataState, char* _name = "NewODESolver") :
			BiochemicalSolver(_dataState, _name)
		{
			system.reserve(dataState.GetAllSpecies().size());
		}

		virtual void Initialize(const ECellEngine::Data::Module* _module) override;

		virtual void Update(const ECellEngine::Core::Timer& _timer) override = 0;

	};
}