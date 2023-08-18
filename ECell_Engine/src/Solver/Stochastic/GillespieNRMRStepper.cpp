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