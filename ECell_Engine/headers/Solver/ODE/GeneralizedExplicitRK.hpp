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
		@brief External equations (not ODE) that are true at all time and must
				be accounted for when integrating the system of differential
				equations.
		*/
		std::vector<Maths::Equation*> externalEquations;

		/*!
		@brief The number of external equations.
		*/
		unsigned short extEqSize = 0;

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
		@brief The size of the system of differential equations.
		*/
		unsigned short systemSize = 0;

		/*!
		@brief The stepper to control the step size.
		*/
		Stepper stepper;

		/*!
		@brief The subset of triggers from the datastate that might be triggered
				during the solver step update.
		@details The first element of the pair is the trigger, the second element
				is the index of the equation modifying the target or the threshold
				value of the trigger.
		*/
		std::vector<std::pair<Core::Trigger<Operand*, Operand*>*, unsigned short>> triggersOnODE;

		/*!
		@brief The subset of triggers from the datastate that might be triggered
				during the update of the external equations.
		*/
		std::vector<std::pair<Core::Trigger<Operand*, Operand*>*, unsigned short>> triggersOnExtEq;

		/*!
		@brief A buffer value to store the time of the earliest trigger that 
				must be triggered within a the solver step update.
		*/
		float triggerTriggerTime = 0.0f;

		/*!
		@brief Indicates whether the solver is set with a method enabling error
				control.
		*/
		bool errorControl = false;

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
		@brief A buffer for the first estimation of the next values of the
				variables modified by the differential equations stored in
				::system.
		*/
		float* ynp1 = nullptr;

		/*!
		@brief A buffer for the second estimation of the next values of the
				variables modified by the differential equations stored in
				::system.
		*/
		float* ynp12 = nullptr;

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

		/*!
		@bief loops through all triggers stored in the datastate and adds them
			to the ::triggers vector if their targets or threshold value are
			updated by the external equations.
		*/
		void ScanForTriggersOnExtEq() noexcept;

		/*!
		@brief loops through all triggers stored in the datastate and adds them
			to the ::triggers vector if their targets or threshold value are
			updated by the system of differential equations.
		*/
		void ScanForTriggersOnODE() noexcept;

		/*!
		@brief Integrates the system of differential equations for methods without
				error control.
		*/
		void UpdateClassic(const ECellEngine::Core::Timer& _timer);

		/*!
		@brief Integrates the system of differential equations for methods with
				error control.
		*/
		void UpdateWithErrorControl(const ECellEngine::Core::Timer& _timer);

	public:
		
		GeneralizedExplicitRK(ECellEngine::Data::DataState& _dataState, const std::string& _name) :
			BiochemicalSolver(_dataState, _name)
		{

		}

		~GeneralizedExplicitRK()
		{
			delete[] yn;
			delete[] ynp1;
			delete[] ynp12;
			delete[] yn_ext;
		}

		/*!
		@brief Sets this solver to use the classic Runge-Kutta method.
		@details Fixed step size, no error control.
		@see ECellEngine::Solvers::ODE::Stepper::SetToClassicRK4()
		*/
		void SetToClassicRK4() noexcept;

		/*!
		@brief Sets this solver to use the Dormand-Prince method.
		@details Controlled step size, error control.
		@see ECellEngine::Solvers::ODE::Stepper::SetToDormandPrince54()
		*/
		void SetToDormandPrince5() noexcept;

		/*!
		@brief Sets this solver to use the Merson method.
		@details Controlled step size, error control.
		@see ECellEngine::Solvers::ODE::Stepper::SetToMerson4()
		*/
		void SetToMerson4() noexcept;

		virtual void Initialize(const ECellEngine::Data::Module* _module) override;

		virtual void Update(const ECellEngine::Core::Timer& _timer) override;

	};
}