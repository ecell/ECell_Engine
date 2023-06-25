#include "Maths/MathUtility.hpp"
#include "Solver/ODE/Stepper.hpp"

bool ECellEngine::Solvers::ODE::Stepper::ComputeError(float _y0[], float _yEst1[], float _yEst2[], unsigned short _count) noexcept
{
	error = 0;
	for (unsigned short i = 0; i < _count; ++i)
	{
		sc = absoluteTolerance + relativeTolerance * Maths::max(fabs(_y0[i]), fabs(_yEst1[i]));
		error += powf((_yEst1[i] - _yEst2[i]) / sc, 2);
	}
	error = sqrtf(error / _count);

	return error < 1.f;
}

float ECellEngine::Solvers::ODE::Stepper::ComputeNext(unsigned short _q) noexcept
{
	h_prev = h_next;
	h_next *= ECellEngine::Maths::max(facmin, ECellEngine::Maths::min(facmax, fac * powf(1.f / error, 1.f / (_q + 1))));

	return h_next;
}

float ECellEngine::Solvers::ODE::Stepper::Next() noexcept
{
	t += h_next;
	return h_next;
}