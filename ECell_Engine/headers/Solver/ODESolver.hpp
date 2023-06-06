#pragma once

#include "Solver/BiochemicalSolver.hpp"

namespace ECellEngine::Solvers
{
	class ODESolver : public BiochemicalSolver
	{
	private:
		/*!
		@brief The system of differential equations.
		@details Be wary that, the left hand side are the variables
				to be solved by the differential equation (right hand side) and NOT
				their derivative.

				Indeed, the object Equation represents something of the form: lhs = rhs.
				For a differential equation, we have something like: dx/dt = f(x, t).
				But, in our case, we have something like: x = f(x, t).

				This is for convenience, namely to easily access the variable to be
				updated when solving the differential equation.
		*/
		std::vector<Maths::Equation> system;

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