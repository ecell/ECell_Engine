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
		float absoluteTolerance = 0.0001f;

		/*!
		@brief The relative tolerance used to compute the error.
		*/
		float relativeTolerance = 0.0001f;

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
			return t-h_next >= _t;
		}
	};
}