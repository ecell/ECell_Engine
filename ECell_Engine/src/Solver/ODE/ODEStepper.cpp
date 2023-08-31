#include "Maths/MathUtility.hpp"
#include "Solver/ODE/ODEStepper.hpp"

float ECellEngine::Solvers::ODE::ODEStepper::ComputeDenseOutputIncrement(const float _bsp[], const float _theta, const float _ks[],
	const unsigned _eqIdx, const unsigned short _order, const unsigned short _stage)
{
	float res = 0;
	float uTheta = 0;
	for (unsigned short s = 0; s < _stage; ++s)
	{
		uTheta = 0;
		for (unsigned short j = 0; j < _order; ++j)
		{
			uTheta += _bsp[s * _order + j] * pow(_theta, j+1);
		}
		res += uTheta * _ks[_eqIdx * _stage + s];
	}
	return res;
}

float ECellEngine::Solvers::ODE::ODEStepper::ComputeDenseOutputTime(const float _t, const float _t0, const float _t1) const noexcept
{
	return (_t - _t0) / (_t1 - _t0);
}

bool ECellEngine::Solvers::ODE::ODEStepper::ComputeError(float _y0[], float _yEst1[], float _yEst2[], unsigned short _count) noexcept
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

float ECellEngine::Solvers::ODE::ODEStepper::ComputeNext(unsigned short _q) noexcept
{
	h *= ECellEngine::Maths::max(facmin, ECellEngine::Maths::min(facmax, fac * powf(1.f / error, 1.f / (_q + 1))));

	return h;
}

float ECellEngine::Solvers::ODE::ODEStepper::ComputeTimeForValue(const float _targetValue, const float _y0,
	const float _y1, const float _bsp[], const float _ks[], const unsigned _eqIdx, const unsigned short _order,
	const unsigned short _stage)
{
	float deltaStep = fabsf(_y1 - _y0);
	float deltaTarget = fabsf(_targetValue - _y0) + computeTimeThetaTolerance * deltaStep;
	float a = 0.f; 
	float b = 1.f;
	float theta = 0.5f;
	float deltaTheta = h * ComputeDenseOutputIncrement(_bsp, theta, _ks, _eqIdx, _order, _stage);

	while (fabsf(deltaTarget - deltaTheta)/deltaStep > computeTimeThetaTolerance)
	{
		if (deltaTarget < deltaTheta)
		{
			b = theta;
		}
		else
		{
			a = theta;
		}
		theta = (a + b) * 0.5f;
		deltaTheta = h * ComputeDenseOutputIncrement(_bsp, theta, _ks, _eqIdx, _order, _stage);
	}

	return timer.elapsedTime + theta * h;
}

float ECellEngine::Solvers::ODE::ODEStepper::ForceNext(float _deltaTime) noexcept
{
	timer.Increment(_deltaTime);

	return timer.elapsedTime;
};

float ECellEngine::Solvers::ODE::ODEStepper::Next() noexcept
{
	timer.Increment(h);

	return timer.elapsedTime;
}