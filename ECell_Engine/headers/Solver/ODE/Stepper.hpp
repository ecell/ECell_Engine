#pragma once

namespace ECellEngine::Solvers::ODE
{
	/*!
	@brief Structure to control the step size of an ODE solver.
	*/
	struct Stepper
	{
		/*!
		@brief The step size of the next step.
		*/
		float h_next = 0.001f;

		/*!
		@brief The step size of the previous step.
		*/
		float h_prev = 0.001f;

		/*!
		@brief The current time.
		*/
		float t = 0.f;

		/*!
		@brief A parameter used to control the step size.
		*/
		const float fac = 0.8f;

		/*!
		@brief The value by which to multiply the step size on increase.
		*/
		const float facmax = 5.f;

		/*!
		@brief The value by which to multiply the step size on decrease.
		*/
		const float facmin = 0.2f;

		/*!
		@brief The error of the last step.
		*/
		float error = 0.f;

		/*!
		@brief The scaling factor used to compute the error.
		@details It is computed as: absoluteTolerance + relativeTolerance * max(|y0|, |yEst1|).
		*/
		float sc = 0.f;

		/*!
		@brief The absolute tolerance used to compute the error.
		*/
		float absoluteTolerance = 0.000001f;

		/*!
		@brief The relative tolerance used to compute the error.
		*/
		float relativeTolerance = 0.000001f;

		/*!
		@brief Computes the dense output increment.
		@details It is computed as: sum_{s=0}^{stage-1} sum_{j=0}^{order-1} b_{s,j} * theta^(j+1) * k_{eqIdx+s}.
				 The stage-1 and order-1 are used because the arrays are 0-based.
				 You must add this value to the values at the beginning of the step
				 (y0) to get the interpolation at @p _theta.
		@param _bsp The b_{s,j} coefficients of the polynom in _theta.
		@param _theta The value at which to compute the interpolation. It must be
					  between 0 and 1.
		@param _ks The integration coefficients of the Runge-Kutta method.
		@param _eqIdx The index of the equation for which to compute the dense
					  output increment. It is used to access the correct values in
					  @p _ks.
		@param _order The order of the polynom in _theta.
		@param _stage The number of stages of the underlying Runge-Kutta method
					  employed to solve the ODE.
		*/
		float ComputeDenseOutputIncrement(const float _bsp[], const float _theta, const float _ks[], const unsigned _eqIdx, const unsigned short _order, const unsigned short _stage);

		/*!
		@brief Computes the error of the steps that produced the estimations
				stored in @p _yEst1 and @p _yEst2 from @p _y0.
		@param _y0 The values at the beginning of the step.
		@param _yEst1 The first estimation of the values at the end of the step.
		@param _yEst2 The second estimation of the values at the end of the step.
		@param _count The number of values in the arrays.
		@returns True if the error is less than 1.
		*/
		bool ComputeError(float _y0[], float _yEst1[], float _yEst2[], unsigned short _count) noexcept;

		/*!
		@brief Computes the step size of the next step based on the error of the
				previous step.
		@details You should call this function after @ref ComputeError.
		*/
		float ComputeNext(unsigned short _q) noexcept;

		/*!
		@brief Advances the time by ::h_next.
		@details t += h_next.
		*/
		float Next() noexcept;
		
		/*!
		@brief Returns true if the next step is less than or equal to the given
				time.
		*/
		inline bool NextLEQ(float _t) const noexcept
		{
			return t+h_next <= _t;
		}

		/*!
		@brief Returns true if the next step is greater than or equal to the
				given time.
		*/
		inline bool NextGEQ(float _t) const noexcept
		{
			return t+h_next >= _t;
		}
	};
}