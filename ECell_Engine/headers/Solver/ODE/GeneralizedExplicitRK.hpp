#pragma once

#include "Solver/BiochemicalSolver.hpp"
#include "Solver/ODE/ExplicitRKCoefficients.hpp"
#include "Solver/ODE/Stepper.hpp"

namespace ECellEngine::Solvers::ODE
{
	/*!
	@brief A solver for ordinary differential equations using the generalized
			explicit Runge-Kutta method.
	*/
	class GeneralizedExplicitRK final : public BiochemicalSolver
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

		/*!
		@brief External equations (not ODE) that are true at all time and must
				be accounted for when integrating the system of differential
				equations.
		*/
		const std::unordered_map<std::string, std::shared_ptr<Maths::Equation>>* externalEquations = nullptr;
		
		/*!
		@brief The size of the system of differential equations.
		*/
		unsigned short systemSize = 0;

		/*!
		@brief The stepper to control the step size.
		*/
		Stepper stepper;

		/*!
		@brief The coefficients of the generalized explicit Runge-Kutta method.
		*/
		ExplicitRKCoefficients coeffs;

		/*!
		@brief A buffer to the current values of the variables modified by the
				differential equations stored in ::system.
		*/
		float* yn = nullptr;

		/*!
		@brief A buffer to the current values of the variables modified by the
				external equations.
		*/
		float* yn_ext = nullptr;

		/*!
		@brief Helper function to build the right hand side of the differential
				equation.
		@todo This fuction is only working under the assumption that we are dealing
				with a system of differential equations for a biochemical system.
		*/
		void BuildEquationRHS(Operation& _outRHS, std::vector<Maths::Operation>& _flux);

	public:
		
		GeneralizedExplicitRK(ECellEngine::Data::DataState& _dataState, const std::string& _name) :
			BiochemicalSolver(_dataState, _name)
		{

		}

		~GeneralizedExplicitRK()
		{
			delete[] yn;
			delete[] yn_ext;
		}

		virtual void Initialize(const ECellEngine::Data::Module* _module) override;

		virtual void Update(const ECellEngine::Core::Timer& _timer) override;

	};
}