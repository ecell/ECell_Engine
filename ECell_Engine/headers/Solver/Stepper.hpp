#pragma once
#include "Core/Timer.hpp"

namespace ECellEngine::Solvers
{
	/*!
	@brief Structure to control the step size of an ODE solver.
	*/
	struct Stepper
	{
		Core::Timer timer;

		/*!
		@brief Computes the timer's next elapsed time value based on @p _deltaTime.
		@returns ::timer.elapsedTime.
		*/
		virtual float ForceNext(float _deltaTime) noexcept = 0;

		/*!
		@brief Computes the timer's next elapsed time value based on internal
				parameters.
		@returns ::timer.elapsedTime.
		*/
		virtual float Next() noexcept = 0;

		/*!
		@brief Checks whether the next elapsed time value is less or equal
				than @p _t.
		*/
		virtual bool IsNextLEQ(float _t) const noexcept = 0;

		/*!
		@brief whecks whether the next elapsed time value is less than @p _t.
		*/
		virtual bool IsNextLE(float _t) const noexcept = 0;

		/*!
		@brief Checks whether the next elapsed time value is greater or equal
				than @p _t.
		*/
		virtual bool IsNextGEQ(float _t) const noexcept = 0;

		/*!
		@brief Checks whether the next elapsed time value is greater than @p _t.
		*/
		virtual bool IsNextGE(float _t) const noexcept = 0;
	};
}