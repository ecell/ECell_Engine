#pragma once

#include "Solver/BiochemicalSolver.hpp"

namespace ECellEngine::Solvers
{
	class ODESolver final : public BiochemicalSolver
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
		unsigned short nbEquations;

		float solveDeltaTime = 0.01f;
		float halfSolveDeltaTime = solveDeltaTime * 0.5f;
		float oneSixthSolveDeltaTime = solveDeltaTime / 6.f;
		float finaldt = 0;
		float halfFinaldt = 0;
		float oneSixthFinaldt = 0;
		float solveCurrentTime = 0.f;

		float* k1 = nullptr;
		float* k2 = nullptr;
		float* k3 = nullptr;
		float* k4 = nullptr;
		float* yn = nullptr;

	public:
		
		ODESolver(ECellEngine::Data::DataState& _dataState, const std::string& _name) :
			BiochemicalSolver(_dataState, _name)
		{
			
		}

		~ODESolver()
		{
			delete[] k1;
			delete[] k2;
			delete[] k3;
			delete[] k4;
			delete[] yn;
		}

		virtual void Initialize(const ECellEngine::Data::Module* _module) override;

		virtual void Update(const ECellEngine::Core::Timer& _timer) override;

	};
}