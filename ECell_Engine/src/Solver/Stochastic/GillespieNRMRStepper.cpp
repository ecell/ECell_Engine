#include "Solver/Stochastic/GillespieNRMRStepper.hpp"

void ECellEngine::Solvers::Stochastic::GillespieNRMRStepper::ComputeDepReactionNextTime()
{
	tau_new = (a_old / a_new) * (tau_old - timer.elapsedTime) + timer.elapsedTime; //muTau.second is current time
}

float ECellEngine::Solvers::Stochastic::GillespieNRMRStepper::Next() noexcept
{
	timer.Increment(h);

	return timer.elapsedTime;
}

void ECellEngine::Solvers::Stochastic::GillespieNRMRStepper::Reset() noexcept
{
	timer.ResetTimes();
	tau_old = 0.0f;
	tau_new = 0.0f;
	a_old = 0.0f;
	a_new = 0.0f;
	muTau.first = 0;
	muTau.second = 0.0f;
	reactionName = "";
	h = 0.0f;
}