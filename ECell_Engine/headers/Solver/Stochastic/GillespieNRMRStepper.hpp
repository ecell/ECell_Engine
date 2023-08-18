#include <string>
#include "Solver/Stepper.hpp"

namespace ECellEngine::Solvers::Stochastic
{
	/*!
	@brief A stepper for the GillespieNRMR solver.
	@see ECellEngine::Solvers::GillespieNRMR
	*/
	struct GillespieNRMRStepper : public ECellEngine::Solvers::Stepper
	{
		/*!
		@brief A buffer for the old value of the propensity.
		*/
		float a_old = 0.f;

		/*!
		@brief A buffer for the new value of the propensity.
		*/
		float a_new = 0.f;
		
		/*!
		@brief A buffer for the old value for the firing time
				of a reaction.
		@details Scales on the elapsed time.
		*/
		float tau_old = 0.f;

		/*!
		@brief A buffer for the new value for the firing time
				of a reaction.
		@details Scales on the elapsed time.
		*/
		float tau_new = 0.f;

		/*!
		@brief A buffer for the root of the tau min heap.
		@details The root is the reaction with the smallest firing time.
		*/
		std::pair<int, float> muTau = { 0, 0.f };

		/*!
		@brief A buffer for the name of a reaction.
		@remarks Used multiple times in an update step.
		*/
		std::string reactionName = "";

		/*!
		@brief A buffer for the next firing delta time of the root reaction.
		*/
		float h = 0.f;

		/*!
		@brief Computes the next firing time of a reaction that has been impacted
				by the firing of the root reaction (change of species quantities).
		*/
		void ComputeDepReactionNextTime();

		/*!
		@brief Increments the ::time.elapsedTime by ::h.
		*/
		float Next() noexcept override;

		/*!
		@returns True if ::timer.elapsedTime + ::h <= @p _t ; False otherwise.
		*/
		inline bool IsNextLEQ(float _t) const noexcept override
		{
			return timer.elapsedTime + h <= _t;
		}

		/*!
		@returns True if ::timer.elapsedTime + ::h < @p _t ; False otherwise.
		*/
		inline bool IsNextLE(float _t) const noexcept override
		{
			return timer.elapsedTime + h < _t;
		}

		/*!
		@returns True if ::timer.elapsedTime + ::h >= @p _t ; False otherwise.
		*/
		inline bool IsNextGEQ(float _t) const noexcept override
		{
			return timer.elapsedTime + h >= _t;
		}

		/*!
		@returns True if ::timer.elapsedTime + ::h > @p _t ; False otherwise.
		*/
		inline bool IsNextGE(float _t) const noexcept override
		{
			return timer.elapsedTime + h > _t;
		}
	};
}