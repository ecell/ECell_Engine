#include "Maths/MathUtility.hpp"
#include "Solver/ODE/Stepper.hpp"

float ECellEngine::Solvers::ODE::Stepper::Next(float _err, unsigned short _q) noexcept
{
	float fac1 = fac * powf(1.f / _err, 1.f / (_q + 1));
	fac1 = ECellEngine::Maths::max(facmin, ECellEngine::Maths::min(facmax, fac1));
	h *= fac1;
	return h;
}