#pragma once

namespace ECellEngine::Solvers::ODE
{
	/*!
	@brief Structure to control the step size of an ODE solver.
	*/
	struct Stepper
	{
		/*!
		@brief The step size.
		*/
		float h = 0.001f;

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

		float Next(float _err, unsigned short _q) noexcept;
		
		/*!
		@brief Returns true if the next step is less than or equal to the given
				time.
		*/
		inline bool NextLEQ(float _t) const noexcept
		{
			return t+h <= _t;
		}

		/*!
		@brief Returns true if the next step is greater than or equal to the
				given time.
		*/
		inline bool NextGEQ(float _t) const noexcept
		{
			return t-h >= _t;
		}
	};
}